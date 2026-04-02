/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \QtLearning\Widgets\watermark\src\main.cpp
 */
/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-03 11:18:00
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2026-04-01 18:58:44
 * @FilePath: \QtLearning\Widgets\watermark\src\main.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <ini.h>
#include <string>
#include <iostream>
#include <QApplication>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include "watermark.h"
#include "sysTray.h"
#include "mainWindow.h"

int main(int argc, char* argv[]){
    std::cout << "Program start..." << std::endl;
	QApplication *app = new QApplication(argc, argv);
	QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
	if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        auto choice = QMessageBox::critical(nullptr, QObject::tr("Systray"),
            QObject::tr("I couldn't detect any system tray on this system."),
            QMessageBox::Close | QMessageBox::Ignore);
        if (choice == QMessageBox::Close)
            return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);
	watermark *mark = new watermark();
	mainWindow *w = new mainWindow(*mark);
    delete w;
    sysTray *tray = new sysTray(*mark);
	mark->show();
    std::cout << "watermark showed\n" << std::endl;
	return app->exec();
}