/********************************************************************************
** Form generated from reading UI file 'loginwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGINWIDGET_H
#define UI_LOGINWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LoginWidget
{
public:
    QVBoxLayout *verticalLayout;
    QStackedWidget *stack;
    QWidget *loginPage;
    QVBoxLayout *loginPageLayout;
    QLabel *loginTitleLabel;
    QLabel *loginIdLabel;
    QLineEdit *loginIdEdit;
    QLabel *loginPwdLabel;
    QLineEdit *loginPwdEdit;
    QPushButton *loginBtn;
    QPushButton *goRegisterBtn;
    QSpacerItem *loginSpacer;
    QWidget *registerPage;
    QVBoxLayout *registerPageLayout;
    QLabel *regTitleLabel;
    QLabel *regNameLabel;
    QLineEdit *regNameEdit;
    QLabel *regPwdLabel;
    QLineEdit *regPwdEdit;
    QPushButton *regBtn;
    QPushButton *goLoginBtn;
    QSpacerItem *registerSpacer;

    void setupUi(QWidget *LoginWidget)
    {
        if (LoginWidget->objectName().isEmpty())
            LoginWidget->setObjectName("LoginWidget");
        LoginWidget->resize(400, 300);
        verticalLayout = new QVBoxLayout(LoginWidget);
        verticalLayout->setObjectName("verticalLayout");
        stack = new QStackedWidget(LoginWidget);
        stack->setObjectName("stack");
        loginPage = new QWidget();
        loginPage->setObjectName("loginPage");
        loginPageLayout = new QVBoxLayout(loginPage);
        loginPageLayout->setObjectName("loginPageLayout");
        loginTitleLabel = new QLabel(loginPage);
        loginTitleLabel->setObjectName("loginTitleLabel");
        loginTitleLabel->setAlignment(Qt::AlignCenter);
        loginTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; margin-bottom: 10px;"));

        loginPageLayout->addWidget(loginTitleLabel);

        loginIdLabel = new QLabel(loginPage);
        loginIdLabel->setObjectName("loginIdLabel");

        loginPageLayout->addWidget(loginIdLabel);

        loginIdEdit = new QLineEdit(loginPage);
        loginIdEdit->setObjectName("loginIdEdit");

        loginPageLayout->addWidget(loginIdEdit);

        loginPwdLabel = new QLabel(loginPage);
        loginPwdLabel->setObjectName("loginPwdLabel");

        loginPageLayout->addWidget(loginPwdLabel);

        loginPwdEdit = new QLineEdit(loginPage);
        loginPwdEdit->setObjectName("loginPwdEdit");
        loginPwdEdit->setEchoMode(QLineEdit::Password);

        loginPageLayout->addWidget(loginPwdEdit);

        loginBtn = new QPushButton(loginPage);
        loginBtn->setObjectName("loginBtn");

        loginPageLayout->addWidget(loginBtn);

        goRegisterBtn = new QPushButton(loginPage);
        goRegisterBtn->setObjectName("goRegisterBtn");

        loginPageLayout->addWidget(goRegisterBtn);

        loginSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        loginPageLayout->addItem(loginSpacer);

        stack->addWidget(loginPage);
        registerPage = new QWidget();
        registerPage->setObjectName("registerPage");
        registerPageLayout = new QVBoxLayout(registerPage);
        registerPageLayout->setObjectName("registerPageLayout");
        regTitleLabel = new QLabel(registerPage);
        regTitleLabel->setObjectName("regTitleLabel");
        regTitleLabel->setAlignment(Qt::AlignCenter);
        regTitleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; margin-bottom: 10px;"));

        registerPageLayout->addWidget(regTitleLabel);

        regNameLabel = new QLabel(registerPage);
        regNameLabel->setObjectName("regNameLabel");

        registerPageLayout->addWidget(regNameLabel);

        regNameEdit = new QLineEdit(registerPage);
        regNameEdit->setObjectName("regNameEdit");

        registerPageLayout->addWidget(regNameEdit);

        regPwdLabel = new QLabel(registerPage);
        regPwdLabel->setObjectName("regPwdLabel");

        registerPageLayout->addWidget(regPwdLabel);

        regPwdEdit = new QLineEdit(registerPage);
        regPwdEdit->setObjectName("regPwdEdit");
        regPwdEdit->setEchoMode(QLineEdit::Password);

        registerPageLayout->addWidget(regPwdEdit);

        regBtn = new QPushButton(registerPage);
        regBtn->setObjectName("regBtn");

        registerPageLayout->addWidget(regBtn);

        goLoginBtn = new QPushButton(registerPage);
        goLoginBtn->setObjectName("goLoginBtn");

        registerPageLayout->addWidget(goLoginBtn);

        registerSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        registerPageLayout->addItem(registerSpacer);

        stack->addWidget(registerPage);

        verticalLayout->addWidget(stack);


        retranslateUi(LoginWidget);

        QMetaObject::connectSlotsByName(LoginWidget);
    } // setupUi

    void retranslateUi(QWidget *LoginWidget)
    {
        LoginWidget->setWindowTitle(QCoreApplication::translate("LoginWidget", "Chat - \347\231\273\345\275\225", nullptr));
        loginTitleLabel->setText(QCoreApplication::translate("LoginWidget", "\347\224\250\346\210\267\347\231\273\345\275\225", nullptr));
        loginIdLabel->setText(QCoreApplication::translate("LoginWidget", "\347\224\250\346\210\267 ID:", nullptr));
        loginIdEdit->setPlaceholderText(QCoreApplication::translate("LoginWidget", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267 ID", nullptr));
        loginPwdLabel->setText(QCoreApplication::translate("LoginWidget", "\345\257\206\347\240\201:", nullptr));
        loginPwdEdit->setPlaceholderText(QCoreApplication::translate("LoginWidget", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        loginBtn->setText(QCoreApplication::translate("LoginWidget", "\347\231\273\345\275\225", nullptr));
        goRegisterBtn->setText(QCoreApplication::translate("LoginWidget", "\346\263\250\345\206\214\346\226\260\350\264\246\345\217\267", nullptr));
        regTitleLabel->setText(QCoreApplication::translate("LoginWidget", "\347\224\250\346\210\267\346\263\250\345\206\214", nullptr));
        regNameLabel->setText(QCoreApplication::translate("LoginWidget", "\347\224\250\346\210\267\345\220\215:", nullptr));
        regNameEdit->setPlaceholderText(QCoreApplication::translate("LoginWidget", "\350\257\267\350\276\223\345\205\245\347\224\250\346\210\267\345\220\215", nullptr));
        regPwdLabel->setText(QCoreApplication::translate("LoginWidget", "\345\257\206\347\240\201:", nullptr));
        regPwdEdit->setPlaceholderText(QCoreApplication::translate("LoginWidget", "\350\257\267\350\276\223\345\205\245\345\257\206\347\240\201", nullptr));
        regBtn->setText(QCoreApplication::translate("LoginWidget", "\346\263\250\345\206\214", nullptr));
        goLoginBtn->setText(QCoreApplication::translate("LoginWidget", "\350\277\224\345\233\236\347\231\273\345\275\225", nullptr));
    } // retranslateUi

};

namespace Ui {
    class LoginWidget: public Ui_LoginWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGINWIDGET_H
