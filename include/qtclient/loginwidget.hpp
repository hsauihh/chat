#ifndef LOGINWIDGET_H
#define LOGINWIDGET_H

#include <QWidget>

using namespace std;

namespace Ui {
    class LoginWidget;
}

/*
 * ============================================================================
 * LoginWidget — 登录/注册页面
 * ============================================================================
 *
 * 【功能】
 *   1. 登录页：输入用户 ID + 密码 → 点击登录 → emit loginRequest 信号
 *   2. 注册页：输入用户名 + 密码 → 点击注册 → emit registerRequest 信号
 *   3. 两个页面之间通过"去注册"/"去登录"按钮切换
 *
 * 【Qt 概念】QStackedWidget (ui->stack) 实现登录/注册页切换：
 *   - index 0: 登录页（ID + 密码 + 登录按钮 + 去注册链接）
 *   - index 1: 注册页（名称 + 密码 + 注册按钮 + 去登录链接）
 *
 * 【信号设计】LoginWidget 不直接发送网络请求，只通过信号通知 MainWidget。
 * 这样做的好处：LoginWidget 不需要知道网络层的存在，方便单元测试。
 */
class LoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();

signals:
    // 用户在登录页点击登录按钮（id: 用户 ID, password: 密码）
    void loginRequest(int id, const string &password);
    // 用户在注册页点击注册按钮（name: 用户名, password: 密码）
    void registerRequest(const string &name, const string &password);

private slots:
    void onLoginClicked();       // 登录按钮 → 校验输入 → emit loginRequest
    void onRegisterClicked();    // 注册按钮 → 校验输入 → emit registerRequest
    void switchToLogin();        // 切换到登录页 (stack index 0)
    void switchToRegister();     // 切换到注册页 (stack index 1)

private:
    Ui::LoginWidget *ui;
};

#endif
