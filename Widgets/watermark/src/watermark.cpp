/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \QtLearning\Widgets\watermark\src\watermark.cpp
 */
#include "watermark.h"

watermark::watermark() : QWidget(nullptr, Qt::WindowFlags(Qt::FramelessWindowHint |
                                 Qt::WindowStaysOnTopHint |
                                 Qt::Tool |
                                 Qt::WindowTransparentForInput |
                                 Qt::WA_TranslucentBackground))
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->title = new QLabel(this);
    this->text = new QLabel(this);
    title->setAlignment(Qt::AlignCenter);
    text->setAlignment(Qt::AlignCenter);
    
    this->layout = new QVBoxLayout(this);
    layout->addWidget(title);
    layout->addWidget(text);
    
    QString qss = QString("QLabel {"
                  "   background-color: transparent;"
                  "   color: %1;"
                  "   font-size: 20px;"
                  "   font-weight: bold;"
                  "   font-family: 'Microsoft YaHei';"
                  "}").arg(color); 
    title->setStyleSheet(qss);
    text->setStyleSheet(qss);
    
    resize(300, 100);
}

void watermark::setCR(int cR) {
    info.cR = cR;
    setColor(QString("rgb(%1, %2, %3)")
        .arg(info.cR).arg(info.cG).arg(info.cB).toStdString());
}

void watermark::setCG(int cG) {
    info.cG = cG;
    setColor(QString("rgb(%1, %2, %3)")
        .arg(info.cR).arg(info.cG).arg(info.cB).toStdString());
}

void watermark::setCB(int cB) {
    info.cB = cB;
    setColor(QString("rgb(%1, %2, %3)")
        .arg(info.cR).arg(info.cG).arg(info.cB).toStdString());
}

void watermark::setTitle(std::string title) {
    info.title = title;
    this->title->setText(QString::fromStdString(title));
    this->adjustSize();
}


void watermark::setTitleSize(int size) {
    info.titleSize = size;
}

void watermark::setText(std::string text) {
    info.text = text;
    this->text->setText(QString::fromStdString(text));
    this->adjustSize();
}

void watermark::setTextSize(int size) {
    info.textSize = size;
}

void watermark::setColor(std::string color) {
    QString qss = QString("QLabel {"
                  "   background-color: transparent;"
                  "   color: %1;"
                  "   font-size: %2px;"
                  "   font-weight: bold;"
                  "   font-family: 'Microsoft YaHei';"
                  "}").arg(color).arg("%1"); 
    title->setStyleSheet(qss.arg(info.titleSize));
    text->setStyleSheet(qss.arg(info.textSize));
    this->update();
}