#include "calculator.h"
#include <QApplication>
#include <QStyleFactory>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("Qt Calculator");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Sordayne");
    
    // 设置全局样式
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    
    // 设置全局字体
    QFont font("Arial", 10);
    app.setFont(font);
    
    // 创建并显示计算器
    Calculator calculator;
    calculator.show();
    
    return app.exec();
}