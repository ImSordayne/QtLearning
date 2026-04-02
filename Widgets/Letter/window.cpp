// window.cpp

#include "window.h"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QFont>
#include <QTimer>
#include <QScreen>
#include <QGuiApplication>
#include <QRandomGenerator>
#include <QHBoxLayout>
#include <QRect>


Window::Window(int argc, char* argv[]) {
    app = new QApplication(argc, argv);
    app -> setApplicationName("Qt Letter");
    app -> setApplicationVersion("1.0.0");
    app -> setOrganizationName("Sordayne");
    colorList = {"#f570c8ff", "#6dc9ffff", "#5aff67ff", "#ffd37aff", "#ff6262ff", "#b65cffff"};
    textList = {"别熬夜", "多喝水哦", "愿所有烦恼都消失", "记得吃水果哦"};
    //textList = {"i1", "i2", "i3"};
    windowsCount = 0;
}

bool Window::run() {
    startWindow();
    app->setQuitOnLastWindowClosed(false);
    return app->exec();
}

void Window::popWindow(const std::string& color, const std::string& text) {
    windowsCount++;

    // 创建窗口
    QWidget* window = new QWidget();
    window->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    
    // 设置背景色、圆角
    window->setStyleSheet(QString("QWidget { background-color: %1;"
                                            "border-radius: 30px;}")
                                  .arg(QString::fromStdString(color)));
    
    // 创建并设置标签
    QLabel* label = new QLabel(QString::fromStdString(text));
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel { background: transparent;"
                                 "color: #000000; "
                                 "font-size: 14px; "
                                 "border-radius: 30px; }");
    
    // 设置布局
    QVBoxLayout* layout = new QVBoxLayout(window);
    layout->addWidget(label);
    layout->setContentsMargins(30, 20, 30, 20);

    // 自适应大小
    window->adjustSize();
    
    // 获取屏幕尺寸并计算随机位置
    QScreen* screen = QGuiApplication::primaryScreen();
    QRect screenRect = screen->availableGeometry();
    
    int windowWidth = window->width();
    int windowHeight = window->height();
    
    // 确保窗口在屏幕内
    int maxX = screenRect.width() - windowWidth;
    int maxY = screenRect.height() - windowHeight;
    
    if (maxX > 0 && maxY > 0) {
        int x = QRandomGenerator::global()->bounded(0, maxX);
        int y = QRandomGenerator::global()->bounded(0, maxY);
        window->move(x, y);
    } else {
        // 窗口太大，居中显示
        window->move(screenRect.center() - window->rect().center());
    }

    auto closeWindow = [this, window](){
        windowsCount--;
        if (windowsCount == 0) {
            emit allWindowsClosed();
        }
        window->close();
        window->deleteLater();
    };
    // 显示窗口
    window->show();
    // 使用QTimer在15秒后关闭窗口
    QTimer::singleShot(15000, window, closeWindow);
}

void Window::startWindow() {
    // 创建主窗口
    QWidget* window = new QWidget();
    window->setWindowTitle("Main");
    window->setFixedSize(400, 300);
    
    // 设置无边框窗口
    window->setWindowFlags(Qt::FramelessWindowHint);
    
    // 设置窗口背景色、圆角
    window->setStyleSheet(
        "QWidget { "
        "background-color: #f0f0f0; "
        "border-radius: 30px;"
        "}"
    );
    
    // 创建垂直布局
    QVBoxLayout* layout = new QVBoxLayout(window);
    
    // 创建标签（显示文字）
    QLabel* label = new QLabel("你好");
    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet("QLabel { color: #000000; "
                         "font-size: 30px; "
                         "border-radius: 30px; }");
    
    // 设置标签字体
    QFont labelFont("Microsoft YaHei", 24, QFont::Bold);
    label->setFont(labelFont);
    
    // 创建按钮
    QPushButton* button = new QPushButton("你好！");
    
    // 设置按钮字体
    QFont buttonFont("Microsoft YaHei", 16);
    button->setFont(buttonFont);
    
    // 设置按钮样式
    button->setStyleSheet(
        "QPushButton {"
        "   background-color: #4CAF50;"
        "   color: white;"
        "   border-radius: 10px;"
        "   padding: 15px;"
        "   border: none;"
        "}"
        "QPushButton:hover {"
        "   background-color: #45a049;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #3d8b40;"
        "}"
    );
    
    // 添加控件到布局
    layout->addWidget(label);
    layout->addWidget(button);
    
    // 设置布局间距
    layout->setSpacing(30);
    layout->setContentsMargins(50, 50, 50, 50);

    // 为按钮添加点击事件
    QObject::connect(button, &QPushButton::clicked, [this, window]() {
        window->showMinimized();
        for (int i = 0; i < 300; i++) { // 弹出300个
            QTimer::singleShot(i * 10, [this, i]() { // 每隔10毫秒弹出一个
                int colorIndex = QRandomGenerator::global()->bounded(0, int(colorList.size()));
                int textIndex = QRandomGenerator::global()->bounded(0, int(textList.size()));
                popWindow(colorList[colorIndex], textList[textIndex]);
            });
        }
    });
    
    // 显示窗口
    window->show();
}