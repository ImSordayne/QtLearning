/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \QtLearning\Widgets\watermark\src\sysTray.cpp
 */
#include "sysTray.h"
#include "mainWindow.h"
#include <QMenu>
#include <QAction>
#include <QApplication>

sysTray::sysTray(watermark &mark) : QObject(nullptr) {
    this->mark = &mark;
    trayIcon = new QSystemTrayIcon(this);
    QIcon icon(":/logo.png");
    if (icon.isNull()) {
        qWarning() << "图标加载失败，请检查路径！";
    }
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("watermark");
    connect(trayIcon, &QSystemTrayIcon::activated, this, &sysTray::onIconClicked);
    trayIcon->show();
}

void sysTray::onIconClicked(QSystemTrayIcon::ActivationReason reason) {
    if(reason == QSystemTrayIcon::Context) {
        QMenu menu;
        QAction *exitAction = menu.addAction("退出");
        QAction *settingsAction = menu.addAction("显示设置");
        connect(exitAction, &QAction::triggered, qApp, &QApplication::quit);
        connect(settingsAction, &QAction::triggered, this, [this](){
            mainWindow *w = new mainWindow(*this->mark);
            w->show();
        });
        menu.exec(QCursor::pos());
    }
}