/**
 * 系统监控工具 - 主程序入口
 * 功能：创建应用程序实例和主窗口
 */

#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("System Monitor");      // 应用程序名称
    app.setApplicationVersion("1.0.0");            // 版本号
    app.setOrganizationName("Sordayne");          // 组织名称
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    MainWindow window;
    window.show();
    
    return app.exec();
}