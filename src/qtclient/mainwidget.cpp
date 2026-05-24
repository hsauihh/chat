/*
 * ============================================================================
 * MainWidget 实现 — 应用程序主控制器
 * ============================================================================
 *
 * 【架构角色】MainWidget 是 MVC 架构中的 Controller 层：
 *   Model  ← 消息数据（json 格式，由 ChatClient 接收）
 *   View   ← SidebarWidget + ContactWidget + ChatWidget + LoginWidget
 *   Controller ← MainWidget（连接 View 和 Model，处理消息分发）
 *
 * 【事件流向】
 *   用户操作 → Widget 信号 → MainWidget 槽 → 构造 JSON → ChatClient.send()
 *   服务端消息 → ChatClient 信号 → MainWidget 槽 → 解析 → Widget 方法
 */

#include "mainwidget.hpp"
#include "public.hpp"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDateTime>
#include <iostream>

using namespace std;

// ============================================================================
// 构造 / 析构
// ============================================================================

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent), _currentUserId(-1)
{
    setWindowTitle("Chat - Qt 客户端");
    resize(960, 640);

    /*
     * 创建顶层页面栈（登录页 ↔ 主页面）。
     *
     * 【Qt 概念】QStackedWidget 是"一摞卡片"容器，addWidget() 按顺序添加页面，
     * setCurrentWidget() / setCurrentIndex() 切换显示的页面。
     * 所有页面共享同一个内存空间，切换时不销毁隐藏的页面。
     */
    _pageStack = new QStackedWidget(this);
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(_pageStack);

    _loginWidget = new LoginWidget;
    _client = new ChatClient(this);

    setupLoginPage();
    switchToLoginPage();  // 启动时显示登录页

    /*
     * 连接网络层信号。
     * ChatClient 不关心消息内容，只管收发。MainWidget 负责解析和分发。
     */
    connect(_client, &ChatClient::connected, this, &MainWidget::handleConnected);
    connect(_client, &ChatClient::disconnected, this, &MainWidget::handleDisconnected);
    connect(_client, &ChatClient::messageReceived, this, &MainWidget::handleMessageReceived);

    // Lambda 连接：适合简单的一次性处理逻辑，无需单独写槽函数
    connect(_client, &ChatClient::errorOccurred, this, [](const QString &error) {
        QMessageBox::warning(nullptr, "连接错误", "网络错误: " + error);
    });
}

MainWidget::~MainWidget()
{
    /*
     * 【Qt 概念】Qt 父子树管理生命周期。所有 new 出来的子 Widget 都指定了
     * parent (this)，MainWidget 析构时 Qt 会自动 delete 它们。
     * 所以这里不需要写任何释放代码。
     */
}

// ============================================================================
// 网络连接
// ============================================================================

void MainWidget::connectToServer(const QString &ip, quint16 port)
{
    _serverIp = ip;
    _serverPort = port;
    _client->connectToServer(ip, port);
}

// ============================================================================
// 页面设置
// ============================================================================

/*
 * setupLoginPage — 设置登录页面（只需连接信号，UI 已在 loginwidget.ui 中定义）
 *
 * 这个函数在 setupMainPage 之前调用，因为登录页是首先显示的。
 * 主页面采用"懒初始化"（登录成功后才调用 setupMainPage），节省资源。
 */
void MainWidget::setupLoginPage()
{
    connect(_loginWidget, &LoginWidget::loginRequest, this, &MainWidget::handleLoginRequest);
    connect(_loginWidget, &LoginWidget::registerRequest, this, &MainWidget::handleRegisterRequest);
    _pageStack->addWidget(_loginWidget);  // index 0
}

/*
 * setupMainPage — 构建主页面布局
 *
 * 布局结构：
 *   ┌────┬───┬──────────────────────────────┐
 *   │ 侧 │ 分│  _rightPanel (QStackedWidget)  │
 *   │ 边 │ 隔│  ├─ [0] 联系人列表 + 聊天区    │
 *   │ 栏 │ 线│  └─ [1] 联系人管理页面         │
 *   └────┴───┴──────────────────────────────┘
 *
 * 只在登录成功时调用一次（switchToMainPage 中的懒初始化保护）。
 */
void MainWidget::setupMainPage()
{
    _mainPage = new QWidget;
    QHBoxLayout *layout = new QHBoxLayout(_mainPage);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // -- 左侧导航栏（60px 固定宽度）--
    _sidebarWidget = new SidebarWidget;
    _sidebarWidget->setFixedWidth(60);
    layout->addWidget(_sidebarWidget);

    // -- 分隔线 --
    QWidget *separator = new QWidget;
    separator->setFixedWidth(1);
    separator->setStyleSheet("background-color: #ddd;");
    layout->addWidget(separator);

    // -- 右侧面板（QStackedWidget 切换聊天/联系人管理）--
    _rightPanel = new QStackedWidget;

    // 页面 0: 聊天视图 = 联系人列表(30%) + 聊天区(70%)
    QWidget *chatView = new QWidget;
    QHBoxLayout *chatLayout = new QHBoxLayout(chatView);
    chatLayout->setContentsMargins(0, 0, 0, 0);
    chatLayout->setSpacing(0);

    _contactWidget = new ContactWidget;
    _contactWidget->setMinimumWidth(200);
    _chatWidget = new ChatWidget;

    // addWidget 的第二个参数是 stretch factor（拉伸因子）
    // 3:7 意味着聊天区占 70% 的空间
    chatLayout->addWidget(_contactWidget, 3);
    chatLayout->addWidget(_chatWidget, 7);

    _rightPanel->addWidget(chatView);  // index 0

    // 页面 1: 联系人管理视图
    _contactsManageWidget = new ContactsManageWidget;
    _rightPanel->addWidget(_contactsManageWidget);  // index 1

    // 右侧面板占用剩余的所有水平空间
    layout->addWidget(_rightPanel, 1);

    /*
     * ---- 信号/槽连接 ----
     *
     * 连接顺序和逻辑不相关，按模块组织便于阅读。
     * 每个 connect 对应一条"用户操作 → 系统响应"的路径。
     */

    // 侧边栏导航
    connect(_sidebarWidget, &SidebarWidget::chatNavClicked, this, &MainWidget::switchToChatView);
    connect(_sidebarWidget, &SidebarWidget::contactsNavClicked, this, &MainWidget::switchToContactsView);
    connect(_sidebarWidget, &SidebarWidget::logoutRequest, this, &MainWidget::handleLogout);
    connect(_sidebarWidget, &SidebarWidget::clearHistoryRequest, this, &MainWidget::handleClearHistory);

    // 联系人列表双击 → 打开聊天
    connect(_contactWidget, &ContactWidget::chatRequested, this, &MainWidget::handleContactChatRequest);
    connect(_contactWidget, &ContactWidget::groupChatRequested, this, &MainWidget::handleContactGroupChatRequest);

    // 聊天区发送消息 → 网络发送
    connect(_chatWidget, &ChatWidget::sendOneChat, this, &MainWidget::handleSendOneChat);
    connect(_chatWidget, &ChatWidget::sendGroupChat, this, &MainWidget::handleSendGroupChat);

    // 非当前聊天消息到达 → 联系人列表显示红点
    connect(_chatWidget, &ChatWidget::unreadMessageStored, this,
            [this](int targetId, bool isGroup) {
                _contactWidget->setItemUnread(targetId, isGroup, true);
            });

    // 联系人管理操作
    connect(_contactsManageWidget, &ContactsManageWidget::addFriendRequest, this, &MainWidget::handleAddFriend);
    connect(_contactsManageWidget, &ContactsManageWidget::createGroupRequest, this, &MainWidget::handleCreateGroup);
    connect(_contactsManageWidget, &ContactsManageWidget::addGroupRequest, this, &MainWidget::handleAddGroup);

    _pageStack->addWidget(_mainPage);  // index 1
}

// ============================================================================
// 页面切换
// ============================================================================

void MainWidget::switchToLoginPage()
{
    _pageStack->setCurrentWidget(_loginWidget);
}

void MainWidget::switchToMainPage()
{
    // 懒初始化：第一次调用时才构建主页面
    if (_pageStack->indexOf(_mainPage) < 0)
    {
        setupMainPage();
    }
    _pageStack->setCurrentWidget(_mainPage);
    _rightPanel->setCurrentIndex(0);  // 默认显示聊天视图
}

void MainWidget::switchToChatView()
{
    _rightPanel->setCurrentIndex(0);
}

void MainWidget::switchToContactsView()
{
    _rightPanel->setCurrentIndex(1);
}

// ============================================================================
// 登录/注册处理
// ============================================================================

/*
 * handleLoginRequest — 用户点击登录 → 构造登录 JSON → 发送到服务器
 *
 * 【消息格式】
 *   {"msgid": LOGIN_MSG(1), "id": <用户ID>, "password": "<密码>"}
 */
void MainWidget::handleLoginRequest(int id, const string &password)
{
    json js;
    js["msgid"] = LOGIN_MSG;
    js["id"] = id;
    js["password"] = password;
    _client->send(js);
}

/*
 * handleRegisterRequest — 用户点击注册 → 构造注册 JSON → 发送到服务器
 *
 * 【消息格式】
 *   {"msgid": REG_MSG(3), "name": "<用户名>", "password": "<密码>"}
 *
 * 服务器收到后返回 REG_MSG_ACK，包含新分配的 user ID。
 */
void MainWidget::handleRegisterRequest(const string &name, const string &password)
{
    json js;
    js["msgid"] = REG_MSG;
    js["name"] = name;
    js["password"] = password;
    _client->send(js);
}

// ============================================================================
// 网络事件处理
// ============================================================================

void MainWidget::handleConnected()
{
    cout << "connect success" << endl;
}

void MainWidget::handleDisconnected()
{
    QMessageBox::warning(this, "连接断开", "与服务器的连接已断开");
    switchToLoginPage();
}

/*
 * handleMessageReceived — 消息分发中心
 *
 * 所有服务端消息都经过这里。根据 msgid 字段路由到对应的处理函数。
 * 这是典型的"消息分发器"模式（类似后端开发中的路由表）。
 *
 * 如果要添加新的消息类型：
 *   1. 在 public.hpp 的 EnMsgType 枚举中添加新值
 *   2. 在本函数的 switch 中添加对应的 case
 *   3. 在类中新增对应的 handleXxxResponse 方法
 */
void MainWidget::handleMessageReceived(json js)
{
    int msgid = js["msgid"].get<int>();
    switch (msgid)
    {
    case LOGIN_MSG_ACK:
        handleLoginResponse(js);
        break;
    case REG_MSG_ACK:
        handleRegisterResponse(js);
        break;
    case ONE_CHAT_MSG:
        handleOneChatMessage(js);
        break;
    case GROUP_CHAT_MSG:
        handleGroupChatMessage(js);
        break;
    default:
        break;
    }
}

// ============================================================================
// 登录/注册响应处理
// ============================================================================

/*
 * handleLoginResponse — 处理服务端登录响应
 *
 * 登录成功后的操作序列：
 *   1. 保存当前用户 ID 和名称
 *   2. 解析好友列表和群组列表（服务端以 JSON 字符串数组形式发送）
 *   3. 切换到主页面
 *   4. 初始化侧边栏用户信息、聊天组件用户 ID、联系人列表数据
 *   5. 处理离线消息（存储到对应聊天的本地文件，不直接显示）
 */
void MainWidget::handleLoginResponse(json &js)
{
    int errno_ = js["errno"].get<int>();
    if (errno_ == 0)
    {
        _currentUserId = js["id"].get<int>();
        _currentUserName = js["name"].get<string>();

        // -- 解析好友列表 --
        // 每个好友是一个 JSON 字符串，包含了 id、name、state（在线状态）
        vector<User> friends;
        if (js.contains("friends"))
        {
            vector<string> friendStrs = js["friends"];
            for (string &str : friendStrs)
            {
                json fjs = json::parse(str);
                User user;
                user.setId(fjs["id"].get<int>());
                user.setName(fjs["name"].get<string>());
                user.setState(fjs["state"].get<string>());
                friends.push_back(user);
            }
        }

        // -- 解析群组列表 --
        // 每个群组包含 id、name、desc 以及 users（群成员列表）
        vector<Group> groups;
        if (js.contains("groups"))
        {
            vector<string> groupStrs = js["groups"];
            for (string &str : groupStrs)
            {
                json gjs = json::parse(str);
                Group group;
                group.setId(gjs["id"].get<int>());
                group.setName(gjs["groupname"].get<string>());
                group.setDesc(gjs["groupdesc"].get<string>());
                if (gjs.contains("users"))
                {
                    vector<string> userStrs = gjs["users"];
                    for (string &ustr : userStrs)
                    {
                        json ujs = json::parse(ustr);
                        GroupUser guser;
                        guser.setId(ujs["id"].get<int>());
                        guser.setName(ujs["name"].get<string>());
                        guser.setState(ujs["state"].get<string>());
                        guser.setRole(ujs["role"].get<string>());
                        group.getUsers().push_back(guser);
                    }
                }
                groups.push_back(group);
            }
        }

        // -- 切换到主页面并初始化数据 --
        switchToMainPage();
        _sidebarWidget->setUserInfo(_currentUserId, QString::fromStdString(_currentUserName));
        _chatWidget->setCurrentUserId(_currentUserId);
        _contactWidget->updateFriendList(friends);
        _contactWidget->updateGroupList(groups);

        // -- 离线消息：按类型路由到对应聊天的本地存储 --
        // storeMessage 会按 msgid 自动分配到正确的 "user_xxx" 或 "group_xxx"
        // 用户点击对应联系人时，loadMessagesForTarget 会自动加载显示
        if (js.contains("offlinemsg"))
        {
            vector<string> offlineMsgs = js["offlinemsg"];
            for (string &str : offlineMsgs)
            {
                json msgJs = json::parse(str);
                _chatWidget->storeMessage(msgJs);
            }
        }
    }
    else
    {
        QString errmsg = QString::fromStdString(js["errmsg"].get<string>());
        QMessageBox::warning(this, "登录失败", errmsg);
    }
}

void MainWidget::handleRegisterResponse(json &js)
{
    int errno_ = js["errno"].get<int>();
    if (errno_ == 0)
    {
        int id = js["id"].get<int>();
        QMessageBox::information(this, "注册成功",
                                 QString("注册成功! 您的用户 ID 是: %1\n请牢记此 ID，用于登录。").arg(id));
    }
    else
    {
        QMessageBox::warning(this, "注册失败", "注册失败，请重试");
    }
}

// ============================================================================
// 聊天消息处理
// ============================================================================

/*
 * handleOneChatMessage — 收到一对一聊天消息
 * 直接交给 ChatWidget::displayMessage，内部会判断是否属于当前聊天对象
 */
void MainWidget::handleOneChatMessage(json &js)
{
    _chatWidget->displayMessage(js);
}

void MainWidget::handleGroupChatMessage(json &js)
{
    _chatWidget->displayMessage(js);
}

/*
 * handleSendOneChat — 发送一对一消息
 *
 * 构造的 JSON 格式：
 *   {"msgid":5, "id":<当前用户ID>, "name":"<当前用户名>",
 *    "to":<接收者ID>, "msg":"<消息内容>", "time":"<发送时间>"}
 */
void MainWidget::handleSendOneChat(int toId, const string &msg)
{
    json js;
    js["msgid"] = ONE_CHAT_MSG;
    js["id"] = _currentUserId;
    js["name"] = _currentUserName;
    js["to"] = toId;
    js["msg"] = msg;
    js["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    _client->send(js);
}

/*
 * handleSendGroupChat — 发送群聊消息
 *
 * 构造的 JSON 格式：
 *   {"msgid":8, "id":<当前用户ID>, "name":"<当前用户名>",
 *    "groupid":<群组ID>, "msg":"<消息内容>", "time":"<发送时间>"}
 */
void MainWidget::handleSendGroupChat(int groupId, const string &msg)
{
    json js;
    js["msgid"] = GROUP_CHAT_MSG;
    js["id"] = _currentUserId;
    js["name"] = _currentUserName;
    js["groupid"] = groupId;
    js["msg"] = msg;
    js["time"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss").toStdString();
    _client->send(js);
}

// ============================================================================
// 联系人 / 群组操作
// ============================================================================

void MainWidget::handleContactChatRequest(int userId, const QString &name)
{
    _chatWidget->setChatTarget(userId, name, false);
    _contactWidget->setItemUnread(userId, false, false);  // 清除红点
    switchToChatView();
}

void MainWidget::handleContactGroupChatRequest(int groupId, const QString &name)
{
    _chatWidget->setChatTarget(groupId, name, true);
    _contactWidget->setItemUnread(groupId, true, false);   // 清除红点
    switchToChatView();
}

void MainWidget::handleAddFriend(int friendId)
{
    json js;
    js["msgid"] = ADD_FRIEND_MSG;
    js["id"] = _currentUserId;
    js["friendid"] = friendId;
    _client->send(js);
    QMessageBox::information(this, "添加好友", QString("已发送好友请求给用户 %1").arg(friendId));
}

void MainWidget::handleCreateGroup(const string &groupName, const string &groupDesc)
{
    json js;
    js["msgid"] = CREAT_GROUP_MSG;
    js["id"] = _currentUserId;
    js["groupname"] = groupName;
    js["groupdesc"] = groupDesc;
    _client->send(js);
    QMessageBox::information(this, "创建群组",
                             QString("已创建群组: %1").arg(QString::fromStdString(groupName)));
}

void MainWidget::handleAddGroup(int groupId)
{
    json js;
    js["msgid"] = ADD_GROUP_MSG;
    js["id"] = _currentUserId;
    js["groupid"] = groupId;
    _client->send(js);
    QMessageBox::information(this, "加入群组", QString("已加入群组 %1").arg(groupId));
}

// ============================================================================
// 用户操作
// ============================================================================

/*
 * handleClearHistory — 清理当前聊天记录
 *
 * 使用 QMessageBox::question 弹出确认对话框，防止误操作。
 * 用户点击"是"后才真正清理。
 */
void MainWidget::handleClearHistory()
{
    /*
     * QMessageBox::question 创建一个模态对话框，有"是"/"否"两个按钮。
     * 返回值 QMessageBox::StandardButton 表示用户点击了哪个按钮。
     */
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "清理聊天记录",
        "确定要清理当前聊天记录吗？\n此操作不可撤销。",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);     // 默认选中"否"，防止误操作
    if (reply == QMessageBox::Yes)
    {
        _chatWidget->clearCurrentChat();
    }
}

/*
 * handleLogout — 退出登录
 *
 * 流程：
 *   1. 发送注销消息给服务器
 *   2. 清空本地用户状态
 *   3. 切换回登录页面
 */
void MainWidget::handleLogout()
{
    json js;
    js["msgid"] = LOGINOUT_MSG;
    js["id"] = _currentUserId;
    _client->send(js);

    _currentUserId = -1;
    _currentUserName.clear();
    switchToLoginPage();
}
