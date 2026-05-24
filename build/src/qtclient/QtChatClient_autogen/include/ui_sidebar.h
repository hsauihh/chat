/********************************************************************************
** Form generated from reading UI file 'sidebar.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SIDEBAR_H
#define UI_SIDEBAR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SidebarWidget
{
public:
    QVBoxLayout *mainLayout;
    QPushButton *avatarBtn;
    QWidget *userInfoPanel;
    QVBoxLayout *userInfoLayout;
    QLabel *userIdLabel;
    QLabel *userNameLabel;
    QPushButton *logoutBtn;
    QWidget *navSeparator;
    QPushButton *chatNavBtn;
    QPushButton *contactsNavBtn;
    QSpacerItem *bottomSpacer;
    QPushButton *clearHistoryBtn;

    void setupUi(QWidget *SidebarWidget)
    {
        if (SidebarWidget->objectName().isEmpty())
            SidebarWidget->setObjectName("SidebarWidget");
        SidebarWidget->setMinimumSize(QSize(60, 0));
        mainLayout = new QVBoxLayout(SidebarWidget);
        mainLayout->setSpacing(4);
        mainLayout->setObjectName("mainLayout");
        avatarBtn = new QPushButton(SidebarWidget);
        avatarBtn->setObjectName("avatarBtn");
        avatarBtn->setMinimumSize(QSize(44, 44));
        avatarBtn->setMaximumSize(QSize(44, 44));
        avatarBtn->setStyleSheet(QString::fromUtf8("border-radius: 22px; background-color: #ddd; font-size: 20px;"));
        avatarBtn->setFlat(true);

        mainLayout->addWidget(avatarBtn);

        userInfoPanel = new QWidget(SidebarWidget);
        userInfoPanel->setObjectName("userInfoPanel");
        userInfoPanel->setVisible(false);
        userInfoLayout = new QVBoxLayout(userInfoPanel);
        userInfoLayout->setObjectName("userInfoLayout");
        userInfoLayout->setContentsMargins(0, 0, 0, 0);
        userIdLabel = new QLabel(userInfoPanel);
        userIdLabel->setObjectName("userIdLabel");
        userIdLabel->setAlignment(Qt::AlignCenter);
        userIdLabel->setStyleSheet(QString::fromUtf8("font-size: 11px; color: #666;"));

        userInfoLayout->addWidget(userIdLabel);

        userNameLabel = new QLabel(userInfoPanel);
        userNameLabel->setObjectName("userNameLabel");
        userNameLabel->setAlignment(Qt::AlignCenter);
        userNameLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #333;"));

        userInfoLayout->addWidget(userNameLabel);

        logoutBtn = new QPushButton(userInfoPanel);
        logoutBtn->setObjectName("logoutBtn");
        logoutBtn->setStyleSheet(QString::fromUtf8("background-color: #f56c6c; color: white; border-radius: 4px; padding: 4px 8px; font-size: 12px;"));
        logoutBtn->setFlat(true);

        userInfoLayout->addWidget(logoutBtn);


        mainLayout->addWidget(userInfoPanel);

        navSeparator = new QWidget(SidebarWidget);
        navSeparator->setObjectName("navSeparator");
        navSeparator->setMaximumSize(QSize(16777215, 1));
        navSeparator->setStyleSheet(QString::fromUtf8("background-color: #ddd;"));

        mainLayout->addWidget(navSeparator);

        chatNavBtn = new QPushButton(SidebarWidget);
        chatNavBtn->setObjectName("chatNavBtn");
        chatNavBtn->setMinimumSize(QSize(44, 44));
        chatNavBtn->setMaximumSize(QSize(44, 44));
        chatNavBtn->setStyleSheet(QString::fromUtf8("border-radius: 8px; font-size: 18px;"));
        chatNavBtn->setFlat(true);

        mainLayout->addWidget(chatNavBtn);

        contactsNavBtn = new QPushButton(SidebarWidget);
        contactsNavBtn->setObjectName("contactsNavBtn");
        contactsNavBtn->setMinimumSize(QSize(44, 44));
        contactsNavBtn->setMaximumSize(QSize(44, 44));
        contactsNavBtn->setStyleSheet(QString::fromUtf8("border-radius: 8px; font-size: 18px;"));
        contactsNavBtn->setFlat(true);

        mainLayout->addWidget(contactsNavBtn);

        bottomSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        mainLayout->addItem(bottomSpacer);

        clearHistoryBtn = new QPushButton(SidebarWidget);
        clearHistoryBtn->setObjectName("clearHistoryBtn");
        clearHistoryBtn->setMinimumSize(QSize(44, 44));
        clearHistoryBtn->setMaximumSize(QSize(44, 44));
        clearHistoryBtn->setStyleSheet(QString::fromUtf8("border-radius: 8px; font-size: 18px;"));
        clearHistoryBtn->setFlat(true);

        mainLayout->addWidget(clearHistoryBtn);


        retranslateUi(SidebarWidget);

        QMetaObject::connectSlotsByName(SidebarWidget);
    } // setupUi

    void retranslateUi(QWidget *SidebarWidget)
    {
        avatarBtn->setText(QString());
        userIdLabel->setText(QCoreApplication::translate("SidebarWidget", "ID: -", nullptr));
        userNameLabel->setText(QCoreApplication::translate("SidebarWidget", "-", nullptr));
        logoutBtn->setText(QCoreApplication::translate("SidebarWidget", "\351\200\200\345\207\272\347\231\273\345\275\225", nullptr));
        chatNavBtn->setText(QString());
#if QT_CONFIG(tooltip)
        chatNavBtn->setToolTip(QCoreApplication::translate("SidebarWidget", "\350\201\212\345\244\251", nullptr));
#endif // QT_CONFIG(tooltip)
        contactsNavBtn->setText(QString());
#if QT_CONFIG(tooltip)
        contactsNavBtn->setToolTip(QCoreApplication::translate("SidebarWidget", "\350\201\224\347\263\273\344\272\272\347\256\241\347\220\206", nullptr));
#endif // QT_CONFIG(tooltip)
        clearHistoryBtn->setText(QCoreApplication::translate("SidebarWidget", "\360\237\227\221", nullptr));
#if QT_CONFIG(tooltip)
        clearHistoryBtn->setToolTip(QCoreApplication::translate("SidebarWidget", "\346\270\205\347\220\206\350\201\212\345\244\251\350\256\260\345\275\225", nullptr));
#endif // QT_CONFIG(tooltip)
        (void)SidebarWidget;
    } // retranslateUi

};

namespace Ui {
    class SidebarWidget: public Ui_SidebarWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SIDEBAR_H
