#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QStackedWidget>
#include "chatclient.hpp"
#include "loginwidget.hpp"
#include "sidebarwidget.hpp"
#include "contactwidget.hpp"
#include "chatwidget.hpp"
#include "contactsmanagewidget.hpp"
#include "json.hpp"
#include "user.hpp"
#include "group.hpp"

using namespace std;
using json = nlohmann::json;

/*
 * ============================================================================
 * MainWidget — 应用程序主控制器 (Controller)
 * ============================================================================
 *
 * 【Qt 概念】QWidget 是所有界面组件的基类。MainWidget 继承 QWidget，是整个
 * 客户端窗口的"大脑"，负责：
 *   1. 管理页面切换（登录页 ↔ 主页面）
 *   2. 连接各个子组件（侧边栏、联系人列表、聊天区）的信号和槽
 *   3. 处理服务端消息的分发（根据 msgid 路由到不同处理方法）
 *   4. 管理当前用户状态（ID、名称）
 *
 * 【Qt 概念】QStackedWidget 是一个"卡片堆"容器，同一时间只显示一张卡片。
 * 本应用有两层 QStackedWidget：
 *   - _pageStack：整个窗口级别，切换登录页 / 主页面
 *   - _rightPanel：主页面右侧区域，切换聊天视图 / 联系人管理视图
 *
 * 【架构】布局结构如下：
 *   MainWidget (_pageStack)
 *   ├── 页面0: LoginWidget (登录/注册)
 *   └── 页面1: _mainPage (主页面)
 *        └── HBoxLayout
 *             ├── SidebarWidget (60px 左侧栏)
 *             ├── 分隔线 (1px)
 *             └── _rightPanel (QStackedWidget)
 *                  ├── 页面0: ContactWidget + ChatWidget (聊天视图)
 *                  └── 页面1: ContactsManageWidget (联系人管理)
 */
class MainWidget : public QWidget
{
    // 【Qt 概念】Q_OBJECT 宏是 Qt 信号/槽机制的必要条件。任何定义了 signals:
    // 或 slots: 的类都必须写这个宏。构建时 moc (Meta-Object Compiler) 会扫描
    // 这个宏所在的头文件，生成元对象代码。忘记写 Q_OBJECT 会导致连接信号槽时
    // 编译报错 "undefined reference to vtable"。
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    // 连接到聊天服务器（在 main() 中调用）
    void connectToServer(const QString &ip, quint16 port);

private slots:
    // ---- slots: 都是被 Qt 信号触发的回调函数 ----
    // 【Qt 概念】"槽" (slot) 是响应信号的普通 C++ 成员函数。当某个信号被 emit
    // 时，所有连接到此信号的槽函数会被自动调用。槽可以有参数，参数由信号传入。

    void handleLoginRequest(int id, const string &password);
    void handleRegisterRequest(const string &name, const string &password);
    void handleMessageReceived(json js);
    void handleConnected();
    void handleDisconnected();

    // 聊天相关
    void handleSendOneChat(int toId, const string &msg);
    void handleSendGroupChat(int groupId, const string &msg);
    void handleContactChatRequest(int userId, const QString &name);
    void handleContactGroupChatRequest(int groupId, const QString &name);

    // 好友/群组操作
    void handleAddFriend(int friendId);
    void handleCreateGroup(const string &groupName, const string &groupDesc);
    void handleAddGroup(int groupId);
    void handleLogout();
    void handleClearHistory();

    // 导航切换
    void switchToChatView();
    void switchToContactsView();

private:
    void setupLoginPage();
    void setupMainPage();
    void switchToMainPage();
    void switchToLoginPage();
    void handleLoginResponse(json &js);
    void handleRegisterResponse(json &js);
    void handleOneChatMessage(json &js);
    void handleGroupChatMessage(json &js);

    // ---- 成员变量 ----
    // 【Qt 概念】Qt 使用父子树管理对象生命周期。指定了 parent 的子对象会在
    // 父对象析构时自动被 delete，无需手动释放。

    QStackedWidget *_pageStack;       // 顶层页面栈（登录/主页切换）

    // 页面
    LoginWidget *_loginWidget;        // 登录/注册页面
    QWidget *_mainPage;               // 主页面容器

    // 主页面布局：左侧栏 + 右侧面板
    SidebarWidget *_sidebarWidget;    // 60px 导航侧边栏
    QStackedWidget *_rightPanel;      // 右侧面板栈（聊天/联系人管理）
    ContactWidget *_contactWidget;    // 联系人列表（好友+群组）
    ChatWidget *_chatWidget;          // 聊天区域（消息气泡+输入框）
    ContactsManageWidget *_contactsManageWidget; // 联系人管理页面

    // 网络客户端
    ChatClient *_client;              // TCP 网络层，封装 QTcpSocket

    // 当前连接信息
    QString _serverIp;
    quint16 _serverPort;

    // 当前用户信息
    int _currentUserId;               // 登录用户 ID
    string _currentUserName;          // 登录用户名称
};

#endif
