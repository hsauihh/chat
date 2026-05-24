/********************************************************************************
** Form generated from reading UI file 'contactsmanage.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTSMANAGE_H
#define UI_CONTACTSMANAGE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactsManageWidget
{
public:
    QVBoxLayout *mainLayout;
    QLabel *titleLabel;
    QGroupBox *friendGroup;
    QHBoxLayout *friendLayout;
    QLineEdit *addFriendEdit;
    QPushButton *addFriendBtn;
    QGroupBox *createGroupBox;
    QVBoxLayout *createGroupLayout;
    QLineEdit *createGroupNameEdit;
    QLineEdit *createGroupDescEdit;
    QPushButton *createGroupBtn;
    QGroupBox *joinGroupBox;
    QHBoxLayout *joinGroupLayout;
    QLineEdit *addGroupEdit;
    QPushButton *addGroupBtn;
    QSpacerItem *bottomSpacer;

    void setupUi(QWidget *ContactsManageWidget)
    {
        if (ContactsManageWidget->objectName().isEmpty())
            ContactsManageWidget->setObjectName("ContactsManageWidget");
        mainLayout = new QVBoxLayout(ContactsManageWidget);
        mainLayout->setObjectName("mainLayout");
        titleLabel = new QLabel(ContactsManageWidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 18px; font-weight: bold; padding: 8px;"));

        mainLayout->addWidget(titleLabel);

        friendGroup = new QGroupBox(ContactsManageWidget);
        friendGroup->setObjectName("friendGroup");
        friendLayout = new QHBoxLayout(friendGroup);
        friendLayout->setObjectName("friendLayout");
        addFriendEdit = new QLineEdit(friendGroup);
        addFriendEdit->setObjectName("addFriendEdit");

        friendLayout->addWidget(addFriendEdit);

        addFriendBtn = new QPushButton(friendGroup);
        addFriendBtn->setObjectName("addFriendBtn");
        addFriendBtn->setStyleSheet(QString::fromUtf8("background-color: #07c160; color: white; padding: 6px 16px; border-radius: 4px;"));

        friendLayout->addWidget(addFriendBtn);


        mainLayout->addWidget(friendGroup);

        createGroupBox = new QGroupBox(ContactsManageWidget);
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
        createGroupBtn->setStyleSheet(QString::fromUtf8("background-color: #07c160; color: white; padding: 6px 16px; border-radius: 4px;"));

        createGroupLayout->addWidget(createGroupBtn);


        mainLayout->addWidget(createGroupBox);

        joinGroupBox = new QGroupBox(ContactsManageWidget);
        joinGroupBox->setObjectName("joinGroupBox");
        joinGroupLayout = new QHBoxLayout(joinGroupBox);
        joinGroupLayout->setObjectName("joinGroupLayout");
        addGroupEdit = new QLineEdit(joinGroupBox);
        addGroupEdit->setObjectName("addGroupEdit");

        joinGroupLayout->addWidget(addGroupEdit);

        addGroupBtn = new QPushButton(joinGroupBox);
        addGroupBtn->setObjectName("addGroupBtn");
        addGroupBtn->setStyleSheet(QString::fromUtf8("background-color: #07c160; color: white; padding: 6px 16px; border-radius: 4px;"));

        joinGroupLayout->addWidget(addGroupBtn);


        mainLayout->addWidget(joinGroupBox);

        bottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(bottomSpacer);


        retranslateUi(ContactsManageWidget);

        QMetaObject::connectSlotsByName(ContactsManageWidget);
    } // setupUi

    void retranslateUi(QWidget *ContactsManageWidget)
    {
        titleLabel->setText(QCoreApplication::translate("ContactsManageWidget", "\350\201\224\347\263\273\344\272\272\347\256\241\347\220\206", nullptr));
        friendGroup->setTitle(QCoreApplication::translate("ContactsManageWidget", "\346\267\273\345\212\240\345\245\275\345\217\213", nullptr));
        addFriendEdit->setPlaceholderText(QCoreApplication::translate("ContactsManageWidget", "\350\276\223\345\205\245\345\245\275\345\217\213 ID", nullptr));
        addFriendBtn->setText(QCoreApplication::translate("ContactsManageWidget", "\346\267\273\345\212\240", nullptr));
        createGroupBox->setTitle(QCoreApplication::translate("ContactsManageWidget", "\345\210\233\345\273\272\347\276\244\347\273\204", nullptr));
        createGroupNameEdit->setPlaceholderText(QCoreApplication::translate("ContactsManageWidget", "\347\276\244\347\273\204\345\220\215\347\247\260", nullptr));
        createGroupDescEdit->setPlaceholderText(QCoreApplication::translate("ContactsManageWidget", "\347\276\244\347\273\204\346\217\217\350\277\260", nullptr));
        createGroupBtn->setText(QCoreApplication::translate("ContactsManageWidget", "\345\210\233\345\273\272\347\276\244\347\273\204", nullptr));
        joinGroupBox->setTitle(QCoreApplication::translate("ContactsManageWidget", "\345\212\240\345\205\245\347\276\244\347\273\204", nullptr));
        addGroupEdit->setPlaceholderText(QCoreApplication::translate("ContactsManageWidget", "\350\276\223\345\205\245\347\276\244\347\273\204 ID", nullptr));
        addGroupBtn->setText(QCoreApplication::translate("ContactsManageWidget", "\345\212\240\345\205\245", nullptr));
        (void)ContactsManageWidget;
    } // retranslateUi

};

namespace Ui {
    class ContactsManageWidget: public Ui_ContactsManageWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTSMANAGE_H
