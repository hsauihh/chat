/********************************************************************************
** Form generated from reading UI file 'infowidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INFOWIDGET_H
#define UI_INFOWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InfoWidget
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *userGroup;
    QVBoxLayout *userLayout;
    QLabel *userIdLabel;
    QLabel *userNameLabel;
    QGroupBox *friendGroup;
    QVBoxLayout *friendLayout;
    QLineEdit *addFriendEdit;
    QPushButton *addFriendBtn;
    QGroupBox *createGroupBox;
    QVBoxLayout *createGroupLayout;
    QLineEdit *createGroupNameEdit;
    QLineEdit *createGroupDescEdit;
    QPushButton *createGroupBtn;
    QGroupBox *joinGroupBox;
    QVBoxLayout *joinGroupLayout;
    QLineEdit *addGroupEdit;
    QPushButton *addGroupBtn;
    QPushButton *logoutBtn;
    QSpacerItem *infoSpacer;

    void setupUi(QWidget *InfoWidget)
    {
        if (InfoWidget->objectName().isEmpty())
            InfoWidget->setObjectName("InfoWidget");
        InfoWidget->setMinimumSize(QSize(200, 0));
        InfoWidget->setMaximumSize(QSize(200, 16777215));
        verticalLayout = new QVBoxLayout(InfoWidget);
        verticalLayout->setObjectName("verticalLayout");
        userGroup = new QGroupBox(InfoWidget);
        userGroup->setObjectName("userGroup");
        userLayout = new QVBoxLayout(userGroup);
        userLayout->setObjectName("userLayout");
        userIdLabel = new QLabel(userGroup);
        userIdLabel->setObjectName("userIdLabel");

        userLayout->addWidget(userIdLabel);

        userNameLabel = new QLabel(userGroup);
        userNameLabel->setObjectName("userNameLabel");

        userLayout->addWidget(userNameLabel);


        verticalLayout->addWidget(userGroup);

        friendGroup = new QGroupBox(InfoWidget);
        friendGroup->setObjectName("friendGroup");
        friendLayout = new QVBoxLayout(friendGroup);
        friendLayout->setObjectName("friendLayout");
        addFriendEdit = new QLineEdit(friendGroup);
        addFriendEdit->setObjectName("addFriendEdit");

        friendLayout->addWidget(addFriendEdit);

        addFriendBtn = new QPushButton(friendGroup);
        addFriendBtn->setObjectName("addFriendBtn");

        friendLayout->addWidget(addFriendBtn);


        verticalLayout->addWidget(friendGroup);

        createGroupBox = new QGroupBox(InfoWidget);
        createGroupBox->setObjectName("createGroupBox");
        createGroupLayout = new QVBoxLayout(createGroupBox);
        createGroupLayout->setObjectName("createGroupLayout");
        createGroupNameEdit = new QLineEdit(createGroupBox);
        createGroupNameEdit->setObjectName("createGroupNameEdit");

        createGroupLayout->addWidget(createGroupNameEdit);

        createGroupDescEdit = new QLineEdit(createGroupBox);
        createGroupDescEdit->setObjectName("createGroupDescEdit");

        createGroupLayout->addWidget(createGroupDescEdit);

        createGroupBtn = new QPushButton(createGroupBox);
        createGroupBtn->setObjectName("createGroupBtn");

        createGroupLayout->addWidget(createGroupBtn);


        verticalLayout->addWidget(createGroupBox);

        joinGroupBox = new QGroupBox(InfoWidget);
        joinGroupBox->setObjectName("joinGroupBox");
        joinGroupLayout = new QVBoxLayout(joinGroupBox);
        joinGroupLayout->setObjectName("joinGroupLayout");
        addGroupEdit = new QLineEdit(joinGroupBox);
        addGroupEdit->setObjectName("addGroupEdit");

        joinGroupLayout->addWidget(addGroupEdit);

        addGroupBtn = new QPushButton(joinGroupBox);
        addGroupBtn->setObjectName("addGroupBtn");

        joinGroupLayout->addWidget(addGroupBtn);


        verticalLayout->addWidget(joinGroupBox);

        logoutBtn = new QPushButton(InfoWidget);
        logoutBtn->setObjectName("logoutBtn");
        logoutBtn->setStyleSheet(QString::fromUtf8("background-color: #f56c6c; color: white; padding: 8px;"));

        verticalLayout->addWidget(logoutBtn);

        infoSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        verticalLayout->addItem(infoSpacer);


        retranslateUi(InfoWidget);

        QMetaObject::connectSlotsByName(InfoWidget);
    } // setupUi

    void retranslateUi(QWidget *InfoWidget)
    {
        userGroup->setTitle(QCoreApplication::translate("InfoWidget", "\345\275\223\345\211\215\347\224\250\346\210\267", nullptr));
        userIdLabel->setText(QCoreApplication::translate("InfoWidget", "\347\224\250\346\210\267 ID: -", nullptr));
        userNameLabel->setText(QCoreApplication::translate("InfoWidget", "\347\224\250\346\210\267\345\220\215: -", nullptr));
        friendGroup->setTitle(QCoreApplication::translate("InfoWidget", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        addFriendEdit->setPlaceholderText(QCoreApplication::translate("InfoWidget", "\350\276\223\345\205\245\345\245\275\345\217\213 ID", nullptr));
        addFriendBtn->setText(QCoreApplication::translate("InfoWidget", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        createGroupBox->setTitle(QCoreApplication::translate("InfoWidget", "\345\210\233\345\273\272\347\276\244\347\273\204", nullptr));
        createGroupNameEdit->setPlaceholderText(QCoreApplication::translate("InfoWidget", "\347\276\244\347\273\204\345\220\215\347\247\260", nullptr));
        createGroupDescEdit->setPlaceholderText(QCoreApplication::translate("InfoWidget", "\347\276\244\347\273\204\346\217\217\350\277\260", nullptr));
        createGroupBtn->setText(QCoreApplication::translate("InfoWidget", "\345\210\233\345\273\272\347\276\244\347\273\204", nullptr));
        joinGroupBox->setTitle(QCoreApplication::translate("InfoWidget", "\345\212\240\345\205\245\347\276\244\347\273\204", nullptr));
        addGroupEdit->setPlaceholderText(QCoreApplication::translate("InfoWidget", "\350\276\223\345\205\245\347\276\244\347\273\204 ID", nullptr));
        addGroupBtn->setText(QCoreApplication::translate("InfoWidget", "\345\212\240\345\205\245\347\276\244\347\273\204", nullptr));
        logoutBtn->setText(QCoreApplication::translate("InfoWidget", "\351\200\200\345\207\272\347\231\273\345\275\225", nullptr));
        (void)InfoWidget;
    } // retranslateUi

};

namespace Ui {
    class InfoWidget: public Ui_InfoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INFOWIDGET_H
