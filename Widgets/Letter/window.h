#pragma once

#include <QObject>
#include <vector>
#include <string>
#include <QApplication>

class QApplication;
class QWidget;

class Window : public QObject {
    Q_OBJECT

private:
    QApplication* app;
    std::vector<std::string> colorList;
    std::vector<std::string> textList;
    int windowsCount;

signals:
    void allWindowsClosed();

public:
    Window(int argc, char* argv[]);
    bool run();

private:
    void endWindow();
    void popWindow(const std::string& color, const std::string& text);
    void startWindow();
};
