#include "chatserver.hpp"
#include <functional>
#include <iostream>
#include <string>
#include <cstring>
#include "json.hpp"
#include "chatservice.hpp"
#include <muduo/base/Logging.h>

using namespace std;
using namespace placeholders;
using json = nlohmann::json;
ChatServer::ChatServer(EventLoop *loop,
                       const InetAddress &listenAddr,
                       const string &nameArg)
    : _server(loop, listenAddr, nameArg), _loop(loop)
{

    // 注册链接回调
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection, this, _1));

    // 注册消息回调
    _server.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    // 设置线程数量
    _server.setThreadNum(4);
}

void ChatServer::start()
{
    _server.start();
}

// 上报链接相关信息的回调函数
void ChatServer::onConnection(const TcpConnectionPtr &conn)
{
    // 客户端断开链接
    if (!conn->connected()) 
    {
        ChatService::instance()->clientCloseException(conn);
        conn->shutdown();
    }
}

// 上报读写事件相关信息的回调函数
void ChatServer::onMessage(const TcpConnectionPtr &conn,
               Buffer *buffer,
               Timestamp time)
{
    // 按 '\0' 分隔处理粘包/半包，支持大 JSON（如图片 Base64）
    while (buffer->readableBytes() > 0)
    {
        const char *data = buffer->peek();
        size_t len = buffer->readableBytes();

        const char *end = (const char *)memchr(data, '\0', len);
        if (end == nullptr) break; // 不完整消息，等下次 onMessage 拼接

        size_t msgLen = end - data;
        if (msgLen > 0)
        {
            try {
                string msg(data, msgLen);
                json js = json::parse(msg);
                auto msghandler = ChatService::instance()->getHandler(js["msgid"].get<int>());
                msghandler(conn, js, time);
            } catch (const exception &e) {
                LOG_ERROR << "JSON parse error: " << e.what();
            }
        }

        buffer->retrieve(msgLen + 1); // 移除已处理的消息+分隔符
    }
}