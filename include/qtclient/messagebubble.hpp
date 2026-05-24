#ifndef MESSAGEBUBBLE_HPP
#define MESSAGEBUBBLE_HPP

#include <QWidget>
#include <QString>

namespace Ui {
    class MessageBubble;
}

/*
 * ============================================================================
 * MessageBubble — 单条消息气泡组件
 * ============================================================================
 *
 * 【功能】显示单条聊天消息的气泡，样式根据是否为自己发送而不同：
 *   - 自己的消息：绿色背景 (#95ec69)，靠右对齐，不显示名称
 *   - 对方的消息：白色背景 (#ffffff)，靠左对齐，显示发送者名称
 *
 * 【Qt 概念】这是一个"自定义 Widget"，设计模式：
 *   1. 在 messagebubble.ui 中设计布局（时间标签、气泡标签、名称标签、spacer）
 *   2. 在构造函数中根据 isSelf 参数动态调整样式和布局
 *   3. 通过 QListWidget::setItemWidget() 嵌入到消息列表中
 *
 * 【UI 结构（messagebubble.ui 定义）】
 *   顶层: 垂直布局
 *   ├── timeLabel (时间，居中灰色)
 *   └── 水平气泡行
 *        ├── leftSpacer  (弹性伸缩)
 *        ├── bubbleLabel (消息文本，圆角背景)
 *        ├── nameLabel   (发送者名称)
 *        └── rightSpacer (弹性伸缩)
 *
 * 【如何添加新消息类型（图片、文件等）】
 *   1. 在 messagebubble.ui 中添加对应的 QLabel 或自定义 Widget
 *   2. 在构造函数中添加对应的初始化代码
 *   3. 参考 bubbleLabel 的 spacer 控制逻辑来实现左右对齐
 */
class MessageBubble : public QWidget
{
    Q_OBJECT
public:
    // senderName: 发送者名称（自己的消息传空字符串）
    // message:    消息文本内容
    // timeStr:    时间字符串（格式: "yyyy-MM-dd HH:mm:ss"）
    // isSelf:     true=自己发送（绿色靠右），false=对方发送（白色靠左）
    explicit MessageBubble(const QString &senderName,
                           const QString &message,
                           const QString &timeStr,
                           bool isSelf,
                           QWidget *parent = nullptr);
    ~MessageBubble();

private:
    Ui::MessageBubble *ui;
};

#endif
