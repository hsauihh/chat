/*
 * ============================================================================
 * QtChatClient 入口文件
 * ============================================================================
 * 用法: ./QtChatClient <服务器IP> <端口号>
 * 示例: ./QtChatClient 127.0.0.1 6000
 *
 * 【Qt 应用生命周期】
 *   1. 创建 QApplication（管理全局事件循环）
 *   2. 创建主窗口 MainWidget
 *   3. 调用 show() 显示窗口
 *   4. 调用 connectToServer() 连接服务器
 *   5. app.exec() 进入事件循环（阻塞，直到窗口关闭）
 *
 * 【事件循环】
 *   Qt 程序靠"事件循环" (event loop) 驱动。app.exec() 启动事件循环后，
 *   程序不断从事件队列中取出事件（鼠标点击、键盘输入、网络数据到达等），
 *   分发给对应的 Widget 处理。信号/槽机制在事件循环中运转。
 */

#include <QApplication>
#include <QFont>
#include <iostream>
#include <clocale>
#include "mainwidget.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // -- 参数校验 --
    if (argc < 3)
    {
        cerr << "command invalid! example: ./QtChatClient 127.0.0.1 6000" << endl;
        return -1;
    }

    // -- 本地化设置 --
    // setlocale: 设置 C 标准库的 locale，确保 std::string 中的中文能正常显示
    setlocale(LC_ALL, "zh_CN.UTF-8");

    // QT_IM_MODULE 控制 Qt 使用哪个输入法模块
    // 空字符串 = 使用内置 "compose" 模块，兼容 WSLg XWayland 环境下的中文输入
    // 其他选项: "ibus", "fcitx", "xim"。如果中文输入法不可用，尝试切换这些值
    qputenv("QT_IM_MODULE", "");

    char *ip = argv[1];
    quint16 port = static_cast<quint16>(atoi(argv[2]));

    /*
     * QApplication 必须在所有 QWidget 之前创建。
     * 它负责：全局事件循环、剪贴板管理、桌面集成、样式管理。
     * 每个 Qt GUI 应用有且只有一个 QApplication 实例。
     */
    QApplication app(argc, argv);

    // 设置默认字体（Qt 会自动使用系统支持的回退字体来显示中文）
    QFont font = app.font();
    font.setPointSize(10);
    app.setFont(font);

    // 创建并显示主窗口
    MainWidget mainWidget;
    mainWidget.show();

    // 连接到聊天服务器
    mainWidget.connectToServer(QString(ip), port);

    /*
     * app.exec() 启动 Qt 事件循环。
     * 这一行之后的代码只有在用户关闭窗口（app.quit() 被调用）后才会执行。
     * 返回值是程序的退出码。
     */
    return app.exec();
}
