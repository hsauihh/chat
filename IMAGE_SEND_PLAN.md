# 图片发送功能计划书

## 1. 目标

在现有聊天项目框架上增加图片发送能力，使 Qt 客户端支持在单聊和群聊中选择图片、发送图片、接收图片、显示图片气泡，并将图片消息纳入现有本地聊天记录和离线消息流程。

本计划完全基于当前架构：

- 网络协议继续使用 TCP + JSON。
- 客户端继续使用 Qt Widgets。
- 服务端继续使用 ChatServer + ChatService + Model 三层结构。
- 在线消息继续走本机连接表或 Redis 跨服务器转发。
- 离线消息继续走 OfflineMsgModel。
- 本地聊天记录继续由 ChatWidget 管理。

## 2. 任务边界

### 2.1 本期包含

1. 单聊图片发送。
2. 群聊图片发送。
3. 在线用户实时接收图片。
4. 离线用户登录后接收图片消息。
5. 当前聊天窗口内显示图片气泡。
6. 非当前聊天收到图片时存入本地记录并显示未读红点。
7. 图片消息随聊天记录一起本地持久化。
8. 客户端限制图片类型和大小，避免超大 JSON 影响稳定性。
9. 服务端增加图片消息路由，但不解析图片内容。

### 2.2 本期不包含

1. 不做服务端图片文件存储。
2. 不做 HTTP 图片下载接口。
3. 不做断点续传。
4. 不做发送进度条。
5. 不做图片压缩编辑器。
6. 不做拖拽发送。
7. 不做大图预览弹窗。
8. 不做多图片批量发送。
9. 不做端到端加密。

### 2.3 关键约束

1. 图片第一版通过 Base64 放入 JSON 传输。
2. 单张图片大小限制建议为 1 MB，最多不超过 2 MB。
3. 支持格式限定为 png、jpg、jpeg、webp。
4. 图片消息必须兼容现有好友、群组、Redis、离线消息流程。
5. 修改 `.ui` 文件后必须重新执行 `cd build && cmake .. && make`。
6. 必须保留 `ChatWidget` 构造函数中对 `WA_InputMethodEnabled` 的设置。

## 3. 总体方案

客户端选择图片后读取为二进制数据，转换为 Base64 字符串，放入 JSON 消息发送到服务端。服务端只根据 `msgid`、目标用户或群组进行消息转发，不关心图片内容。接收端收到图片消息后解码保存到本地图片资源目录，并在聊天记录 JSON 中保存图片本地路径和消息元信息。

网络传输格式使用完整 JSON：

```json
{
  "msgid": 11,
  "id": 1,
  "name": "alice",
  "to": 2,
  "time": "2026-05-30 12:00:00",
  "filename": "photo.png",
  "filesize": 123456,
  "mime": "image/png",
  "image": "base64..."
}
```

本地存储格式去掉 Base64，只保存图片本地路径：

```json
{
  "msgid": 11,
  "id": 1,
  "name": "alice",
  "to": 2,
  "time": "2026-05-30 12:00:00",
  "filename": "photo.png",
  "filesize": 123456,
  "mime": "image/png",
  "localpath": ".../messages/assets/user_2/xxx.png"
}
```

## 4. 消息协议改造任务

### 4.1 修改文件

- `include/public.hpp`

### 4.2 任务

1. 在 `EnMsgType` 中新增图片消息类型。
2. 建议枚举值放在现有聊天消息之后，保持语义清晰。

建议定义：

```cpp
IMAGE_CHAT_MSG,       // 单聊图片消息
GROUP_IMAGE_CHAT_MSG  // 群聊图片消息
```

### 4.3 验收标准

1. 服务端、终端客户端、Qt 客户端都能包含新的消息枚举。
2. 原有文本消息枚举值不被破坏。

## 5. TCP 分帧稳定性任务

### 5.1 修改文件

- `src/server/chatserver.cpp`
- 如需要，可修改 `include/server/chatserver.hpp`
- 如需要，可为每个连接增加上下文缓存

### 5.2 背景

Qt 客户端当前发送 JSON 时以 `\0` 作为消息结束符，但服务端当前直接对 `buffer->retrieveAllAsString()` 做 `json::parse()`。图片 Base64 会显著增大 JSON，TCP 半包和粘包概率升高，如果不先修复分帧，图片发送会不稳定。

### 5.3 任务

1. 服务端读取 TCP 数据后按 `\0` 拆分完整 JSON 消息。
2. 对不完整尾部数据进行缓存，等待下次 `onMessage()` 拼接。
3. 每解析出一条完整 JSON，再调用 `ChatService::getHandler()` 分发。
4. 对 JSON parse 异常做日志记录，不让服务端崩溃。

### 5.4 验收标准

1. 连续发送多条文本消息不会粘包解析失败。
2. 发送较大的 JSON 消息不会因为半包直接 parse 失败。
3. 错误 JSON 不导致服务端进程退出。

## 6. 服务端业务路由任务

### 6.1 修改文件

- `include/server/chatservice.hpp`
- `src/server/chatservice.cpp`

### 6.2 任务

1. 在 `ChatService` 构造函数中注册图片消息 handler。
2. 新增 `imageChat()`，逻辑复用 `oneChat()`。
3. 新增 `groupImageChat()`，逻辑复用 `groupChat()`。
4. 修复群聊转发中 Redis publish 后提前 `return` 的问题，应继续处理后续群成员。
5. 持锁范围尽量只覆盖 `_userConnectionMap` 查询，不在锁内执行数据库查询、Redis publish 或离线消息写入。

### 6.3 单聊图片流程

1. 读取 `to` 字段。
2. 检查目标用户是否在本机在线。
3. 本机在线则直接 `conn->send(js.dump())`。
4. 不在本机则查询用户状态。
5. 如果用户状态为 online，则通过 Redis publish。
6. 如果用户离线，则写入 OfflineMsgModel。

### 6.4 群聊图片流程

1. 读取 `id` 和 `groupid`。
2. 查询群内除发送者外的成员列表。
3. 遍历每个成员。
4. 本机在线则直接发送。
5. 其他服务器在线则 Redis publish。
6. 离线则写入 OfflineMsgModel。
7. 任意成员处理完成后不得提前中断整个群发流程。

### 6.5 验收标准

1. 单聊图片在线转发成功。
2. 单聊图片离线存储成功。
3. 群聊图片能发送给所有在线和离线成员。
4. Redis 跨服务器路径与文本消息一致。

## 7. Qt 客户端发送入口任务

### 7.1 修改文件

- `src/qtclient/chatwidget.ui`
- `include/qtclient/chatwidget.hpp`
- `src/qtclient/chatwidget.cpp`

### 7.2 UI 任务

1. 在聊天输入区增加图片按钮。
2. 图片按钮放在发送按钮附近。
3. 按钮文本可暂用“图片”，后续可替换为图标。

### 7.3 ChatWidget 任务

1. 增加图片按钮点击槽函数，例如 `onImageClicked()`。
2. 使用 `QFileDialog::getOpenFileName()` 选择图片。
3. 校验文件格式。
4. 校验文件大小。
5. 使用 `QFile` 读取图片。
6. 使用 `QByteArray::toBase64()` 编码。
7. 根据当前聊天类型发出信号：
   - `sendOneImage(...)`
   - `sendGroupImage(...)`
8. 本地立即显示图片气泡，实现乐观 UI。
9. 将发送的图片保存到本地 assets 目录，并写入聊天记录。

### 7.4 验收标准

1. 未选择聊天对象时点击图片按钮不发送。
2. 选择不支持格式时提示用户。
3. 选择超大图片时提示用户。
4. 选择合法图片后当前聊天窗口立即出现图片气泡。

## 8. MainWidget 控制器任务

### 8.1 修改文件

- `include/qtclient/mainwidget.hpp`
- `src/qtclient/mainwidget.cpp`

### 8.2 任务

1. 在 `setupMainPage()` 中连接 ChatWidget 的图片发送信号。
2. 新增 `handleSendOneImage()`。
3. 新增 `handleSendGroupImage()`。
4. 在槽函数中组装图片 JSON。
5. 调用 `_client->send(js)`。
6. 在 `handleMessageReceived()` 中新增图片消息分发。
7. 图片消息可直接交给 `_chatWidget->displayMessage(js)`。

### 8.3 验收标准

1. 单聊图片 JSON 字段完整。
2. 群聊图片 JSON 字段完整。
3. 收到图片消息时能进入 ChatWidget 路由。

## 9. 图片气泡渲染任务

### 9.1 修改文件

- `include/qtclient/messagebubble.hpp`
- `src/qtclient/messagebubble.cpp`
- 如需要，可修改 `src/qtclient/messagebubble.ui`

### 9.2 任务

1. 保留现有文本气泡构造方式。
2. 新增图片气泡构造方式。
3. 使用 QLabel 显示 QPixmap。
4. 图片最大显示宽度建议为 240 到 320 像素。
5. 保持原图宽高比缩放。
6. 自己发送的图片靠右，对方发送的图片靠左。
7. 群聊中对方图片仍显示发送者名称。

### 9.3 验收标准

1. 文本消息显示不受影响。
2. 图片消息显示为缩略图。
3. 自己和对方的图片气泡方向正确。
4. 图片过大时不会撑破聊天窗口布局。

## 10. ChatWidget 消息路由与持久化任务

### 10.1 修改文件

- `include/qtclient/chatwidget.hpp`
- `src/qtclient/chatwidget.cpp`

### 10.2 任务

1. `displayMessage()` 支持 `IMAGE_CHAT_MSG` 和 `GROUP_IMAGE_CHAT_MSG`。
2. 图片消息按单聊或群聊计算所属聊天对象。
3. 属于当前聊天则显示图片气泡。
4. 不属于当前聊天则调用 `storeMessage()` 存储并触发未读红点。
5. `storeMessage()` 支持图片消息。
6. 图片消息含 `image` 字段时，先 Base64 解码并保存到本地 assets 目录。
7. 存入 `_messageStore` 前移除 `image` 字段，加入 `localpath` 字段。
8. `loadMessagesForTarget()` 根据 `localpath` 恢复图片气泡。
9. `clearCurrentChat()` 删除当前聊天 JSON 时，也可删除对应 assets 目录下的图片文件。

### 10.3 本地目录建议

```text
<AppDataLocation>/
  messages/
    user_2.json
    group_10.json
    assets/
      user_2/
      group_10/
```

### 10.4 验收标准

1. 当前聊天收到图片立即显示。
2. 非当前聊天收到图片只显示红点，不打断当前聊天。
3. 切换到对应聊天后图片能从本地记录恢复。
4. 关闭并重启客户端后图片消息仍能显示。

## 11. 离线消息任务

### 11.1 修改文件

- 服务端数据库脚本，如项目中后续补充 SQL 初始化文件
- 当前可能需要检查 OfflineMessage 表字段类型

### 11.2 任务

1. 确认离线消息字段是否能存储较大的 Base64 JSON。
2. 如果字段较小，调整为 `MEDIUMTEXT`。
3. 登录响应中的 `offlinemsg` 继续携带图片消息 JSON。
4. Qt 客户端登录后继续通过 `ChatWidget::storeMessage()` 存储离线图片。

### 11.3 验收标准

1. A 给离线 B 发送图片。
2. B 登录后联系人出现未读提示。
3. B 打开对应聊天后能看到图片。

## 12. 测试流程

### 12.1 构建测试

1. 修改 `.ui` 后执行：

```bash
cd build && cmake .. && make QtChatClient
```

2. 服务端改动后执行：

```bash
cd build && cmake .. && make ChatServer
```

### 12.2 功能测试

1. 单聊文本消息仍可发送。
2. 群聊文本消息仍可发送。
3. 单聊发送 png 图片成功。
4. 单聊发送 jpg 图片成功。
5. 群聊发送图片成功。
6. 未打开的聊天收到图片后显示红点。
7. 切换聊天后图片从本地记录加载。
8. 离线用户登录后能看到图片。
9. 超大图片被客户端拒绝。
10. 非图片文件被客户端拒绝。

### 12.3 回归测试

1. 登录、注册流程不受影响。
2. 添加好友、创建群、加入群不受影响。
3. 清理当前聊天记录后，文本和图片记录都被清理。
4. 断开连接后发送图片不会崩溃。

## 13. 推荐实施顺序

1. 新增消息枚举。
2. 修复服务端 TCP 分帧。
3. 服务端新增图片 handler。
4. Qt 客户端新增图片按钮和选图逻辑。
5. MainWidget 增加图片 JSON 组装和消息分发。
6. MessageBubble 支持图片缩略图。
7. ChatWidget 支持图片路由、本地保存和历史恢复。
8. 检查离线消息字段容量。
9. 完成在线、离线、群聊测试。
10. 整理 README 或开发手册中的使用说明。

## 14. 风险与应对

### 14.1 大图片导致 JSON 过大

应对：

- 客户端限制单张图片大小。
- 第一版建议 1 MB。
- 后续再考虑服务端文件存储。

### 14.2 TCP 半包导致解析失败

应对：

- 图片功能前必须先修复服务端分帧。

### 14.3 本地聊天记录文件膨胀

应对：

- 网络 JSON 中携带 Base64。
- 本地 JSON 中只保存 `localpath`。

### 14.4 离线消息字段容量不足

应对：

- 检查数据库表结构。
- 必要时改为 `MEDIUMTEXT`。

### 14.5 UI 卡顿

应对：

- 限制图片大小。
- 第一版同步读取可接受。
- 后续可将图片读取和缩略图生成移到后台线程。

## 15. 版本切分建议

### v1：最小可用版本

1. 单聊图片发送。
2. 群聊图片发送。
3. 图片气泡显示。
4. 本地持久化。
5. 离线图片接收。

### v2：体验增强

1. 点击图片预览大图。
2. 图片发送失败状态。
3. 图片压缩。
4. 发送进度显示。
5. 多图批量发送。

### v3：架构升级

1. 服务端保存图片文件。
2. JSON 消息只传图片 URL 或资源 ID。
3. HTTP 或静态文件服务下载图片。
4. 图片鉴权和清理策略。

