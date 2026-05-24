#ifndef CONTACTSMANAGEWIDGET_HPP
#define CONTACTSMANAGEWIDGET_HPP

#include <QWidget>
#include <string>

using namespace std;

namespace Ui {
    class ContactsManageWidget;
}

/*
 * ============================================================================
 * ContactsManageWidget — 联系人管理页面
 * ============================================================================
 *
 * 【功能】
 *   1. 添加好友：输入好友 ID → 点击按钮 → emit addFriendRequest
 *   2. 创建群组：输入群名 + 描述 → 点击按钮 → emit createGroupRequest
 *   3. 加入群组：输入群 ID → 点击按钮 → emit addGroupRequest
 *
 * 【位置】在右侧面板 (_rightPanel) 的 index 1，点击侧边栏"联系人"按钮切换到此页。
 *
 * 【如何添加新的管理功能（如删除好友）】
 *   1. 在 contactsmanage.ui 中添加按钮和输入框
 *   2. 在本类的 signals: 中添加新信号（如 deleteFriendRequest(int userId)）
 *   3. 添加对应的 private slot 和 connect
 *   4. 在 MainWidget 中 connect 该信号到处理方法
 */
class ContactsManageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContactsManageWidget(QWidget *parent = nullptr);
    ~ContactsManageWidget();

signals:
    void addFriendRequest(int friendId);
    void createGroupRequest(const string &groupName, const string &groupDesc);
    void addGroupRequest(int groupId);

private slots:
    void onAddFriendClicked();
    void onCreateGroupClicked();
    void onAddGroupClicked();

private:
    Ui::ContactsManageWidget *ui;
};

#endif
