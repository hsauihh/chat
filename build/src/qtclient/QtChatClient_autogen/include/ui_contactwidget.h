/********************************************************************************
** Form generated from reading UI file 'contactwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONTACTWIDGET_H
#define UI_CONTACTWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ContactWidget
{
public:
    QVBoxLayout *mainLayout;
    QLineEdit *searchEdit;
    QWidget *friendHeader;
    QHBoxLayout *friendHeaderLayout;
    QLabel *friendHeaderLabel;
    QSpacerItem *friendHeaderSpacer;
    QLabel *friendCountLabel;
    QListWidget *friendList;
    QWidget *groupHeader;
    QHBoxLayout *groupHeaderLayout;
    QLabel *groupHeaderLabel;
    QSpacerItem *groupHeaderSpacer;
    QLabel *groupCountLabel;
    QListWidget *groupList;

    void setupUi(QWidget *ContactWidget)
    {
        if (ContactWidget->objectName().isEmpty())
            ContactWidget->setObjectName("ContactWidget");
        mainLayout = new QVBoxLayout(ContactWidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, -1, 0, -1);
        searchEdit = new QLineEdit(ContactWidget);
        searchEdit->setObjectName("searchEdit");
        searchEdit->setStyleSheet(QString::fromUtf8("border: 1px solid #ddd; border-radius: 4px; padding: 8px; margin: 8px;"));

        mainLayout->addWidget(searchEdit);

        friendHeader = new QWidget(ContactWidget);
        friendHeader->setObjectName("friendHeader");
        friendHeader->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0;"));
        friendHeaderLayout = new QHBoxLayout(friendHeader);
        friendHeaderLayout->setObjectName("friendHeaderLayout");
        friendHeaderLayout->setContentsMargins(12, 0, 0, 0);
        friendHeaderLabel = new QLabel(friendHeader);
        friendHeaderLabel->setObjectName("friendHeaderLabel");
        friendHeaderLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; font-weight: bold; color: #666;"));

        friendHeaderLayout->addWidget(friendHeaderLabel);

        friendHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        friendHeaderLayout->addItem(friendHeaderSpacer);

        friendCountLabel = new QLabel(friendHeader);
        friendCountLabel->setObjectName("friendCountLabel");
        friendCountLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #999; margin-right: 8px;"));

        friendHeaderLayout->addWidget(friendCountLabel);


        mainLayout->addWidget(friendHeader);

        friendList = new QListWidget(ContactWidget);
        friendList->setObjectName("friendList");
        friendList->setStyleSheet(QString::fromUtf8("QListWidget::item { padding: 10px 12px; } border: none;"));

        mainLayout->addWidget(friendList);

        groupHeader = new QWidget(ContactWidget);
        groupHeader->setObjectName("groupHeader");
        groupHeader->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0;"));
        groupHeaderLayout = new QHBoxLayout(groupHeader);
        groupHeaderLayout->setObjectName("groupHeaderLayout");
        groupHeaderLayout->setContentsMargins(12, 0, 0, 0);
        groupHeaderLabel = new QLabel(groupHeader);
        groupHeaderLabel->setObjectName("groupHeaderLabel");
        groupHeaderLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; font-weight: bold; color: #666;"));

        groupHeaderLayout->addWidget(groupHeaderLabel);

        groupHeaderSpacer = new QSpacerItem(0, 0, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        groupHeaderLayout->addItem(groupHeaderSpacer);

        groupCountLabel = new QLabel(groupHeader);
        groupCountLabel->setObjectName("groupCountLabel");
        groupCountLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #999; margin-right: 8px;"));

        groupHeaderLayout->addWidget(groupCountLabel);


        mainLayout->addWidget(groupHeader);

        groupList = new QListWidget(ContactWidget);
        groupList->setObjectName("groupList");
        groupList->setStyleSheet(QString::fromUtf8("QListWidget::item { padding: 10px 12px; } border: none;"));

        mainLayout->addWidget(groupList);


        retranslateUi(ContactWidget);

        QMetaObject::connectSlotsByName(ContactWidget);
    } // setupUi

    void retranslateUi(QWidget *ContactWidget)
    {
        searchEdit->setPlaceholderText(QCoreApplication::translate("ContactWidget", "\346\220\234\347\264\242\345\245\275\345\217\213\346\210\226\347\276\244\347\273\204...", nullptr));
        friendHeaderLabel->setText(QCoreApplication::translate("ContactWidget", "\345\245\275\345\217\213", nullptr));
        friendCountLabel->setText(QCoreApplication::translate("ContactWidget", "0\344\272\272", nullptr));
        groupHeaderLabel->setText(QCoreApplication::translate("ContactWidget", "\347\276\244\347\273\204", nullptr));
        groupCountLabel->setText(QCoreApplication::translate("ContactWidget", "0\344\270\252", nullptr));
        (void)ContactWidget;
    } // retranslateUi

};

namespace Ui {
    class ContactWidget: public Ui_ContactWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONTACTWIDGET_H
