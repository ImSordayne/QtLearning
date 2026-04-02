/*
 * @Author: Sordayne 455248805@qq.com
 * @FilePath: \Base64ConverUI\src\Base64ConverUI.cpp
 */
#include "Base64ConverUI.h"

Base64ConverUI::Base64ConverUI(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("文件Base64编解码器");

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    QFormLayout* formLayout = new QFormLayout();

    pathEdit = new QLineEdit(this);
    progressBar = new QProgressBar(this);
    progressBar->setRange(0, 100);
    progressBar->setValue(0);

    QPushButton* BrowseGameDirBtn = new QPushButton("浏览...", this);
    connect(BrowseGameDirBtn, &QPushButton::clicked, this, &Base64ConverUI::browseFileDir);
    QHBoxLayout* DirLayout = new QHBoxLayout();
    DirLayout->addWidget(pathEdit);
    DirLayout->addWidget(BrowseGameDirBtn);
    formLayout->addRow("文件路径:", DirLayout);

    QHBoxLayout *progressLayout = new QHBoxLayout();
    progressLayout->addWidget(progressBar);
    formLayout->addRow("进度:", progressLayout);


    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* enCodeButton = new QPushButton("编码");
    connect(enCodeButton, &QPushButton::clicked, this, &Base64ConverUI::enCode);
    buttonLayout->addWidget(enCodeButton);
    QPushButton* deCodeButton = new QPushButton("解码");
    connect(deCodeButton, &QPushButton::clicked, this, &Base64ConverUI::deCode);
    buttonLayout->addWidget(deCodeButton);
    formLayout->addRow(buttonLayout);
    mainLayout->addLayout(formLayout);
    setFixedSize(400, 100);
}

Base64ConverUI::~Base64ConverUI()
{}
void Base64ConverUI::browseFileDir()
{
    QString Dir = QFileDialog::getOpenFileName(this, "选择文件路径ַ");
    if (!Dir.isEmpty()) {
        pathEdit->setText(Dir);
    }
}

void Base64ConverUI::enCode() {
    QString filePath = pathEdit->text();
    if (filePath.isEmpty())QMessageBox::warning(this, "错误", "文件名不能为空");
    else try {
        bool res = Base64Conver::encFile(filePath.toStdString(), filePath.toStdString() + ".base64.txt", [this](size_t total, size_t convered) -> void* {
            progressBar->setValue((convered * 1.0) / total * 100);
            QApplication::processEvents();
            return nullptr;
        });
        if(res)
            QMessageBox::information(this, "完成", "编码成功");
        else
            QMessageBox::information(this, "完成", "编码失败");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "错误", e.what());
    }
}

void Base64ConverUI::deCode() {

    QString filePath = pathEdit->text();
    std::string s_filePath = filePath.toStdString();
    if (filePath.isEmpty())QMessageBox::warning(this, "错误", "文件名不能为空");
    else if (s_filePath.size() < 11 || s_filePath.substr(s_filePath.size() - 11, 11) != ".base64.txt")QMessageBox::warning(this, "错误", "文件名不合法");
    else try {
        bool res = Base64Conver::decFile(filePath.toStdString(), s_filePath.substr(0, s_filePath.size() - 11), [this](size_t total, size_t convered) -> void* {
            progressBar->setValue((convered * 1.0) / total * 100);
            QApplication::processEvents();
            return nullptr;
        });
        if(res)
            QMessageBox::information(this, "完成", "解码成功");
        else
            QMessageBox::information(this, "完成", "解码失败");
    }
    catch (const std::exception& e) {
        QMessageBox::warning(this, "错误", e.what());
    }

}