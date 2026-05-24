/********************************************************************************
** Form generated from reading UI file 'creategroupwidget.ui'
**
** Created by: Qt User Interface Compiler version 6.10.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEGROUPWIDGET_H
#define UI_CREATEGROUPWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_CreateGroupWidget
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *titleLabel;
    QLineEdit *groupNameEdit;
    QLineEdit *groupDescEdit;
    QLineEdit *groupIdEdit;
    QHBoxLayout *btnLayout;
    QPushButton *confirmBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *CreateGroupWidget)
    {
        if (CreateGroupWidget->objectName().isEmpty())
            CreateGroupWidget->setObjectName("CreateGroupWidget");
        CreateGroupWidget->resize(350, 250);
        verticalLayout = new QVBoxLayout(CreateGroupWidget);
        verticalLayout->setObjectName("verticalLayout");
        titleLabel = new QLabel(CreateGroupWidget);
        titleLabel->setObjectName("titleLabel");
        titleLabel->setAlignment(Qt::AlignCenter);
        titleLabel->setStyleSheet(QString::fromUtf8("font-size: 16px; font-weight: bold;"));

        verticalLayout->addWidget(titleLabel);

        groupNameEdit = new QLineEdit(CreateGroupWidget);
        groupNameEdit->setObjectName("groupNameEdit");

        verticalLayout->addWidget(groupNameEdit);

        groupDescEdit = new QLineEdit(CreateGroupWidget);
        groupDescEdit->setObjectName("groupDescEdit");

        verticalLayout->addWidget(groupDescEdit);

        groupIdEdit = new QLineEdit(CreateGroupWidget);
        groupIdEdit->setObjectName("groupIdEdit");
        groupIdEdit->setVisible(false);

        verticalLayout->addWidget(groupIdEdit);

        btnLayout = new QHBoxLayout();
        btnLayout->setObjectName("btnLayout");
        confirmBtn = new QPushButton(CreateGroupWidget);
        confirmBtn->setObjectName("confirmBtn");

        btnLayout->addWidget(confirmBtn);

        cancelBtn = new QPushButton(CreateGroupWidget);
        cancelBtn->setObjectName("cancelBtn");

        btnLayout->addWidget(cancelBtn);


        verticalLayout->addLayout(btnLayout);


        retranslateUi(CreateGroupWidget);

        QMetaObject::connectSlotsByName(CreateGroupWidget);
    } // setupUi

    void retranslateUi(QDialog *CreateGroupWidget)
    {
        CreateGroupWidget->setWindowTitle(QCoreApplication::translate("CreateGroupWidget", "\345\210\233\345\273\272\347\276\244\347\273\204", nullptr));
        titleLabel->setText(QCoreApplication::translate("CreateGroupWidget", "\345\210\233\345\273\272\347\276\244\347\273\204", nullptr));
        groupNameEdit->setPlaceholderText(QCoreApplication::translate("CreateGroupWidget", "\347\276\244\347\273\204\345\220\215\347\247\260", nullptr));
        groupDescEdit->setPlaceholderText(QCoreApplication::translate("CreateGroupWidget", "\347\276\244\347\273\204\346\217\217\350\277\260", nullptr));
        groupIdEdit->setPlaceholderText(QCoreApplication::translate("CreateGroupWidget", "\347\276\244\347\273\204 ID", nullptr));
        confirmBtn->setText(QCoreApplication::translate("CreateGroupWidget", "\347\241\256\350\256\244", nullptr));
        cancelBtn->setText(QCoreApplication::translate("CreateGroupWidget", "\345\217\226\346\266\210", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateGroupWidget: public Ui_CreateGroupWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEGROUPWIDGET_H
