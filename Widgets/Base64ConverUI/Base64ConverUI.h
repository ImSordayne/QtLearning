/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2026-03-26 21:54:41
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2026-03-26 21:59:40
 * @FilePath: \QtLearning\Widgets\Base64ConverUI\src\Base64ConverUI.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \Base64ConverUI\src\Base64ConverUI.h
 */
#pragma once

#include <QMainWindow>
#include <QApplication>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include <QProgressBar>
#include <QProgressDialog>
#include "Base64Conver.h"

class Base64ConverUI : public QMainWindow
{
    Q_OBJECT

public:
    Base64ConverUI(QWidget *parent = nullptr);
    ~Base64ConverUI();

private slots:
    void browseFileDir();
    void enCode();
    void deCode();

private:
    QLineEdit* pathEdit;
    QProgressBar *progressBar;
};
