#include "json.hpp"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <time.h>
#include <iomanip>
#include <sstream>

using namespace std;
using json = nlohmann::json;

#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "group.hpp"
#include "user.hpp"
#include "public.hpp"

// 记录当前系统登录的用户信息
User g_currentUser;
// 记录当前登录用户好友列表信息
vector<User> g_currentUserFriendList;
// 记录当前用户的群组列表信息
vector<Group> g_currentUserGroupList;
// 控制主菜单页面程序的布尔值
bool isMainMenuRunning = false;

// 显示当前登录成功用户的基本信息
void showCurrentUserDtata();
// 接受线程
void readTaskHandler(int clientfd);
// 获取系统时间
string getCurrentTime();
// 主聊天程序页面
void mainMenu(int clientfd);

// 聊天客户端程序实现,main线程用作发送线程，子线程用作接受线程
int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "command invalid! example: ./ChatClient 127.0.0.1 6000" << endl;
        exit(-1);
    }

    // 解析命令行参数传递的ip和port
    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    // 创建client端的socket
    int clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == clientfd)
    {
        cerr << "socket creat error" << endl;
        exit(-1);
    }

    // 填写client需要连接的server信息ip+port
    sockaddr_in server;
    memset(&server, 0, sizeof(sockaddr_in));

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(ip);

    // client和server进行连接
    if (-1 == connect(clientfd, (sockaddr *)&server, sizeof(sockaddr_in)))
    {
        cerr << "connect server error" << endl;
        close(clientfd);
        exit(-1);
    }

    // main线程用于接受用户输入，负责发送数据
    for (;;)
    {
        cout << "=========================" << endl;
        cout << "1. login" << endl;
        cout << "2. register" << endl;
        cout << "3. quit" << endl;
        cout << "=========================" << endl;
        cout << "choice: ";
        int choice = 0;
        cin >> choice;
        cin.get(); // 读掉缓冲区的回车
        switch (choice)
        {
        case 1:
        {
            int id = 0;
            char pwd[50] = {0};
            cout << "userid: ";
            cin >> id;
            cin.get();
            cout << "userpassword: ";
            cin.getline(pwd, 50);

            json js;
            js["msgid"] = LOGIN_MSG;
            js["id"] = id;
            js["password"] = pwd;
            string request = js.dump();

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (len == -1)
            {
                cerr << "send login msg error" << endl;
            }
            else
            {
                char buffer[1024] = {0};
                len = recv(clientfd, buffer, 1024, 0);
                if (-1 == len)
                {
                    cerr << "recv reg response error" << endl;
                }
                else
                {
                    json responsejs = json::parse(buffer);
                    if (0 != responsejs["errno"].get<int>())
                    {
                        cerr << responsejs["name"] << "is already exist, register error!" << endl;
                        continue;
                    }
                    else // 登录成功
                    {
                        // 记录当前用户id和name
                        g_currentUser.setId(responsejs["id"].get<int>());
                        g_currentUser.setName(responsejs["name"]);

                        // 记录好友列表
                        if (responsejs.contains("friends"))
                        {
                            vector<string> vec = responsejs["friends"];
                            for (string &str : vec)
                            {
                                json js = json::parse(str);
                                User user;
                                user.setId(js["id"].get<int>());
                                user.setName(js["name"]);
                                user.setState(js["state"]);
                                g_currentUserFriendList.push_back(user);
                            }
                        }

                        // 记录群组列表
                        if (responsejs.contains("groups"))
                        {
                            vector<string> vec = responsejs["groups"];
                            for (string &str : vec)
                            {
                                json js = json::parse(str);
                                Group group;
                                group.setId(js["id"].get<int>());
                                group.setName(js["groupname"]);
                                group.setDesc(js["groupdesc"]);

                                vector<string> vec2 = js["users"];
                                for (string ustr : vec2)
                                {
                                    GroupUser user;
                                    json ujs = json::parse(ustr);
                                    user.setId(ujs["id"].get<int>());
                                    user.setName(ujs["name"]);
                                    user.setState(ujs["state"]);
                                    user.setRole(ujs["role"]);
                                    group.getUsers().push_back(user);
                                }
                                g_currentUserGroupList.push_back(group);
                            }
                        }
                    }

                    // 显示用户的基本信息
                    showCurrentUserDtata();

                    // 显示当前用户的离线消息和群组消息
                    if (responsejs.contains("offlinemsg"))
                    {
                        vector<string> vec = responsejs["offlinemsg"];
                        for (string &str : vec)
                        {
                            json js = json::parse(str);
                            // time + [id] + name + "said:" + xxx
                            if (ONE_CHAT_MSG == js["msgid"].get<int>())
                            {
                                cout << js["time"].get<string>() << " [" << js["id"] << "] " << js["name"].get<string>()
                                     << " said: " << js["msg"].get<string>() << endl;
                            }
                            else
                            {
                                cout << "(" << js["time"].get<string>() << ") 群消息["
                                     << js["groupid"] << "] : [" << js["id"] << "] " << js["name"].get<string>()
                                     << " said: " << js["msg"].get<string>() << endl;
                            }
                        }
                    }
                    static int readthreadnumber = 0;
                    if (readthreadnumber == 0)
                    {
                        // 登录成功启动线程负责接收消息,该线程只启动一次
                        std::thread readTask(readTaskHandler, clientfd); // pthread_create
                        readTask.detach();                               // pthread_detach
                        readthreadnumber++;
                    }

                    isMainMenuRunning = true;
                    // 进入主菜单
                    mainMenu(clientfd);
                }
            }
        }
        break;
        case 2: // 注册
        {
            char name[50] = {0};
            char pwd[50] = {0};
            cout << "username: ";
            cin.getline(name, 50);
            cout << "userpassword: ";
            cin.getline(pwd, 50);

            json js;
            js["msgid"] = REG_MSG;
            js["name"] = name;
            js["password"] = pwd;
            string request = js.dump();

            int len = send(clientfd, request.c_str(), strlen(request.c_str()) + 1, 0);
            if (len == -1)
            {
                cerr << "send reg msg error" << endl;
            }
            else
            {
                char buffer[1024] = {0};
                len = recv(clientfd, buffer, 1024, 0);
                if (-1 == len)
                {
                    cerr << "recv reg response error" << endl;
                }
                else
                {
                    json responsejs = json::parse(buffer);
                    if (0 != responsejs["errno"].get<int>())
                    {
                        cerr << name << "is already exist, register error!" << endl;
                    }
                    else
                    {
                        cout << name << "register ok, userid = " << responsejs["id"]
                             << ", do not forget it" << endl;
                    }
                }
            }
        }
        break;
        case 3: // quit
        {
            close(clientfd);
            exit(0);
        }
        default:
            cerr << "incalid input!" << endl;
            break;
        }
    }
    return 0;
}

// 显示当前登录成功用户的基本信息
void showCurrentUserDtata()
{
    cout << "==============================login user====================================" << endl;
    cout << "current login user: " << g_currentUser.getName() << endl;
    cout << "============================friend list====================================" << endl;
    if (!g_currentUserFriendList.empty())
    {
        for (User &user : g_currentUserFriendList)
        {
            cout << user.getId() << " " << user.getName() << " " << user.getState() << endl;
        }
    }
    cout << "============================group list====================================" << endl;
    if (!g_currentUserGroupList.empty())
    {
        for (Group &group : g_currentUserGroupList)
        {
            cout << group.getId() << " " << group.getName() << " " << group.getDesc() << endl;
        }
    }
    cout << "正在进入客户端" << endl;
}
// 接受线程
void readTaskHandler(int clientfd)
{
    while (isMainMenuRunning)
    {
        char buffer[1024] = {0};
        int len = recv(clientfd, buffer, 1024, 0);
        if (-1 == len || 0 == len)
        {
            close(clientfd);
            exit(-1);
        }
        // 接收chatsever转发的数据反序列化生成json数据对象
        cout << buffer << endl;
        json js = json::parse(buffer);
        int msgtype = js["msgid"].get<int>();
        if (ONE_CHAT_MSG == msgtype)
        {
            cout << js["time"].get<string>() << " [" << js["id"] << "] " << js["name"].get<string>()
                 << " said: " << js["msg"].get<string>() << endl;
            continue;
        }

        if (GROUP_CHAT_MSG == msgtype)
        {
            cout << "(" << js["time"].get<string>() << ") 群消息["
                 << js["groupid"] << "] : [" << js["id"] << "] " << js["name"].get<string>()
                 << " said: " << js["msg"].get<string>() << endl;
            continue;
        }
    }
}
// 获取当前系统时间，返回格式：YYYY-MM-DD HH:MM:SS
string getCurrentTime()
{
    // 获取当前系统时间点
    auto now = chrono::system_clock::now();

    // 转换为time_t类型
    auto now_time_t = chrono::system_clock::to_time_t(now);

    // 转换为本地时间
    tm *local_time = std::localtime(&now_time_t);

    // 使用stringstream格式化输出
    stringstream ss;
    ss << put_time(local_time, "%Y-%m-%d %H:%M:%S");

    return ss.str();
}

// command handler
void help(int fd = 0, string str = "");
void chat(int, string);
void addfriend(int, string);
void creatgroup(int, string);
void addgroup(int, string);
void groupchat(int, string);
void loginout(int, string);

// 系统支持的客户端命令列表
unordered_map<string, string> commandMap = {
    {"help", "显示所有支持的命令,格式help"},
    {"chat", "一对一聊天,格式chat:friendid:msg"},
    {"addfriend", "加好友,格式addfriend:friendid"},
    {"creatgroup", "创建群聊,格式creatgroup:groupname:groupdesc"},
    {"addgroup", "加入群聊,格式addgroup:groupid"},
    {"groupchat", "群组聊天,格式groupchat:groupid:message"},
    {"loginout", "退出登录,格式quit"}};

// 注册系统支持的客户端命令处理
unordered_map<string, function<void(int, string)>> commandHandlerMap = {
    {"help", help},
    {"chat", chat},
    {"addfriend", addfriend},
    {"creatgroup", creatgroup},
    {"addgroup", addgroup},
    {"groupchat", groupchat},
    {"loginout", loginout}};

// 主聊天程序页面
void mainMenu(int cliendfd)
{
    help();
    char buffer[1024];
    while (isMainMenuRunning)
    {
        cin.getline(buffer, 1024);
        string commandbuf(buffer);
        string command;
        int idx = commandbuf.find(":");
        if (-1 == idx)
        {
            command = commandbuf;
        }
        else
        {
            command = commandbuf.substr(0, idx);
        }
        auto it = commandHandlerMap.find(command);
        if (it == commandHandlerMap.end())
        {
            cerr << "invailed input command!" << endl;
            continue;
        }

        // 调用相应命令的事件处理回调，mainMenu对修改封闭，添加新功能不需要修改函数
        it->second(cliendfd, commandbuf.substr(idx + 1, commandbuf.size() - idx)); // 调用命令处理方法
    }
}

// command handler
void help(int, string)
{
    cout << "show command list >>> " << endl;
    for (auto &p : commandMap)
    {
        cout << p.first << " : " << p.second << endl;
    }
    cout << endl;
}

void chat(int clientfd, string str)
{
    int idx = str.find(":");
    if (-1 == idx)
    {
        cerr << "chat command invaild!" << endl;
        return;
    }
    int friendid = atoi(str.substr(0, idx).c_str());
    string message = str.substr(idx + 1, str.size() - idx);

    json js;
    js["msgid"] = ONE_CHAT_MSG;
    js["id"] = g_currentUser.getId();
    js["name"] = g_currentUser.getName();
    js["to"] = friendid;
    js["msg"] = message;
    js["time"] = getCurrentTime();
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);

    if (-1 == len)
    {
        cerr << "send chat msg error ->" << buffer << endl;
    }
}
void addfriend(int clientfd, string str)
{
    int friendid = atoi(str.c_str());
    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = g_currentUser.getId();
    js["friendid"] = friendid;
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send addfriend msg error ->" << buffer << endl;
    }
}
void creatgroup(int clientfd, string str)
{
    int idx = str.find(":");
    if (-1 == idx)
    {
        cerr << "chat command invaild!" << endl;
        return;
    }
    string groupname = str.substr(0, idx);
    string groupdesc = str.substr(idx + 1, str.size() - idx);

    json js;
    js["msgid"] = CREAT_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    js["groupname"] = groupname;
    js["groupdesc"] = groupdesc;
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send creatgroup msg error ->" << buffer << endl;
    }
}
void addgroup(int clientfd, string str)
{
    int groupid = atoi(str.c_str());
    json js;
    js["msgid"] = ADD_GROUP_MSG;
    js["id"] = g_currentUser.getId();
    js["groupid"] = groupid;
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send addfriend msg error ->" << buffer << endl;
    }
}
void groupchat(int clientfd, string str)
{
    int idx = str.find(":");
    if (-1 == idx)
    {
        cerr << "groupchat command invaild!" << endl;
        return;
    }
    int groupid = atoi(str.substr(0, idx).c_str());
    string message = str.substr(idx + 1, str.size() - idx);

    json js;
    js["msgid"] = GROUP_CHAT_MSG;
    js["id"] = g_currentUser.getId();
    js["name"] = g_currentUser.getName();
    js["groupid"] = groupid;
    js["msg"] = message;
    js["time"] = getCurrentTime();
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send groupchat msg error ->" << buffer << endl;
    }
}
void loginout(int clientfd, string str)
{
    json js;
    js["msgid"] = LOGINOUT_MSG;
    js["id"] = g_currentUser.getId();
    string buffer = js.dump();

    int len = send(clientfd, buffer.c_str(), strlen(buffer.c_str()) + 1, 0);
    if (-1 == len)
    {
        cerr << "send loginout msg error ->" << buffer << endl;
    }
    else
    {
        isMainMenuRunning = false;
        g_currentUserFriendList.clear();
        g_currentUserGroupList.clear();
    }
}