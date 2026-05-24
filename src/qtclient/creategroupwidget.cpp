/*
 * ============================================================================
 * CreateGroupWidget 实现 — 创建/加入群组对话框
 * ============================================================================
 *
 * 【Qt 概念】QDialog 的使用模式：
 *   auto *dlg = new CreateGroupWidget(this);
 *   if (dlg->exec() == QDialog::Accepted)  { // 模态阻塞，直到用户关闭
 *       string name = dlg->getGroupName();  // 用户确认后才读取数据
 *   }
 *   dlg->deleteLater();
 *
 * 【注意】本类目前是独立组件，主界面用 ContactsManageWidget 管理群组操作。
 * 保留此组件作为 QDialog 模式的参考示例。
 */

#include "creategroupwidget.hpp"
#include "ui_creategroupwidget.h"
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>

using namespace std;

CreateGroupWidget::CreateGroupWidget(QWidget *parent)
    : QDialog(parent), ui(new Ui::CreateGroupWidget), _isCreateMode(true)
{
    ui->setupUi(this);

    connect(ui->confirmBtn, &QPushButton::clicked, this, &CreateGroupWidget::onConfirmClicked);
    connect(ui->cancelBtn, &QPushButton::clicked, this, &QDialog::reject);  // reject() 返回 QDialog::Rejected

    setCreateMode();
}

CreateGroupWidget::~CreateGroupWidget()
{
    delete ui;
}

string CreateGroupWidget::getGroupName() const
{
    return ui->groupNameEdit->text().toStdString();
}

string CreateGroupWidget::getGroupDesc() const
{
    return ui->groupDescEdit->text().toStdString();
}

int CreateGroupWidget::getGroupId() const
{
    return ui->groupIdEdit->text().toInt();
}

void CreateGroupWidget::setCreateMode()
{
    _isCreateMode = true;
    ui->titleLabel->setText("创建群组");
    setWindowTitle("创建群组");
    // 创建模式：显示名称和描述，隐藏 ID 输入
    ui->groupNameEdit->setVisible(true);
    ui->groupDescEdit->setVisible(true);
    ui->groupIdEdit->setVisible(false);
}

void CreateGroupWidget::setJoinMode()
{
    _isCreateMode = false;
    ui->titleLabel->setText("加入群组");
    setWindowTitle("加入群组");
    // 加入模式：只显示 ID 输入
    ui->groupNameEdit->setVisible(false);
    ui->groupDescEdit->setVisible(false);
    ui->groupIdEdit->setVisible(true);
}

void CreateGroupWidget::onConfirmClicked()
{
    if (_isCreateMode)
    {
        if (ui->groupNameEdit->text().trimmed().isEmpty())
            return;
    }
    else
    {
        if (ui->groupIdEdit->text().trimmed().isEmpty())
            return;
    }
    accept();  // 返回 QDialog::Accepted
}
