#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <map>
#include <vector>
#include "json.hpp"
#include "messagebubble.hpp"

using namespace std;
using json = nlohmann::json;

namespace Ui {
    class ChatWidget;
}

/*
 * ============================================================================
 * ChatWidget — 聊天区域组件（消息气泡 + 输入框 + 本地持久化）
 * ============================================================================
 *
 * 【功能】
 *   1. 显示聊天消息气泡（自己的靠右绿色，对方的靠左白色）
 *   2. 提供消息输入框和发送按钮
 *   3. 管理当前聊天对象（一对一用户或群组）
 *   4. 本地持久化：消息自动保存到 JSON 文件，切换聊天不丢失
 *
 * 【Qt 概念】本类同时使用了"代码构建 UI"（setChatTarget 中设置 label 文本）
 * 和"UI 文件构建"（chatwidget.ui 中定义的 QListWidget、QPlainTextEdit 等）。
 *
 * 【本地存储机制】
 *   消息同时存储在两个位置：
 *   1. 内存 map: _messageStore["user_123"] = [msg1, msg2, ...]
 *   2. 磁盘文件: ~/.local/share/<AppName>/messages/user_123.json
 *   切换聊天时自动存档当前聊天 → 加载目标聊天 → 恢复消息
 *
 * 【消息路由机制】
 *   收到的消息通过 displayMessage() 处理：
 *   - 属于当前聊天对象 → 渲染气泡到界面 + 保存
 *   - 不属于当前聊天对象 → 静默存入对应聊天的存储 + 发红点通知
 */
class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = nullptr);
    ~ChatWidget();

    // ---- 公开方法 ----
    // 显示消息气泡到界面（内部会判断消息是否属于当前聊天）
    void displayMessage(const json &js);
    // 静默存储消息到本地（不渲染 UI），用于离线消息和路由到非当前聊天的消息
    void storeMessage(const json &js);
    // 清理当前聊天记录：清空 UI + 内存 map + 磁盘文件
    void clearCurrentChat();
    // 设置当前聊天对象（切换聊天时调用）
    void setChatTarget(int targetId, const QString &targetName, bool isGroup = false);
    // 获取当前聊天对象 ID
    int getTargetId() const { return _targetId; }
    // 是否为群聊模式
    bool isGroupChat() const { return _isGroupChat; }
    // 设置顶部在线状态文本
    void setOnlineStatus(const QString &status);
    // 设置当前登录用户 ID（区分自己/他人的消息气泡颜色）
    void setCurrentUserId(int userId) { _currentUserId = userId; }

signals:
    // ---- 信号（向 MainWidget 传递用户操作）----
    // 【Qt 概念】"信号" (signal) 在头文件中声明，由 moc 自动生成实现。
    // 信号函数不需要自己写函数体，只需要在 .hpp 中声明即可。
    // 通过 emit 关键字来触发信号：emit sendOneChat(id, msg);

    void sendOneChat(int toId, const string &msg);       // 用户点击发送（一对一）
    void sendGroupChat(int groupId, const string &msg);  // 用户点击发送（群聊）
    void unreadMessageStored(int targetId, bool isGroup); // 消息被存入非当前聊天

private slots:
    // 【Qt 概念】private slots: 只能在类内部被调用的槽。常用作按钮点击的内部响应。
    void onSendClicked();  // 发送按钮点击 → 构造 JSON → emit 信号给 MainWidget

private:
    // ---- 消息存储相关方法 ----
    string makeChatKey(int id, bool isGroup) const;
    void saveCurrentMessages();          // 将当前聊天的消息写入 JSON 文件
    void loadMessagesForTarget();        // 从文件加载目标聊天的历史消息
    void appendToStore(const json &js);  // 追加一条消息到内存 map
    void persistToFile(const string &key); // 将内存中对应 key 的消息写入磁盘
    void loadFromFile(const string &key);  // 从磁盘读取对应 key 的消息
    QString storageDir() const;          // 获取持久化存储目录路径

    // ---- 成员变量 ----
    Ui::ChatWidget *ui;          // UI 文件自动生成的类（ui_chatwidget.h）
    int _targetId;               // 当前聊天对象 ID
    int _currentUserId = -1;     // 当前登录用户 ID（-1 = 未登录）
    bool _isGroupChat;           // 当前是否为群聊模式

    // 消息存储：key 格式为 "user_123" 或 "group_456"
    map<string, vector<json>> _messageStore;
};

#endif
