#include "infowidget.hpp"
#include "ui_infowidget.h"
#include <QPushButton>
#include <QLineEdit>

using namespace std;

InfoWidget::InfoWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::InfoWidget)
{
    ui->setupUi(this);

    connect(ui->addFriendBtn, &QPushButton::clicked, this, &InfoWidget::onAddFriendClicked);
    connect(ui->createGroupBtn, &QPushButton::clicked, this, &InfoWidget::onCreateGroupClicked);
    connect(ui->addGroupBtn, &QPushButton::clicked, this, &InfoWidget::onAddGroupClicked);
    connect(ui->logoutBtn, &QPushButton::clicked, this, &InfoWidget::onLogoutClicked);
}

InfoWidget::~InfoWidget()
{
    delete ui;
}

void InfoWidget::setUserInfo(int userId, const QString &userName)
{
    ui->userIdLabel->setText(QString("用户 ID: %1").arg(userId));
    ui->userNameLabel->setText(QString("用户名: %1").arg(userName));
}

void InfoWidget::onAddFriendClicked()
{
    QString text = ui->addFriendEdit->text().trimmed();
    if (text.isEmpty())
    {
        return;
    }
    int friendId = text.toInt();
    emit addFriendRequest(friendId);
    ui->addFriendEdit->clear();
}

void InfoWidget::onCreateGroupClicked()
{
    QString name = ui->createGroupNameEdit->text().trimmed();
    QString desc = ui->createGroupDescEdit->text().trimmed();
    if (name.isEmpty())
    {
        return;
    }
    emit createGroupRequest(name.toStdString(), desc.toStdString());
    ui->createGroupNameEdit->clear();
    ui->createGroupDescEdit->clear();
}

void InfoWidget::onAddGroupClicked()
{
    QString text = ui->addGroupEdit->text().trimmed();
    if (text.isEmpty())
    {
        return;
    }
    int groupId = text.toInt();
    emit addGroupRequest(groupId);
    ui->addGroupEdit->clear();
}

void InfoWidget::onLogoutClicked()
{
    emit logoutRequest();
}
