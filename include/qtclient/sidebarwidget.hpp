#ifndef SIDEBARWIDGET_HPP
#define SIDEBARWIDGET_HPP

#include <QWidget>

namespace Ui {
    class SidebarWidget;
}

/*
 * ============================================================================
 * SidebarWidget — 左侧导航侧边栏（60px 宽）
 * ============================================================================
 *
 * 【功能】
 *   1. 显示用户头像按钮 → 点击展开/收起用户信息面板（ID、名称、退出登录）
 *   2. 聊天按钮 → 切换到聊天视图
 *   3. 联系人按钮 → 切换到联系人管理视图
 *   4. 清理按钮 → 清理当前聊天记录（弹出确认对话框）
 *
 * 【Qt 概念】本组件的 UI 完全由 sidebar.ui 文件定义（通过 Qt Designer 可视化
 * 编辑）。编译时 UIC (UI Compiler) 将 .ui 文件编译为 ui_sidebar.h，由代码中
 * ui->setupUi(this) 调用创建所有子 Widget。
 *
 * 【信号设计原则】SidebarWidget 本身不执行任何页面切换操作，只通过 emit 信号
 * 把"按钮被点击了"这件事通知给 MainWidget。具体做什么由 MainWidget 决定。
 * 这种"子组件只发信号，父组件响应"的模式是 Qt 推荐的解耦方式。
 *
 * 【布局结构（sidebar.ui 定义）】
 *   ┌──────────┐
 *   │  👤头像   │ ← avatarBtn (44x44 圆形按钮)
 *   │  [ID/名称]│ ← userInfoPanel (默认隐藏，点击头像展开)
 *   │  [退出]   │ ← logoutBtn
 *   │  ─────── │ ← navSeparator (1px 分隔线)
 *   │  💬聊天   │ ← chatNavBtn (导航到聊天视图)
 *   │  👥联系人 │ ← contactsNavBtn (导航到联系人管理视图)
 *   │          │ ← bottomSpacer (弹性空间)
 *   │  🗑清理   │ ← clearHistoryBtn (清理聊天记录)
 *   └──────────┘
 */
class SidebarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SidebarWidget(QWidget *parent = nullptr);
    ~SidebarWidget();

    // 设置当前用户信息显示（登录成功后由 MainWidget 调用）
    void setUserInfo(int userId, const QString &userName);

signals:
    // 【Qt 概念】signals: 下的函数不需要写实现，moc 会自动生成函数体。
    // 其他类通过 connect(src, SIGNAL, dest, SLOT) 来订阅这些信号。

    void chatNavClicked();          // 点击"聊天"导航按钮
    void contactsNavClicked();     // 点击"联系人管理"导航按钮
    void logoutRequest();           // 点击"退出登录"
    void clearHistoryRequest();     // 点击"清理聊天记录"

private slots:
    void onAvatarClicked();  // 点击头像 → 切换 userInfoPanel 显示/隐藏

private:
    Ui::SidebarWidget *ui;
};

#endif
