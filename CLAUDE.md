# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 构建命令

```bash
# 配置并构建全部目标（在仓库根目录下执行）
cd build && cmake .. && make

# 构建指定目标
cd build && make ChatServer      # 聊天服务器
cd build && make ChatClient      # 终端客户端
cd build && make QtChatClient    # Qt6 图形客户端

# 清理构建
cd build && make clean && make

# 构建产物位于项目根目录下的 bin/
#   bin/ChatServer
#   bin/ChatClient
#   bin/QtChatClient
```

项目默认使用 CMake 的 `-g`（调试符号）编译。`src/chat/` 下的 QML 应用是独立的 CMake 项目，需单独配置构建。

## 架构

基于 [muduo](https://github.com/chenshuo/muduo) 网络库的 C++ 客户端-服务器聊天应用。

### 服务端 (`src/server/`)

**网络层 → 业务层 → 数据层** 三层设计：

1. **`ChatServer`** — 网络层。封装 muduo 的 `TcpServer`，使用 4 个 I/O 线程。注册 `onConnection` 和 `onMessage` 回调。反序列化收到的 JSON 消息，根据 `msgid` 分派给 `ChatService`。
2. **`ChatService`**（单例）— 业务逻辑路由。将消息 ID 映射到处理方法（`_msgHandlerMap`）。处理：登录、注册、注销、一对一聊天、添加好友、创建/加入群组、群组聊天。通过 `_userConnectionMap`（用户 id → TcpConnectionPtr）跟踪在线用户。
3. **Model 类** — 数据访问层。每个 model 方法自行创建短生命周期的 MySQL 连接：`UserModel`、`FriendModel`、`GroupModel`、`OfflineMsgModel`。
4. **`MySQL`**（`db/`）— 对 mysqlclient C API 的薄封装。连接参数硬编码（127.0.0.1，用户 `chat`，数据库 `chat`）。
5. **`Redis`**（`redis/`）— 用于跨服务器消息投递的发布/订阅机制。订阅以用户 id 为通道名的频道。当用户连接到服务器 A 但其好友在服务器 B 上时，消息通过 Redis 发布，以便服务器 B 投递。

**消息协议**：基于 TCP 的 JSON，使用 `nlohmann/json`。每条消息都有一个 `msgid` 字段（参见 `include/public.hpp` 中的枚举 `EnMsgType`）。服务器读取 `msgid`，查找对应的 handler，然后分发处理。

### 终端客户端 (`src/client/`)

基于终端、使用原始 POSIX socket 的客户端。主线程处理用户输入/发送；独立的 `readTaskHandler` 线程接收消息。命令系统采用 `命令:参数1:参数2` 格式（如 `chat:friendid:消息`）。命令通过 `commandMap`/`commandHandlerMap` 两个 map 进行查找和分发。

### Qt 客户端 (`src/qtclient/`)

基于 Qt6 Widgets 的图形客户端，使用与终端客户端相同的 JSON 协议与服务端通信。

**页面结构** — 两层 QStackedWidget：
```
_pagStack (顶层: 登录页 ↔ 主页面)
├── [0] LoginWidget — 登录/注册
└── [1] _mainPage
     └── HBoxLayout
          ├── SidebarWidget (60px 左侧导航栏)
          ├── 分隔线 (1px)
          └── _rightPanel (QStackedWidget)
               ├── [0] ContactWidget(30%) + ChatWidget(70%) — 聊天视图
               └── [1] ContactsManageWidget — 联系人管理
```

**组件说明**：

- **`ChatClient`** — 网络层，封装 `QTcpSocket`。以 `\0` 分隔 TCP 字节流中的 JSON 消息帧。通过 Qt 信号（`connected`、`disconnected`、`messageReceived`、`errorOccurred`）向业务层上报事件。异步非阻塞模式，由 Qt 事件循环管理 I/O。

- **`MainWidget`** — 主控制器（MVC 的 C）。管理页面切换、连接所有子组件的信号/槽、构造 JSON 消息并调用 `ChatClient::send()`。`handleMessageReceived()` 是消息分发中心，根据 `msgid` 路由到不同 handler。

- **`SidebarWidget`** — 左侧导航栏（60px 固定宽度）。UI 由 sidebar.ui 定义，按钮图标用 QPainter 程序化绘制（无需外部图片文件）。所有按钮只 emit 信号（`chatNavClicked`、`contactsNavClicked`、`logoutRequest`、`clearHistoryRequest`），不执行实际操作 — 这是 Qt 推荐的解耦模式。

- **`LoginWidget`** — 登录/注册页。内部用 `QStackedWidget` 切换登录页（ID + 密码）和注册页（名称 + 密码）。emit `loginRequest(id, password)` 和 `registerRequest(name, password)` 信号。

- **`ContactWidget`** — 联系人列表。维护好友列表（`friendList`）和群组列表（`groupList`），支持搜索框实时过滤（通过 `setHidden` 而非清空重建）。数据用 `setData(Qt::UserRole, id)` 隐藏在 QListWidgetItem 中。双击发射 `chatRequested(userId, name)` 或 `groupChatRequested(groupId, name)` 信号（名称从 item 文本中提取）。支持红点未读标记（`setItemUnread`，绘制 10x10 红色圆点 QIcon）。

- **`ChatWidget`** — 聊天区域（最复杂组件）。包含顶部信息栏（名称 + 状态）、QListWidget 消息列表（嵌入 MessageBubble）、底部 QPlainTextEdit 输入框 + 发送按钮。支持一对一聊天和群聊两种模式。核心功能：
  - **消息路由** (`displayMessage`)：收到消息后判断是否属于当前聊天对象。匹配 → 渲染气泡；不匹配 → 静默存入对应聊天的本地存储 + emit `unreadMessageStored` 触发红点。
  - **本地持久化**：消息同时保存在内存 map（`_messageStore`，key="user_123"/"group_456"）和磁盘 JSON 文件（`~/.local/share/<AppName>/messages/`）。切换聊天时先 `saveCurrentMessages()` 存档 → 清空 UI → `loadMessagesForTarget()` 从文件恢复。消息发送/收到时实时 `appendToStore()` + `persistToFile()`。
  - **清理聊天记录** (`clearCurrentChat`)：三处清理 — UI (`messageList->clear()`)、内存 (`_messageStore.erase(key)`)、磁盘 (`QFile::remove(path)`)。
  - **离线消息存储** (`storeMessage`)：登录时收到的离线消息按 msgid 类型路由到对应聊天文件，不直接渲染到 UI。

- **`MessageBubble`** — 单条消息气泡。UI 由 messagebubble.ui 定义：时间标签 + 水平气泡行（左spacer + 气泡标签 + 名称标签 + 右spacer）。自己的消息：绿色背景(#95ec69)、靠右（左spacer Expanding、右spacer 归零）、隐藏名称；对方的消息：白色背景、靠左、显示名称。

- **`ContactsManageWidget`** — 联系人管理页面（`_rightPanel` index 1）。包含添加好友、创建群组、加入群组的表单。emit `addFriendRequest`、`createGroupRequest`、`addGroupRequest` 信号。

- **`CreateGroupWidget`** — 创建/加入群组对话框（QDialog 子类，独立组件）。支持创建模式（显示名称+描述）和加入模式（显示 ID 输入），通过 `setCreateMode()`/`setJoinMode()` 切换。

**消息流**：
```
发送: 用户输入 → ChatWidget::onSendClicked()
      → emit sendOneChat/sendGroupChat (→ MainWidget::handleSendXxx → _client->send())
      → 本地立即创建 MessageBubble（乐观 UI）
      → appendToStore (内存+磁盘)

接收: TCP → ChatClient::onReadyRead() → emit messageReceived
      → MainWidget::handleMessageReceived → switch(msgid)
      → handleOneChatMessage/handleGroupChatMessage
      → ChatWidget::displayMessage → 路由判断 → 渲染/存储
```

**信号连接**：所有子组件 → MainWidget 的 connect 集中在 `setupMainPage()` 中（mainwidget.cpp:96-117）。子组件之间不直接互相依赖。

### QML 应用 (`src/chat/`)

独立 CMake 项目，使用 Qt6 Quick + QML，当前为 Hello World 骨架阶段（一个带虚拟键盘的 Window）。

## 关键依赖

- **muduo** — TCP 网络库（库文件：`muduo_net`、`muduo_base`）
- **mysqlclient** — MySQL C 客户端
- **hiredis** — Redis C 客户端
- **nlohmann/json** — JSON 解析（单头文件，位于 `thirdparty/`）
- **Qt6** — Widgets、Network（Qt 客户端）；Quick（QML 应用）

## 注意事项

- 没有测试套件。
- 数据库对用户状态使用软删除（online/offline 状态切换），不实际删除行。
- 跨服务器消息投递：`oneChat` 先检查本地连接，未命中则回退到 Redis 发布。群聊消息遍历所有成员，采用相同模式。
- 收到 SIGINT 信号时，服务器将数据库中所有在线用户状态重置为 offline。
- 此项目为学习性质 — SQL 查询使用 `sprintf` 拼接（存在注入风险），数据库凭据硬编码在源码中。
- 客户端入口 `main.cpp` 的 `QT_IM_MODULE` 设为空字符串（compose 模块），适配 WSLg 环境。如果在纯 Linux 下开发且有 fcitx/ibus，可改为对应的值。
- QPlainTextEdit 的 `WA_InputMethodEnabled` 已在 ChatWidget 构造函数中显式启用，修改输入框相关代码时不要移除此行。
- .ui 文件修改后必须 `cd build && cmake .. && make`（不能只 make），否则 UIC 不会重新生成 `ui_xxx.h`。
- 详细开发文档位于 `docs/DEV_MANUAL.html`，包含 Qt 速成课、每个函数说明和 5 个新功能实战案例。
