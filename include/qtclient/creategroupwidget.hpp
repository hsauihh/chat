#ifndef CREATEGROUPWIDGET_H
#define CREATEGROUPWIDGET_H

#include <QDialog>

using namespace std;

namespace Ui {
    class CreateGroupWidget;
}

/*
 * ============================================================================
 * CreateGroupWidget — 创建/加入群组对话框
 * ============================================================================
 *
 * 【Qt 概念】QDialog 是专门用于对话框的基类。与 QWidget 的区别：
 *   - QDialog 有模态/非模态模式（模态=阻塞父窗口操作）
 *   - QDialog 自带 accept() / reject() 槽，分别返回 QDialog::Accepted / Rejected
 *   - 调用 dialog->exec() 会进入模态事件循环，阻塞代码直到用户关闭对话框
 *
 * 【模式切换】
 *   - 创建模式：显示群名输入框 + 描述输入框，隐藏 ID 输入框
 *   - 加入模式：显示 ID 输入框，隐藏群名和描述输入框
 *
 * 【注意】本类目前未在主界面中使用（ContactsManageWidget 直接管理了表单）。
 * 如果需要在弹出对话框中进行群组操作，可以实例化本类并通过 exec() 显示。
 */
class CreateGroupWidget : public QDialog
{
    Q_OBJECT
public:
    explicit CreateGroupWidget(QWidget *parent = nullptr);
    ~CreateGroupWidget();

    string getGroupName() const;   // 获取群组名称
    string getGroupDesc() const;   // 获取群组描述
    int getGroupId() const;        // 获取群组 ID（加入模式）

    void setCreateMode();          // 切换为创建群组模式
    void setJoinMode();            // 切换为加入群组模式

private slots:
    void onConfirmClicked();       // 确认按钮 → 校验输入 → accept()

private:
    Ui::CreateGroupWidget *ui;
    bool _isCreateMode;
};

#endif
