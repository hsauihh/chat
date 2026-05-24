/*
 * ============================================================================
 * MessageBubble 实现 — 单条消息气泡
 * ============================================================================
 *
 * 【Qt 概念】本文件展示了如何通过代码动态调整 .ui 文件中定义的布局：
 *   - 修改 QSpacerItem 的 sizeHint 来改变弹性伸缩行为
 *   - 通过 hide() 隐藏不需要的 Widget
 *   - 通过 setStyleSheet() 动态修改样式
 *
 * 【左右对齐的实现原理】
 *   气泡的水平布局是: [左spacer] [气泡] [名称] [右spacer]
 *   - 自己的消息（靠右）: 左spacer 设为 Expanding（抢占空间），右spacer 归零
 *   - 对方的消息（靠左）: 右spacer 设为 Expanding，左spacer 归零
 *
 *   spacer 的 Expanding 策略会把气泡"推"到对面去。
 */

#include "messagebubble.hpp"
#include "ui_messagebubble.h"
#include <QLabel>

MessageBubble::MessageBubble(const QString &senderName,
                             const QString &message,
                             const QString &timeStr,
                             bool isSelf,
                             QWidget *parent)
    : QWidget(parent), ui(new Ui::MessageBubble)
{
    ui->setupUi(this);

    // 设置时间和消息文本
    ui->timeLabel->setText(timeStr);
    ui->bubbleLabel->setText(message);

    if (isSelf)
    {
        // ---- 自己的消息：绿色气泡靠右 ----
        ui->bubbleLabel->setStyleSheet(
            "padding: 8px 12px; border-radius: 8px; font-size: 14px; "
            "background-color: #95ec69; color: #000;");
        // 右spacer 收缩到 0，左spacer 扩展 → 气泡被推到右边
        ui->rightSpacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->leftSpacer->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        ui->nameLabel->hide();  // 自己的消息不显示名字
    }
    else
    {
        // ---- 对方的消息：白色气泡靠左 ----
        ui->bubbleLabel->setStyleSheet(
            "padding: 8px 12px; border-radius: 8px; font-size: 14px; "
            "background-color: #ffffff; color: #000;");
        // 左spacer 收缩到 0，右spacer 扩展 → 气泡被推到左边
        ui->leftSpacer->changeSize(0, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
        ui->rightSpacer->changeSize(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        ui->nameLabel->setText(senderName);  // 显示对方的名字
    }
}

MessageBubble::~MessageBubble()
{
    delete ui;
}
