/*
 * ============================================================================
 * ChatClient 实现 — TCP 网络客户端
 * ============================================================================
 *
 * 【Qt 概念】QTcpSocket 是异步的。这意味着：
 *   - connectToHost() 发起连接后立即返回，不会阻塞等待
 *   - write() 发送数据后立即返回，不会等待对方确认
 *   - 数据到达时通过 readyRead 信号通知
 *
 * 这种异步模式比多线程更高效率，因为 Qt 在事件循环中通过 select/epoll
 * 等系统调用管理所有 socket，一个线程就能处理成百上千个连接。
 *
 * 【消息分帧】TCP 是字节流协议，没有消息边界。本应用使用 '\0' 字符来分隔
 * JSON 消息。这是最简单的分帧方式。生产环境通常会使用 "长度前缀" 方案
 * （先发送 4 字节消息长度，再发送消息体），避免消息内容中出现 '\0' 的问题。
 */

#include "chatclient.hpp"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>

using namespace std;

// ============================================================================
// 构造 / 析构
// ============================================================================

ChatClient::ChatClient(QObject *parent)
    : QObject(parent), _socket(new QTcpSocket(this))
{
    /*
     * 将底层 QTcpSocket 的信号连接为本对象的槽。
     * QTcpSocket 的信号含义：
     *   connected()    → TCP 三次握手完成
     *   disconnected() → 连接已断开
     *   readyRead()    → 有数据可读
     *   errorOccurred()→ 发生错误
     */
    connect(_socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(_socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(_socket, &QAbstractSocket::errorOccurred, this, &ChatClient::onErrorOccurred);
}

ChatClient::~ChatClient()
{
    // 如果还连着，优雅断开（发送 FIN 包，等待对方确认）
    if (_socket->state() == QAbstractSocket::ConnectedState)
    {
        _socket->disconnectFromHost();
    }
}

// ============================================================================
// 公开方法
// ============================================================================

void ChatClient::connectToServer(const QString &ip, quint16 port)
{
    _socket->connectToHost(QHostAddress(ip), port);
}

void ChatClient::send(const json &js)
{
    // 发送前检查连接状态
    if (_socket->state() != QAbstractSocket::ConnectedState)
    {
        cerr << "not connected to server" << endl;
        return;
    }

    /*
     * 消息序列化：json → string → QByteArray
     * 发送长度 = 字符串长度 + 1（多发的 1 字节是结尾的 '\0'，作为消息分隔符）
     *
     * 【安全注意】如果 JSON 内容中本身就包含 '\0' 字符（比如用户发送二进制
     * 数据），这种分帧方式会出错。更健壮的方案是用 4 字节长度前缀。
     */
    string data = js.dump();
    _socket->write(data.c_str(), data.size() + 1);
}

void ChatClient::disconnect()
{
    _socket->disconnectFromHost();
}

// ============================================================================
// 私有槽（内部处理 QTcpSocket 信号）
// ============================================================================

void ChatClient::onConnected()
{
    cout << "connect to server success!" << endl;
    emit connected();  // 转发信号给上层（MainWidget）
}

void ChatClient::onDisconnected()
{
    cout << "disconnected from server" << endl;
    emit disconnected();
}

void ChatClient::onReadyRead()
{
    /*
     * 读取所有缓冲数据。可能一次收到：
     *   - 一条完整消息
     *   - 一条不完整消息（TCP 分包）
     *   - 多条消息粘在一起
     *
     * 按 '\0' 字符分割，解析每条完整的 JSON 消息。
     * 不完整的消息片段（末尾没有 '\0'）也尝试解析。
     */
    QByteArray data = _socket->readAll();

    int start = 0;
    for (int i = 0; i < data.size(); i++)
    {
        if (data[i] == '\0')
        {
            if (i > start)
            {
                try
                {
                    string msgStr(data.constData() + start, i - start);
                    json js = json::parse(msgStr);
                    emit messageReceived(js);
                }
                catch (const exception &e)
                {
                    cerr << "json parse error: " << e.what() << endl;
                }
            }
            start = i + 1;
        }
    }

    // 处理尾部没有 '\0' 的残余数据
    if (start < data.size())
    {
        try
        {
            string msgStr(data.constData() + start, data.size() - start);
            if (!msgStr.empty())
            {
                json js = json::parse(msgStr);
                emit messageReceived(js);
            }
        }
        catch (const exception &e)
        {
            cerr << "json parse error: " << e.what() << endl;
        }
    }
}

void ChatClient::onErrorOccurred(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);  // 消除未使用参数的编译警告
    emit errorOccurred(_socket->errorString());
}
