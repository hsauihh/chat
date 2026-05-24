/*
 * ============================================================================
 * LoginWidget 实现 — 登录/注册页面
 * ============================================================================
 *
 * 【Qt 概念】本文件展示了 QStackedWidget 的页面切换模式：
 *   ui->stack->setCurrentIndex(0)  → 显示登录页
 *   ui->stack->setCurrentIndex(1)  → 显示注册页
 */

#include "loginwidget.hpp"
#include "ui_loginwidget.h"
#include <QPushButton>
#include <QLineEdit>

using namespace std;

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWidget)
{
    ui->setupUi(this);

    // 默认显示登录页（index 0）
    ui->stack->setCurrentIndex(0);

    // 连接各个按钮的点击信号到对应的槽函数
    connect(ui->loginBtn, &QPushButton::clicked, this, &LoginWidget::onLoginClicked);
    connect(ui->goRegisterBtn, &QPushButton::clicked, this, &LoginWidget::switchToRegister);
    connect(ui->regBtn, &QPushButton::clicked, this, &LoginWidget::onRegisterClicked);
    connect(ui->goLoginBtn, &QPushButton::clicked, this, &LoginWidget::switchToLogin);
}

LoginWidget::~LoginWidget()
{
    delete ui;
}

// ============================================================================
// 槽函数
// ============================================================================

void LoginWidget::onLoginClicked()
{
    // 获取用户输入的 ID 和密码
    // QLineEdit::text() 返回当前文本，trimmed() 在业务层处理前做空格清理
    QString idText = ui->loginIdEdit->text();
    QString pwdText = ui->loginPwdEdit->text();

    if (idText.isEmpty() || pwdText.isEmpty())
    {
        return;
    }

    int id = idText.toInt();
    string password = pwdText.toStdString();
    emit loginRequest(id, password);
}

void LoginWidget::onRegisterClicked()
{
    QString nameText = ui->regNameEdit->text();
    QString pwdText = ui->regPwdEdit->text();

    if (nameText.isEmpty() || pwdText.isEmpty())
    {
        return;
    }

    string name = nameText.toStdString();
    string password = pwdText.toStdString();
    emit registerRequest(name, password);
}

void LoginWidget::switchToLogin()
{
    ui->stack->setCurrentIndex(0);  // QStackedWidget 切换到登录页
}

void LoginWidget::switchToRegister()
{
    ui->stack->setCurrentIndex(1);  // QStackedWidget 切换到注册页
}
