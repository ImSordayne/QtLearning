/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \QtLearning\Widgets\watermark\src\watermark.h
 */
#pragma once
#include <QWidget>
#include <QLabel>
#include <QString>
#include <QVBoxLayout>

class watermark : public QWidget {
    Q_OBJECT
    QLabel *title;
    QLabel *text;
    QVBoxLayout *layout;
    QString color;
    class markInfo {
    public:
        int cR = 0, cG = 0, cB = 0, titleSize = 40, textSize = 20;
        std::string title = "此Windows无需激活!", text = "您已激活了该Windows";
    }info;
public:
    watermark();
    void setCR(int cR);
    void setCG(int cG);
    void setCB(int cB);
    void setTitle(std::string title);
    void setTitleSize(int size);
    void setText(std::string text);
    void setTextSize(int size);
    void setColor(std::string color);
};