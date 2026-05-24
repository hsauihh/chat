/*
 * ============================================================================
 * SidebarWidget 实现 — 左侧导航侧边栏
 * ============================================================================
 *
 * 【Qt 概念】本文件展示了以下 Qt 核心模式：
 *   1. UI 文件 + 代码混合：ui->setupUi(this) 加载 .ui 文件，代码微调属性
 *   2. 信号/槽连接：connect(按钮, 信号, 本类, 槽/信号)
 *   3. QPainter 绘制：在 QPixmap 上用 QPainter 画图 → 生成 QIcon → 设置到按钮
 */

#include "sidebarwidget.hpp"
#include "ui_sidebar.h"
#include <QPushButton>
#include <QPainter>
#include <QPixmap>

// ============================================================================
// 图标绘制函数（用 QPainter 在内存中绘制矢量图标，无需外部图片文件）
// ============================================================================

/*
 * 【Qt 概念】QPixmap 是离屏像素缓冲区。QPainter 是绘图引擎，可以在任何
 * QPaintDevice（如 QWidget、QPixmap、QPrinter）上绘制。
 *
 * 设计模式：创建空白 QPixmap → 在它上面用 QPainter 画形状 → 转换为 QIcon。
 */

// 用户图标：圆形头部 + 半椭圆身体
static QIcon createUserIcon()
{
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);                  // 透明背景
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);     // 抗锯齿，让边缘平滑
    p.setPen(Qt::NoPen);                         // 无边框
    p.setBrush(QColor("#555"));                  // 深灰色填充
    p.drawEllipse(QPointF(12, 7), 5, 5);         // 头部：圆心(12,7) 半径5
    p.drawEllipse(QPointF(12, 19), 9, 7);        // 身体：圆心(12,19) rx=9 ry=7
    p.end();                                     // 结束绘制，释放资源
    return QIcon(pix);
}

// 聊天图标：圆角矩形气泡 + 左下小三角指针
static QIcon createChatIcon()
{
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#555"));
    p.drawRoundedRect(2, 2, 20, 15, 4, 4);      // 气泡主体：(2,2) 宽20高15 圆角4px
    // 气泡左下角的三角指针
    QPolygonF triangle;
    triangle << QPointF(6, 17) << QPointF(2, 12) << QPointF(8, 14);
    p.drawPolygon(triangle);
    p.end();
    return QIcon(pix);
}

// 联系人图标：两个重叠的人形（每个人形 = 圆头 + 半椭圆身体）
static QIcon createContactsIcon()
{
    QPixmap pix(24, 24);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor("#555"));
    // 左边的人
    p.drawEllipse(QPointF(8, 7), 4, 4);
    p.drawEllipse(QPointF(8, 18), 7, 5);
    // 右边的人
    p.drawEllipse(QPointF(16, 7), 4, 4);
    p.drawEllipse(QPointF(16, 18), 7, 5);
    p.end();
    return QIcon(pix);
}

// ============================================================================
// 构造 / 析构
// ============================================================================

SidebarWidget::SidebarWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::SidebarWidget)
{
    /*
     * 【Qt 概念】ui->setupUi(this) 是必须调用的第一行代码。
     * 它将 sidebar.ui 中定义的所有子 Widget 实例化并添加到当前 Widget 上。
     * setupUi 内部会调用 new、setParent、setGeometry 等操作。
     */
    ui->setupUi(this);

    // 设置程序化绘制的图标（覆盖 .ui 文件中定义的空文本）
    // setIcon() 设置图标，setIconSize() 指定图标显示大小
    ui->avatarBtn->setIcon(createUserIcon());
    ui->avatarBtn->setIconSize(QSize(24, 24));
    ui->chatNavBtn->setIcon(createChatIcon());
    ui->chatNavBtn->setIconSize(QSize(24, 24));
    ui->contactsNavBtn->setIcon(createContactsIcon());
    ui->contactsNavBtn->setIconSize(QSize(24, 24));

    /*
     * 【Qt 概念】connect 是 Qt 最核心的函数 — 连接信号到槽。
     * 语法: connect(发送者, &发送者类::信号名, 接收者, &接收者类::槽名)
     *
     * 现代 Qt (Qt5+) 使用类型安全的函数指针语法，拼写错误在编译期就能发现。
     * 旧式语法使用 SIGNAL()/SLOT() 宏，是字符串匹配，容易出错。
     */
    connect(ui->avatarBtn, &QPushButton::clicked, this, &SidebarWidget::onAvatarClicked);
    connect(ui->chatNavBtn, &QPushButton::clicked, this, &SidebarWidget::chatNavClicked);
    connect(ui->contactsNavBtn, &QPushButton::clicked, this, &SidebarWidget::contactsNavClicked);
    connect(ui->logoutBtn, &QPushButton::clicked, this, &SidebarWidget::logoutRequest);
    connect(ui->clearHistoryBtn, &QPushButton::clicked, this, &SidebarWidget::clearHistoryRequest);
}

SidebarWidget::~SidebarWidget()
{
    delete ui;  // 释放 UI 对象，Qt 会自动释放 ui 管理的所有子 Widget
}

// ============================================================================
// 公开方法
// ============================================================================

void SidebarWidget::setUserInfo(int userId, const QString &userName)
{
    // QString::arg() 是 Qt 的字符串格式化方法，类似 printf
    // "%1" 是第一个参数的占位符
    ui->userIdLabel->setText(QString("ID: %1").arg(userId));
    ui->userNameLabel->setText(userName);
}

// ============================================================================
// 私有槽
// ============================================================================

void SidebarWidget::onAvatarClicked()
{
    // 切换用户信息面板的显示/隐藏
    bool visible = ui->userInfoPanel->isVisible();
    ui->userInfoPanel->setVisible(!visible);
}
