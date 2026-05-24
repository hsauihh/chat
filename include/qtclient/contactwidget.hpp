#ifndef CONTACTWIDGET_H
#define CONTACTWIDGET_H

#include <QWidget>
#include <QListWidgetItem>
#include "user.hpp"
#include "group.hpp"

using namespace std;

namespace Ui {
    class ContactWidget;
}

/*
 * ============================================================================
 * ContactWidget — 联系人列表（好友 + 群组 + 搜索过滤）
 * ============================================================================
 *
 * 【功能】
 *   1. 显示好友列表（名称、ID、在线状态）
 *   2. 显示群组列表（名称、ID、描述）
 *   3. 搜索框实时过滤
 *   4. 红点未读消息标记
 *   5. 双击发起聊天
 *
 * 【Qt 概念】QListWidget 是 Qt 提供的列表组件，每个 item 是一个 QListWidgetItem。
 * 每个 item 可以携带自定义数据（通过 setData(Qt::UserRole, value) 存储），
 * 在点击时可以取出 item->data(Qt::UserRole) 来获取用户 ID 或群组 ID。
 *
 * 【数据存储】联系人数据使用 setData() 存储在 item 中：
 *   item->setData(Qt::UserRole, userId);  // 把用户 ID 藏在 item 里
 *   item->data(Qt::UserRole).toInt();     // 点击时取出 ID
 *
 * 【添加新功能的例子】
 *   如果想在好友列表上新增右键菜单：
 *   1. 在 contactwidget.ui 中给 friendList 设置 contextMenuPolicy
 *   2. 在本类中添加 onFriendContextMenu(const QPoint &pos) 槽函数
 *   3. 在构造函数中用 connect 连接 customContextMenuRequested 信号
 */
class ContactWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ContactWidget(QWidget *parent = nullptr);
    ~ContactWidget();

    // ---- 公开方法 ----
    // 用服务端返回的数据填充好友列表
    void updateFriendList(vector<User> &friends);
    // 用服务端返回的数据填充群组列表
    void updateGroupList(vector<Group> &groups);
    // 更新单个好友的在线/离线状态显示
    void updateFriendState(int userId, const string &state);
    // 设置列表项的未读红点（true=显示红点, false=清除）
    void setItemUnread(int id, bool isGroup, bool unread);

signals:
    // 用户双击好友 → 发起一对一聊天（附带名称，避免 UI 显示"用户+ID"）
    void chatRequested(int userId, const QString &name);
    // 用户双击群组 → 发起群聊
    void groupChatRequested(int groupId, const QString &name);

private slots:
    void onFriendDoubleClicked(QListWidgetItem *item);
    void onGroupDoubleClicked(QListWidgetItem *item);
    void onSearchChanged(const QString &text);

private:
    void applySearchFilter();

    Ui::ContactWidget *ui;
    QString _searchText;  // 当前搜索框文本
};

#endif
