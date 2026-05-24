/*
 * ============================================================================
 * ContactsManageWidget 实现 — 联系人管理页面
 * ============================================================================
 *
 * 【设计模式】表单收集 → 校验 → emit 信号，由 MainWidget 处理业务逻辑。
 * 子 Widget 只负责 UI，不关心网络请求的具体实现。
 */

#include "contactsmanagewidget.hpp"
#include "ui_contactsmanage.h"
#include <QPushButton>
#include <QLineEdit>

ContactsManageWidget::ContactsManageWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::ContactsManageWidget)
{
    ui->setupUi(this);

    connect(ui->addFriendBtn, &QPushButton::clicked, this, &ContactsManageWidget::onAddFriendClicked);
    connect(ui->createGroupBtn, &QPushButton::clicked, this, &ContactsManageWidget::onCreateGroupClicked);
    connect(ui->addGroupBtn, &QPushButton::clicked, this, &ContactsManageWidget::onAddGroupClicked);
}

ContactsManageWidget::~ContactsManageWidget()
{
    delete ui;
}

void ContactsManageWidget::onAddFriendClicked()
{
    QString text = ui->addFriendEdit->text().trimmed();
    if (text.isEmpty()) return;
    int friendId = text.toInt();
    emit addFriendRequest(friendId);
    ui->addFriendEdit->clear();  // 清空输入框，方便连续操作
}

void ContactsManageWidget::onCreateGroupClicked()
{
    QString name = ui->createGroupNameEdit->text().trimmed();
    QString desc = ui->createGroupDescEdit->text().trimmed();
    if (name.isEmpty()) return;
    emit createGroupRequest(name.toStdString(), desc.toStdString());
    ui->createGroupNameEdit->clear();
    ui->createGroupDescEdit->clear();
}

void ContactsManageWidget::onAddGroupClicked()
{
    QString text = ui->addGroupEdit->text().trimmed();
    if (text.isEmpty()) return;
    int groupId = text.toInt();
    emit addGroupRequest(groupId);
    ui->addGroupEdit->clear();
}
