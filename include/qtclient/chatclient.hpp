#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

/*
 * ============================================================================
 * ChatClient — TCP 网络客户端（封装 QTcpSocket）
 * ============================================================================
 *
 * 【功能】
 *   1. 建立/断开与聊天服务器的 TCP 连接
 *   2. 发送 JSON 消息（以 '\0' 结尾的字节流）
 *   3. 接收服务端数据，按 '\0' 分隔解析为多条 JSON 消息
 *   4. 通过 Qt 信号上报连接状态和收到的消息
 *
 * 【Qt 概念】QTcpSocket 是 Qt 提供的 TCP 套接字类，采用异步非阻塞模式。
 *   不用自己开线程，Qt 的事件循环 (event loop) 会自动处理网络 I/O。
 *   连接成功/收到数据/断开连接都会发射信号，只需连接这些信号到槽函数。
 *
 * 【Qt 概念】QObject 是绝大多数 Qt 类的基类。ChatClient 继承 QObject 而不是
 *   QWidget，因为它不需要绘制 UI，只需要信号槽机制。
 *
 * 【消息协议】
 *   每条 JSON 消息以 '\0' (空字符) 分隔，通过 TCP 字节流发送。
 *   例如：{"msgid":1,"id":123}\0{"msgid":5,"msg":"hello"}\0
 *
 * 【如何添加 SSL/TLS 加密】
 *   1. 将 QTcpSocket 替换为 QSslSocket
 *   2. 连接 encrypted() 信号确认握手完成
 *   3. 在 connectToServer 中调用 sslSocket->startClientEncryption()
 */
class ChatClient : public QObject
{
    Q_OBJECT
public:
    explicit ChatClient(QObject *parent = nullptr);
    ~ChatClient();

    // 连接到聊天服务器（ip: 服务器地址, port: 端口号）
    void connectToServer(const QString &ip, quint16 port);
    // 发送一条 JSON 消息到服务器
    void send(const json &js);
    // 主动断开连接
    void disconnect();

signals:
    // 【Qt 概念】信号是 Qt 线程安全的。可以在子线程 emit 信号，槽函数会在
    // 接收者所在的线程执行。本应用所有对象都在主线程，所以不需要跨线程考虑。

    void connected();                           // TCP 连接建立成功
    void disconnected();                        // TCP 连接断开
    void messageReceived(json js);              // 收到一条完整的 JSON 消息
    void errorOccurred(const QString &error);   // 连接出错（含错误描述）

private slots:
    // QTcpSocket 信号 → 本类内部处理 → 对外 emit 信号
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onErrorOccurred(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *_socket;
};

#endif
