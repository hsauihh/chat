#ifndef INFOWIDGET_H
#define INFOWIDGET_H

#include <QWidget>

using namespace std;

namespace Ui {
    class InfoWidget;
}

// 右侧信息面板
class InfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit InfoWidget(QWidget *parent = nullptr);
    ~InfoWidget();

    // 设置当前用户信息
    void setUserInfo(int userId, const QString &userName);

signals:
    // 添加好友请求
    void addFriendRequest(int friendId);
    // 创建群组请求 groupName, groupDesc
    void createGroupRequest(const string &groupName, const string &groupDesc);
    // 加入群组请求
    void addGroupRequest(int groupId);
    // 注销请求
    void logoutRequest();

private slots:
    void onAddFriendClicked();
    void onCreateGroupClicked();
    void onAddGroupClicked();
    void onLogoutClicked();

private:
    Ui::InfoWidget *ui;
};

#endif
