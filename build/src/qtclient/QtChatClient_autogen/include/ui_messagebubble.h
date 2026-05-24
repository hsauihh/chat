/********************************************************************************
** Form generated from reading UI file 'messagebubble.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESSAGEBUBBLE_H
#define UI_MESSAGEBUBBLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MessageBubble
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *timeLabel;
    QHBoxLayout *bubbleLayout;
    QSpacerItem *leftSpacer;
    QLabel *bubbleLabel;
    QLabel *nameLabel;
    QSpacerItem *rightSpacer;

    void setupUi(QWidget *MessageBubble)
    {
        if (MessageBubble->objectName().isEmpty())
            MessageBubble->setObjectName("MessageBubble");
        verticalLayout = new QVBoxLayout(MessageBubble);
        verticalLayout->setSpacing(2);
        verticalLayout->setObjectName("verticalLayout");
        timeLabel = new QLabel(MessageBubble);
        timeLabel->setObjectName("timeLabel");
        timeLabel->setAlignment(Qt::AlignCenter);
        timeLabel->setStyleSheet(QString::fromUtf8("color: #999; font-size: 11px;"));

        verticalLayout->addWidget(timeLabel);

        bubbleLayout = new QHBoxLayout();
        bubbleLayout->setObjectName("bubbleLayout");
        leftSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bubbleLayout->addItem(leftSpacer);

        bubbleLabel = new QLabel(MessageBubble);
        bubbleLabel->setObjectName("bubbleLabel");
        bubbleLabel->setWordWrap(true);
        bubbleLabel->setStyleSheet(QString::fromUtf8("padding: 8px 12px; border-radius: 8px; font-size: 14px;"));

        bubbleLayout->addWidget(bubbleLabel);

        nameLabel = new QLabel(MessageBubble);
        nameLabel->setObjectName("nameLabel");
        nameLabel->setStyleSheet(QString::fromUtf8("color: #999; font-size: 11px;"));

        bubbleLayout->addWidget(nameLabel);

        rightSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        bubbleLayout->addItem(rightSpacer);


        verticalLayout->addLayout(bubbleLayout);


        retranslateUi(MessageBubble);

        QMetaObject::connectSlotsByName(MessageBubble);
    } // setupUi

    void retranslateUi(QWidget *MessageBubble)
    {
        timeLabel->setText(QCoreApplication::translate("MessageBubble", "12:00", nullptr));
        bubbleLabel->setText(QCoreApplication::translate("MessageBubble", "\346\266\210\346\201\257\345\206\205\345\256\271", nullptr));
        nameLabel->setText(QString());
        (void)MessageBubble;
    } // retranslateUi

};

namespace Ui {
    class MessageBubble: public Ui_MessageBubble {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESSAGEBUBBLE_H
