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
#include <QFileDialog>
#include <QMessageBox>
#include <QImageReader>
#include <QFileInfo>
#include <QByteArray>
#include <QCryptographicHash>
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

    // 图片按钮（程序化创建，避免修改 .ui 文件）
    QPushButton *imageBtn = new QPushButton("图片");
    imageBtn->setMinimumSize(32, 32);
    imageBtn->setMaximumSize(32, 32);
    imageBtn->setFlat(true);
    imageBtn->setStyleSheet("font-size: 12px; color: #333;");
    connect(imageBtn, &QPushButton::clicked, this, &ChatWidget::onImageClicked);
    // 插入到 inputEdit 之前（index 2 = emojiBtn, attachBtn, 新按钮, inputEdit, sendBtn）
    ui->bottomBarLayout->insertWidget(2, imageBtn);
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

/*
 * assetsDir — 获取图片资源存储目录（按聊天对象分目录）
 *   <storageDir>/assets/user_<id>/  或  <storageDir>/assets/group_<id>/
 */
QString ChatWidget::assetsDir() const
{
    if (_targetId < 0) return QString();
    string key = makeChatKey(_targetId, _isGroupChat);
    return storageDir() + "/assets/" + QString::fromStdString(key);
}

QString ChatWidget::assetsDirFor(int targetId, bool isGroup) const
{
    string key = makeChatKey(targetId, isGroup);
    return storageDir() + "/assets/" + QString::fromStdString(key);
}

/*
 * saveImageToAssets — 将 Base64 图片解码保存到 assets 目录
 * 返回保存后的本地路径，失败返回空字符串
 */
QString ChatWidget::saveImageToAssets(const string &imageBase64, const string &filename)
{
    return saveImageToAssets(imageBase64, filename, _targetId, _isGroupChat);
}

QString ChatWidget::saveImageToAssets(const string &imageBase64, const string &filename,
                                       int targetId, bool isGroup)
{
    if (targetId < 0) return QString();
    QString dir = assetsDirFor(targetId, isGroup);
    QDir().mkpath(dir);

    QByteArray base64Data = QByteArray::fromStdString(imageBase64);
    QByteArray imgData = QByteArray::fromBase64(base64Data);
    if (imgData.isEmpty()) return QString();

    QString safeName = QString("%1_%2_%3")
        .arg(QDateTime::currentMSecsSinceEpoch())
        .arg(QString(QCryptographicHash::hash(imgData, QCryptographicHash::Md5).toHex().left(8)))
        .arg(QString::fromStdString(filename));
    QString path = dir + "/" + safeName;

    QFile file(path);
    if (file.open(QIODevice::WriteOnly))
    {
        file.write(imgData);
        file.close();
        return path;
    }
    return QString();
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
        QString senderName, message, timeStr, imagePath;
        if (js.contains("name"))
            senderName = QString::fromStdString(js["name"].get<string>());
        if (js.contains("msg"))
            message = QString::fromStdString(js["msg"].get<string>());
        if (js.contains("time"))
            timeStr = QString::fromStdString(js["time"].get<string>());
        if (js.contains("localpath"))
            imagePath = QString::fromStdString(js["localpath"].get<string>());

        int senderId = js.contains("id") ? js["id"].get<int>() : -1;
        bool isSelf = (senderId == _currentUserId);

        auto *bubble = new MessageBubble(senderName, message, timeStr, isSelf, imagePath);

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
    else if (msgid == IMAGE_CHAT_MSG)
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
    else if (msgid == GROUP_IMAGE_CHAT_MSG)
    {
        int groupId = js["groupid"].get<int>();
        if (!_isGroupChat || _targetId != groupId)
        {
            storeMessage(js);
            return;
        }
    }

    // ---- 渲染消息气泡 ----
    QString senderName, message, timeStr, imagePath;
    if (js.contains("name"))
        senderName = QString::fromStdString(js["name"].get<string>());
    if (js.contains("msg"))
        message = QString::fromStdString(js["msg"].get<string>());
    if (js.contains("time"))
        timeStr = QString::fromStdString(js["time"].get<string>());

    int senderId = js.contains("id") ? js["id"].get<int>() : -1;
    bool isSelf = (senderId == _currentUserId);

    // 构造用于本地存储的 JSON（图片消息去掉 Base64，换为 localpath）
    json storeJson = js;

    // 图片消息：将 Base64 保存到本地，用 localpath 替换 image 字段
    if ((msgid == IMAGE_CHAT_MSG || msgid == GROUP_IMAGE_CHAT_MSG) && js.contains("image"))
    {
        string filename = js.contains("filename") ? js["filename"].get<string>() : "image.png";
        string base64 = js["image"].get<string>();
        imagePath = saveImageToAssets(base64, filename);

        storeJson.erase("image");
        if (!imagePath.isEmpty())
            storeJson["localpath"] = imagePath.toStdString();

        auto *bubble = new MessageBubble(senderName, "", timeStr, isSelf, imagePath);

        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(bubble->sizeHint());
        ui->messageList->addItem(item);
        ui->messageList->setItemWidget(item, bubble);
    }
    else
    {
        // 文本消息
        auto *bubble = new MessageBubble(senderName, message, timeStr, isSelf);

        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(bubble->sizeHint());
        ui->messageList->addItem(item);
        ui->messageList->setItemWidget(item, bubble);
    }

    ui->messageList->scrollToBottom();

    // 当前活动的消息才会走到这里，追加到内存存储（使用替换 localpath 后的 JSON）
    appendToStore(storeJson);
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

    if (msgid == ONE_CHAT_MSG || msgid == IMAGE_CHAT_MSG)
    {
        targetId = js["id"].get<int>();  // 发送者 ID 就是聊天对象
        isGroup = false;
    }
    else if (msgid == GROUP_CHAT_MSG || msgid == GROUP_IMAGE_CHAT_MSG)
    {
        targetId = js["groupid"].get<int>();
        isGroup = true;
    }
    else
    {
        return;  // 不支持的消息类型，忽略
    }

    // 图片消息：将 Base64 保存到本地 assets，替换为 localpath
    json storeJs = js;
    if ((msgid == IMAGE_CHAT_MSG || msgid == GROUP_IMAGE_CHAT_MSG) && js.contains("image"))
    {
        string filename = js.contains("filename") ? js["filename"].get<string>() : "image.png";
        string base64 = js["image"].get<string>();
        QString localPath = saveImageToAssets(base64, filename, targetId, isGroup);
        storeJs.erase("image");
        if (!localPath.isEmpty())
            storeJs["localpath"] = localPath.toStdString();
    }

    string key = makeChatKey(targetId, isGroup);
    _messageStore[key].push_back(storeJs);
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

    // 清理图片 assets 目录
    QString assetPath = storageDir() + "/assets/" + QString::fromStdString(key);
    QDir assetDir(assetPath);
    if (assetDir.exists())
    {
        assetDir.removeRecursively();
    }
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

// ============================================================================
// 发送图片
// ============================================================================

void ChatWidget::onImageClicked()
{
    if (_targetId < 0) return;

    // 1. 打开文件选择对话框，过滤图片格式
    QStringList supported;
    supported << "image/png" << "image/jpeg" << "image/webp";
    QString filter = "图片文件 (*.png *.jpg *.jpeg *.webp)";
    QString filePath = QFileDialog::getOpenFileName(this, "选择图片", QString(), filter);
    if (filePath.isEmpty()) return;

    // 2. 验证文件格式
    QImageReader reader(filePath);
    QString mimeType;
    QString fmt = reader.format().toLower();
    if (fmt == "png")       mimeType = "image/png";
    else if (fmt == "jpg" || fmt == "jpeg") mimeType = "image/jpeg";
    else if (fmt == "webp") mimeType = "image/webp";
    else
    {
        QMessageBox::warning(this, "格式不支持", "仅支持 PNG、JPG、WebP 格式的图片。");
        return;
    }

    // 3. 验证文件大小（限制 1 MB）
    QFile file(filePath);
    qint64 filesize = file.size();
    const qint64 maxSize = 1 * 1024 * 1024; // 1 MB
    if (filesize > maxSize)
    {
        QMessageBox::warning(this, "图片过大",
                             QString("图片大小超过限制（最大 1 MB）。\n当前大小: %1 KB")
                             .arg(filesize / 1024));
        return;
    }

    // 4. 读取文件并编码为 Base64
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "读取失败", "无法读取图片文件。");
        return;
    }
    QByteArray fileData = file.readAll();
    file.close();
    string imageBase64 = fileData.toBase64().toStdString();

    // 5. 提取文件名
    QString fileName = QFileInfo(filePath).fileName();

    // 6. 本地立即显示图片气泡（乐观 UI）
    QString timeStr = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 先保存到本地 assets
    QString localPath = saveImageToAssets(imageBase64, fileName.toStdString());

    // 显示本地气泡（如果保存失败，仍显示 - 解码内存数据）
    if (localPath.isEmpty())
    {
        // fallback: 直接从原始数据写到临时文件
        QDir().mkpath(assetsDir());
        QString tmpPath = assetsDir() + "/tmp_" + fileName;
        QFile tmpFile(tmpPath);
        if (tmpFile.open(QIODevice::WriteOnly))
        {
            tmpFile.write(fileData);
            tmpFile.close();
            localPath = tmpPath;
        }
    }

    string localPathStr = localPath.toStdString();
    auto *bubble = new MessageBubble("", "", timeStr, true, localPath);
    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(bubble->sizeHint());
    ui->messageList->addItem(item);
    ui->messageList->setItemWidget(item, bubble);
    ui->messageList->scrollToBottom();

    // 7. 持久化到本地存储（不含 Base64，只存 localpath）
    json storeJs;
    storeJs["id"] = _currentUserId;
    storeJs["msgid"] = _isGroupChat ? GROUP_IMAGE_CHAT_MSG : IMAGE_CHAT_MSG;
    storeJs["filename"] = fileName.toStdString();
    storeJs["filesize"] = static_cast<int>(filesize);
    storeJs["mime"] = mimeType.toStdString();
    storeJs["localpath"] = localPathStr;
    storeJs["time"] = timeStr.toStdString();
    appendToStore(storeJs);

    // 8. 发射信号 → MainWidget 发送到网络
    if (_isGroupChat)
        emit sendGroupImage(_targetId, imageBase64, fileName.toStdString(), static_cast<int>(filesize), mimeType.toStdString());
    else
        emit sendOneImage(_targetId, imageBase64, fileName.toStdString(), static_cast<int>(filesize), mimeType.toStdString());
}
