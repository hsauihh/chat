/*
 * ============================================================================
 * ChatWidget 实现 — 聊天区域 + 消息持久化
 * ============================================================================
 *
 * 【核心流程】
 *   ┌─────────────┐     ┌──────────────┐     ┌─────────────┐
 *   │ 用户输入消息 │ ──→ │ onSendClicked│ ──→ │ emit 信号    │
 *   └─────────────┘     └──────────────┘     │ + 本地显示   │
 *                                             │ + 本地存储   │
 *                                             └─────────────┘
 *   ┌─────────────┐     ┌──────────────┐     ┌─────────────┐
 *   │ 服务端消息   │ ──→ │displayMessage│ ──→ │ 路由判断     │
 *   └─────────────┘     └──────────────┘     │ 渲染或存储    │
 *                                             └─────────────┘
 */

#include "chatwidget.hpp"
#include "public.hpp"
#include "ui_chatwidget.h"
#include <QPushButton>
#include <QPlainTextEdit>
#include <QLabel>
#include <QListWidget>
#include <QDateTime>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <fstream>
#include <iostream>

using namespace std;

// ============================================================================
// 构造 / 析构
// ============================================================================

ChatWidget::ChatWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ChatWidget), _targetId(-1), _isGroupChat(false)
{
    ui->setupUi(this);

    /*
     * 输入法设置：
     * WA_InputMethodEnabled → 显式启用输入法支持（中文等非 ASCII 输入依赖此属性）
     * ImhNone → 不设置任何输入法提示（让输入法使用默认行为）
     */
    ui->inputEdit->setAttribute(Qt::WA_InputMethodEnabled, true);
    ui->inputEdit->setInputMethodHints(Qt::ImhNone);

    // 发送按钮 → 发射信号（实际网络发送由 MainWidget 处理）
    connect(ui->sendBtn, &QPushButton::clicked, this, &ChatWidget::onSendClicked);
}

ChatWidget::~ChatWidget()
{
    saveCurrentMessages();  // 析构前保存当前聊天消息到磁盘
    delete ui;
}

// ============================================================================
// 存储基础设施
// ============================================================================

/*
 * makeChatKey — 构造消息存储的键
 * 一对一聊天: "user_<用户ID>"，群聊: "group_<群组ID>"
 */
string ChatWidget::makeChatKey(int id, bool isGroup) const
{
    return (isGroup ? "group_" : "user_") + to_string(id);
}

/*
 * storageDir — 获取消息存储目录
 * Linux: ~/.local/share/<AppName>/messages/
 * Windows: C:/Users/<用户名>/AppData/Local/<AppName>/messages/
 *
 * QStandardPaths 是 Qt 提供的跨平台标准路径 API，不需要手动判断操作系统。
 */
QString ChatWidget::storageDir() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/messages";
}

// ============================================================================
// 文件持久化（磁盘 I/O）
// ============================================================================

void ChatWidget::persistToFile(const string &key)
{
    QString dir = storageDir();
    QDir().mkpath(dir);  // 确保目录存在（递归创建）
    QString path = dir + "/" + QString::fromStdString(key) + ".json";

    // 构造 JSON 数组
    json arr = json::array();
    for (const auto &msg : _messageStore[key])
    {
        arr.push_back(msg);
    }

    // 写入文件（覆盖模式）。dump(2) 表示 2 空格缩进的格式化 JSON
    ofstream ofs(path.toStdString());
    if (ofs.is_open())
    {
        ofs << arr.dump(2);
        ofs.close();
    }
}

void ChatWidget::loadFromFile(const string &key)
{
    QString path = storageDir() + "/" + QString::fromStdString(key) + ".json";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;

    QByteArray data = file.readAll();
    file.close();

    // 如果文件损坏或为空，静默处理（返回空消息列表）
    try
    {
        json arr = json::parse(data.toStdString());
        if (arr.is_array())
        {
            vector<json> msgs;
            for (const auto &msg : arr)
            {
                msgs.push_back(msg);
            }
            _messageStore[key] = msgs;
        }
    }
    catch (...) {}
}

// ============================================================================
// 内存存储操作
// ============================================================================

void ChatWidget::appendToStore(const json &js)
{
    if (_targetId < 0) return;
    string key = makeChatKey(_targetId, _isGroupChat);
    _messageStore[key].push_back(js);
}

void ChatWidget::saveCurrentMessages()
{
    if (_targetId < 0) return;
    string key = makeChatKey(_targetId, _isGroupChat);

    // 消息已通过 appendToStore 存在内存 map 中，这里只需要落盘
    int count = ui->messageList->count();
    if (count == 0) return;

    persistToFile(key);
}

void ChatWidget::loadMessagesForTarget()
{
    if (_targetId < 0) return;

    string key = makeChatKey(_targetId, _isGroupChat);

    // 优先从文件加载（磁盘优先，覆盖内存）
    loadFromFile(key);

    auto it = _messageStore.find(key);
    if (it == _messageStore.end()) return;

    // 遍历消息列表，为每条消息创建 QListWidgetItem + MessageBubble
    for (const auto &js : it->second)
    {
        QString senderName, message, timeStr;
        if (js.contains("name"))
            senderName = QString::fromStdString(js["name"].get<string>());
        if (js.contains("msg"))
            message = QString::fromStdString(js["msg"].get<string>());
        if (js.contains("time"))
            timeStr = QString::fromStdString(js["time"].get<string>());

        int senderId = js.contains("id") ? js["id"].get<int>() : -1;
        bool isSelf = (senderId == _currentUserId);

        auto *bubble = new MessageBubble(senderName, message, timeStr, isSelf);

        // 【Qt 概念】QListWidgetItem 包装 MessageBubble widget
        // setSizeHint 告诉 QListWidget 这个 item 的高度 = bubble 的高度
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(bubble->sizeHint());
        ui->messageList->addItem(item);
        ui->messageList->setItemWidget(item, bubble);
    }

    ui->messageList->scrollToBottom();
}

// ============================================================================
// 聊天对象切换
// ============================================================================

void ChatWidget::setChatTarget(int targetId, const QString &targetName, bool isGroup)
{
    // 0. 保存当前聊天的消息（不丢失切换前的消息）
    saveCurrentMessages();

    // 1. 更新目标
    _targetId = targetId;
    _isGroupChat = isGroup;

    // 2. 更新顶部栏
    if (isGroup)
        ui->contactNameLabel->setText("群聊: " + targetName);
    else
        ui->contactNameLabel->setText(targetName);

    ui->statusLabel->clear();

    // 3. 清空当前显示的消息列表
    ui->messageList->clear();

    // 4. 加载目标聊天的历史消息
    loadMessagesForTarget();
}

void ChatWidget::setOnlineStatus(const QString &status)
{
    ui->statusLabel->setText(status);
}

// ============================================================================
// 消息显示与路由
// ============================================================================

/*
 * displayMessage — 显示一条消息到聊天界面
 *
 * 【路由逻辑】
 *   1. 提取 msgid，判断消息类型（ONE_CHAT_MSG 或 GROUP_CHAT_MSG）
 *   2. 计算消息所属的聊天对象：
 *      - 单聊：如果我是发送者 → 聊天对象是 "to"；否则聊天对象是发送者
 *      - 群聊：聊天对象是 "groupid"
 *   3. 如果聊天对象 == 当前显示的聊天 → 渲染气泡
 *      如果聊天对象 != 当前显示的聊天 → 调用 storeMessage 静默存储 + 发红点
 */
void ChatWidget::displayMessage(const json &js)
{
    int msgid = js["msgid"].get<int>();

    // ---- 路由判断 ----
    if (msgid == ONE_CHAT_MSG)
    {
        int senderId = js["id"].get<int>();
        int toId = js.contains("to") ? js["to"].get<int>() : -1;
        int peerId = (senderId == _currentUserId) ? toId : senderId;
        if (_isGroupChat || _targetId != peerId)
        {
            storeMessage(js);
            return;
        }
    }
    else if (msgid == GROUP_CHAT_MSG)
    {
        int groupId = js["groupid"].get<int>();
        if (!_isGroupChat || _targetId != groupId)
        {
            storeMessage(js);
            return;
        }
    }

    // ---- 渲染消息气泡 ----
    QString senderName, message, timeStr;
    if (js.contains("name"))
        senderName = QString::fromStdString(js["name"].get<string>());
    if (js.contains("msg"))
        message = QString::fromStdString(js["msg"].get<string>());
    if (js.contains("time"))
        timeStr = QString::fromStdString(js["time"].get<string>());

    int senderId = js.contains("id") ? js["id"].get<int>() : -1;
    bool isSelf = (senderId == _currentUserId);

    auto *bubble = new MessageBubble(senderName, message, timeStr, isSelf);

    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(bubble->sizeHint());
    ui->messageList->addItem(item);
    ui->messageList->setItemWidget(item, bubble);
    ui->messageList->scrollToBottom();

    // 当前活动的消息才会走到这里，追加到内存存储
    appendToStore(js);
}

/*
 * storeMessage — 静默存储消息（不渲染 UI）
 * 用于：离线消息、不属于当前聊天对象的消息
 */
void ChatWidget::storeMessage(const json &js)
{
    int msgid = js["msgid"].get<int>();
    int targetId = -1;
    bool isGroup = false;

    if (msgid == ONE_CHAT_MSG)
    {
        targetId = js["id"].get<int>();  // 发送者 ID 就是聊天对象
        isGroup = false;
    }
    else if (msgid == GROUP_CHAT_MSG)
    {
        targetId = js["groupid"].get<int>();
        isGroup = true;
    }
    else
    {
        return;  // 不支持的消息类型，忽略
    }

    string key = makeChatKey(targetId, isGroup);
    _messageStore[key].push_back(js);
    persistToFile(key);

    // 通知 MainWidget → ContactWidget 显示红点
    emit unreadMessageStored(targetId, isGroup);
}

// ============================================================================
// 清理聊天记录
// ============================================================================

void ChatWidget::clearCurrentChat()
{
    if (_targetId < 0) return;

    string key = makeChatKey(_targetId, _isGroupChat);

    // 三处清理：UI、内存、磁盘
    ui->messageList->clear();
    _messageStore.erase(key);

    QString path = storageDir() + "/" + QString::fromStdString(key) + ".json";
    QFile::remove(path);
}

// ============================================================================
// 发送消息
// ============================================================================

void ChatWidget::onSendClicked()
{
    QString msg = ui->inputEdit->toPlainText().trimmed();
    if (msg.isEmpty() || _targetId < 0) return;

    string message = msg.toStdString();

    // 1. 发射信号 → MainWidget 组装 JSON → ChatClient 发送到服务器
    if (_isGroupChat)
        emit sendGroupChat(_targetId, message);
    else
        emit sendOneChat(_targetId, message);

    // 2. 本地立即显示自己的气泡（乐观 UI：不等待服务器确认）
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    auto *bubble = new MessageBubble("", msg, timeStr, true);

    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(bubble->sizeHint());
    ui->messageList->addItem(item);
    ui->messageList->setItemWidget(item, bubble);
    ui->messageList->scrollToBottom();

    ui->inputEdit->clear();
    ui->inputEdit->setFocus();

    // 3. 持久化自己发送的消息
    json js;
    js["id"] = _currentUserId;
    js["msg"] = message;
    js["time"] = timeStr.toStdString();
    appendToStore(js);
}
