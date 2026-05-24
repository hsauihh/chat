/*
 * ============================================================================
 * ContactWidget 实现 — 联系人列表
 * ============================================================================
 *
 * 【Qt 概念】QListWidget 的使用模式：
 *   1. new QListWidgetItem("显示文本", parentList)
 *   2. item->setData(Qt::UserRole, yourData) — 在 item 中隐藏自定义数据
 *   3. item->data(Qt::UserRole).toInt() — 点击时取出数据
 *   4. item->setHidden(true/false) — 搜索过滤
 *
 * 【搜索过滤的实现】
 *   不删除/重建 item，而是用 setHidden() 隐藏不匹配的项。
 *   这样做比清空+重建更高效，且保留了 item 上的自定义数据。
 */

#include "contactwidget.hpp"
#include "ui_contactwidget.h"
#include <QListWidget>
#include <QLineEdit>
#include <QPainter>
#include <QPixmap>
#include <iostream>

using namespace std;

// ============================================================================
// 构造 / 析构
// ============================================================================

ContactWidget::ContactWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ContactWidget)
{
    ui->setupUi(this);

    // QListWidget::itemDoubleClicked(QListWidgetItem*) 是 Qt 预定义的信号
    connect(ui->friendList, &QListWidget::itemDoubleClicked, this, &ContactWidget::onFriendDoubleClicked);
    connect(ui->groupList, &QListWidget::itemDoubleClicked, this, &ContactWidget::onGroupDoubleClicked);

    // QLineEdit::textChanged(QString) 在每次文本变化时触发（实时搜索）
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &ContactWidget::onSearchChanged);
}

ContactWidget::~ContactWidget()
{
    delete ui;
}

// ============================================================================
// 搜索过滤
// ============================================================================

void ContactWidget::onSearchChanged(const QString &text)
{
    _searchText = text;
    applySearchFilter();
}

void ContactWidget::applySearchFilter()
{
    // 好友列表：名称包含搜索文本的才显示
    for (int i = 0; i < ui->friendList->count(); ++i)
    {
        QListWidgetItem *item = ui->friendList->item(i);
        bool visible = _searchText.isEmpty() ||
                       item->text().contains(_searchText, Qt::CaseInsensitive);
        item->setHidden(!visible);
    }
    // 群组列表同理
    for (int i = 0; i < ui->groupList->count(); ++i)
    {
        QListWidgetItem *item = ui->groupList->item(i);
        bool visible = _searchText.isEmpty() ||
                       item->text().contains(_searchText, Qt::CaseInsensitive);
        item->setHidden(!visible);
    }
}

// ============================================================================
// 列表数据更新
// ============================================================================

void ContactWidget::updateFriendList(vector<User> &friends)
{
    ui->friendList->clear();  // 清空旧数据
    for (auto &user : friends)
    {
        // 显示格式: "名称 [ID] (online/offline)"
        QString text = QString::fromStdString(user.getName())
                       + " [" + QString::number(user.getId()) + "] "
                       + "(" + QString::fromStdString(user.getState()) + ")";

        auto *item = new QListWidgetItem(text);
        // 把用户 ID 藏在 item 中，双击时通过 data(Qt::UserRole) 取出
        item->setData(Qt::UserRole, user.getId());
        ui->friendList->addItem(item);
    }
    ui->friendCountLabel->setText(QString("%1人").arg(friends.size()));
    applySearchFilter();  // 更新列表后重新应用搜索过滤
}

void ContactWidget::updateGroupList(vector<Group> &groups)
{
    ui->groupList->clear();
    for (auto &group : groups)
    {
        // 显示格式: "群名 [ID] - 描述"
        QString text = QString::fromStdString(group.getName())
                       + " [" + QString::number(group.getId()) + "] "
                       + "- " + QString::fromStdString(group.getDesc());

        auto *item = new QListWidgetItem(text);
        item->setData(Qt::UserRole, group.getId());  // 把群组 ID 藏在 item 中
        ui->groupList->addItem(item);
    }
    ui->groupCountLabel->setText(QString("%1个").arg(groups.size()));
    applySearchFilter();
}

void ContactWidget::updateFriendState(int userId, const string &state)
{
    // 在线状态变化时，找到对应 item 并更新文本中的状态部分
    for (int i = 0; i < ui->friendList->count(); ++i)
    {
        QListWidgetItem *item = ui->friendList->item(i);
        if (item->data(Qt::UserRole).toInt() == userId)
        {
            // 解析 item 文本："名称 [ID] (状态)" → 提取名称 + 拼接新状态
            QString text = item->text();
            int bracketPos = text.indexOf(" [");
            QString name = (bracketPos > 0) ? text.left(bracketPos) : text;
            QString newText = name + " [" + QString::number(userId) + "] "
                              + "(" + QString::fromStdString(state) + ")";
            item->setText(newText);
            break;
        }
    }
}

// ============================================================================
// 双击事件 → 发出信号
// ============================================================================

void ContactWidget::onFriendDoubleClicked(QListWidgetItem *item)
{
    int userId = item->data(Qt::UserRole).toInt();
    // 从显示文本中提取名称（格式: "名称 [ID] (状态)"）
    QString text = item->text();
    int bracketPos = text.indexOf(" [");
    QString name = (bracketPos > 0) ? text.left(bracketPos) : text;
    emit chatRequested(userId, name);
}

void ContactWidget::onGroupDoubleClicked(QListWidgetItem *item)
{
    int groupId = item->data(Qt::UserRole).toInt();
    QString text = item->text();
    int bracketPos = text.indexOf(" [");
    QString name = (bracketPos > 0) ? text.left(bracketPos) : text;
    emit groupChatRequested(groupId, name);
}

// ============================================================================
// 未读红点标记
// ============================================================================

void ContactWidget::setItemUnread(int id, bool isGroup, bool unread)
{
    // 确定在哪个列表中查找
    QListWidget *list = isGroup ? ui->groupList : ui->friendList;

    for (int i = 0; i < list->count(); ++i)
    {
        QListWidgetItem *item = list->item(i);
        if (item->data(Qt::UserRole).toInt() == id)
        {
            if (unread)
            {
                // 绘制一个 10x10 的红色圆点作为 item 图标
                QPixmap pix(10, 10);
                pix.fill(Qt::transparent);
                QPainter p(&pix);
                p.setRenderHint(QPainter::Antialiasing);
                p.setPen(Qt::NoPen);
                p.setBrush(QColor("#ff4444"));
                p.drawEllipse(1, 1, 8, 8);
                p.end();
                item->setIcon(QIcon(pix));
            }
            else
            {
                item->setIcon(QIcon());  // 空图标 = 清除红点
            }
            break;
        }
    }
}
