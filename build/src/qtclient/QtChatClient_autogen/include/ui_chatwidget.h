/********************************************************************************
** Form generated from reading UI file 'chatwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHATWIDGET_H
#define UI_CHATWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ChatWidget
{
public:
    QVBoxLayout *mainLayout;
    QWidget *topBar;
    QHBoxLayout *topBarLayout;
    QPushButton *avatarPlaceholder;
    QVBoxLayout *contactInfoLayout;
    QLabel *contactNameLabel;
    QLabel *statusLabel;
    QListWidget *messageList;
    QWidget *bottomBar;
    QHBoxLayout *bottomBarLayout;
    QPushButton *emojiBtn;
    QPushButton *attachBtn;
    QPlainTextEdit *inputEdit;
    QPushButton *sendBtn;

    void setupUi(QWidget *ChatWidget)
    {
        if (ChatWidget->objectName().isEmpty())
            ChatWidget->setObjectName("ChatWidget");
        mainLayout = new QVBoxLayout(ChatWidget);
        mainLayout->setSpacing(0);
        mainLayout->setObjectName("mainLayout");
        mainLayout->setContentsMargins(0, -1, 0, -1);
        topBar = new QWidget(ChatWidget);
        topBar->setObjectName("topBar");
        topBar->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0; border-bottom: 1px solid #ddd;"));
        topBar->setMinimumSize(QSize(0, 56));
        topBar->setMaximumSize(QSize(16777215, 56));
        topBarLayout = new QHBoxLayout(topBar);
        topBarLayout->setObjectName("topBarLayout");
        topBarLayout->setContentsMargins(0, 0, 0, 0);
        avatarPlaceholder = new QPushButton(topBar);
        avatarPlaceholder->setObjectName("avatarPlaceholder");
        avatarPlaceholder->setMinimumSize(QSize(40, 40));
        avatarPlaceholder->setMaximumSize(QSize(40, 40));
        avatarPlaceholder->setStyleSheet(QString::fromUtf8("border-radius: 20px; background-color: #ddd; font-size: 18px;"));
        avatarPlaceholder->setFlat(true);

        topBarLayout->addWidget(avatarPlaceholder);

        contactInfoLayout = new QVBoxLayout();
        contactInfoLayout->setObjectName("contactInfoLayout");
        contactNameLabel = new QLabel(topBar);
        contactNameLabel->setObjectName("contactNameLabel");
        contactNameLabel->setStyleSheet(QString::fromUtf8("font-size: 15px; font-weight: bold; color: #333;"));

        contactInfoLayout->addWidget(contactNameLabel);

        statusLabel = new QLabel(topBar);
        statusLabel->setObjectName("statusLabel");
        statusLabel->setStyleSheet(QString::fromUtf8("font-size: 12px; color: #999;"));

        contactInfoLayout->addWidget(statusLabel);


        topBarLayout->addLayout(contactInfoLayout);


        mainLayout->addWidget(topBar);

        messageList = new QListWidget(ChatWidget);
        messageList->setObjectName("messageList");
        messageList->setStyleSheet(QString::fromUtf8("background-color: #f5f5f5; border: none;"));
        messageList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        messageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        messageList->setSpacing(4);

        mainLayout->addWidget(messageList);

        bottomBar = new QWidget(ChatWidget);
        bottomBar->setObjectName("bottomBar");
        bottomBar->setStyleSheet(QString::fromUtf8("background-color: #f0f0f0; border-top: 1px solid #ddd;"));
        bottomBar->setMinimumSize(QSize(0, 50));
        bottomBarLayout = new QHBoxLayout(bottomBar);
        bottomBarLayout->setObjectName("bottomBarLayout");
        bottomBarLayout->setContentsMargins(0, 0, 0, 0);
        emojiBtn = new QPushButton(bottomBar);
        emojiBtn->setObjectName("emojiBtn");
        emojiBtn->setMinimumSize(QSize(32, 32));
        emojiBtn->setMaximumSize(QSize(32, 32));
        emojiBtn->setFlat(true);

        bottomBarLayout->addWidget(emojiBtn);

        attachBtn = new QPushButton(bottomBar);
        attachBtn->setObjectName("attachBtn");
        attachBtn->setMinimumSize(QSize(32, 32));
        attachBtn->setMaximumSize(QSize(32, 32));
        attachBtn->setFlat(true);

        bottomBarLayout->addWidget(attachBtn);

        inputEdit = new QPlainTextEdit(bottomBar);
        inputEdit->setObjectName("inputEdit");
        inputEdit->setMaximumSize(QSize(16777215, 60));
        inputEdit->setStyleSheet(QString::fromUtf8("border: 1px solid #ddd; border-radius: 4px; padding: 4px;"));
        QSizePolicy sizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(inputEdit->sizePolicy().hasHeightForWidth());
        inputEdit->setSizePolicy(sizePolicy);

        bottomBarLayout->addWidget(inputEdit);

        sendBtn = new QPushButton(bottomBar);
        sendBtn->setObjectName("sendBtn");
        sendBtn->setMinimumSize(QSize(64, 0));
        sendBtn->setStyleSheet(QString::fromUtf8("background-color: #07c160; color: white; border-radius: 4px; padding: 6px 16px; font-size: 14px;"));

        bottomBarLayout->addWidget(sendBtn);


        mainLayout->addWidget(bottomBar);


        retranslateUi(ChatWidget);

        QMetaObject::connectSlotsByName(ChatWidget);
    } // setupUi

    void retranslateUi(QWidget *ChatWidget)
    {
        avatarPlaceholder->setText(QCoreApplication::translate("ChatWidget", "\360\237\221\244", nullptr));
        contactNameLabel->setText(QCoreApplication::translate("ChatWidget", "\350\257\267\351\200\211\346\213\251\350\201\224\347\263\273\344\272\272", nullptr));
        statusLabel->setText(QString());
        emojiBtn->setText(QCoreApplication::translate("ChatWidget", "\360\237\230\212", nullptr));
        attachBtn->setText(QCoreApplication::translate("ChatWidget", "\360\237\223\216", nullptr));
        inputEdit->setPlaceholderText(QCoreApplication::translate("ChatWidget", "\350\276\223\345\205\245\346\266\210\346\201\257...", nullptr));
        sendBtn->setText(QCoreApplication::translate("ChatWidget", "\345\217\221\351\200\201", nullptr));
        (void)ChatWidget;
    } // retranslateUi

};

namespace Ui {
    class ChatWidget: public Ui_ChatWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHATWIDGET_H
