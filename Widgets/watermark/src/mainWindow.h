/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \QtLearning\Widgets\watermark\src\mainWindow.h
 */
#include <QLineEdit>
#include <QPushButton>
#include <QWidget>
#include "watermark.h"

class mainWindow : public QWidget {
    Q_OBJECT
    watermark *mark;
public slots:
    void saveAndUse();
public:
    mainWindow(watermark &mark);
    QLineEdit *xEdit,
              *yEdit,
              *titleEdit,
              *textEdit,
              *titleSizeEdit,
              *textSizeEdit,
              *cREdit,
              *cGEdit,
              *cBEdit,
              *cAlphaEdit;
    QPushButton *saveAndUseButton;
};