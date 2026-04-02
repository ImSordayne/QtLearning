#include <ini.h>
#include <string>
#include <iostream>
#include <QFormLayout>
#include <QHBoxLayout>
#include "mainWindow.h"

mainWindow::mainWindow(watermark &mark)
    : QWidget(nullptr)
{
    this->setStyleSheet("QPushButton { border-radius: 5px; border: 1px solid rgba(0, 255, 255, 0.38);}"
                        "QLineEdit { border-radius: 5px; border: 1px solid rgba(0, 255, 255, 0.38);}");
    this->mark = &mark;
    mINI::INIFile iniFile("settings.ini");
	mINI::INIStructure iniData;
	iniFile.read(iniData);
	int winX, winY, titleSize, textSize;
	std::string title, text;

	auto isNumberStr = [](std::string s) -> bool{
		bool hasDot = false;
		for(const char c:s) {
			if(!isalnum(c)) {
				return false;
			} else if(c == '.') {
				if(hasDot)
					return false;
				else
					hasDot = true;
			}
		}
		return true;
	};
	if(!iniData["window"].has("winx") || !isNumberStr(iniData["window"]["winx"]))
		iniData["window"]["winx"] = '0';
	if(!iniData["window"].has("winy") || !isNumberStr(iniData["window"]["winy"]))
		iniData["window"]["winy"] = '0';
	if(!iniData["window"].has("title"))
		iniData["window"]["title"] = "此Windows无需激活!";
	if(!iniData["window"].has("text")) 
		iniData["window"]["text"] = "您已激活了该Windows";
	if(!iniData["window"].has("titlesize") || !isNumberStr(iniData["window"]["titlesize"]))
		iniData["window"]["titlesize"] = '0';
	if(!iniData["window"].has("textsize") || !isNumberStr(iniData["window"]["textsize"]))
		iniData["window"]["textsize"] = '0';
	if(!iniData["color"].has("r") || !isNumberStr(iniData["color"]["r"]))
		iniData["color"]["r"] = "145";
	if(!iniData["color"].has("g") || !isNumberStr(iniData["color"]["g"]))
		iniData["color"]["g"] = "145";
	if(!iniData["color"].has("b") || !isNumberStr(iniData["color"]["b"]))
		iniData["color"]["b"] = "145";
	if(!iniData["color"].has("alpha") || !isNumberStr(iniData["color"]["alpha"]))
		iniData["color"]["alpha"] = "0.57";
	iniFile.write(iniData);

	winX = stoi(iniData["window"]["winx"]);
	winY = stoi(iniData["window"]["winy"]);
	titleSize = stoi(iniData["window"]["titlesize"]);
	textSize = stoi(iniData["window"]["textsize"]);
	text = iniData["window"]["text"];
	title = iniData["window"]["title"];
	
    std::cout << "Loaded settings.ini\n" << std::endl;
	this->mark->setTitle(title);
	this->mark->setText(text);
	this->mark->setTitleSize(titleSize);
	this->mark->setTextSize(textSize);
	this->mark->move(winX, winY);
	this->mark->setColor("rgba(" + iniData["color"]["r"] + ", "
                          + iniData["color"]["g"] + ", "
                          + iniData["color"]["b"] + ", "
                          + iniData["color"]["alpha"] + ")");
	this->mark->show();

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QFormLayout* formLayout = new QFormLayout();

    xEdit = new QLineEdit(this);
    xEdit->setText(iniData["window"]["winx"].c_str());
    yEdit = new QLineEdit(this);
    yEdit->setText(iniData["window"]["winy"].c_str());
    titleEdit = new QLineEdit(this);
    titleEdit->setText(iniData["window"]["title"].c_str());
    textEdit = new QLineEdit(this);
    textEdit->setText(iniData["window"]["text"].c_str());
    titleSizeEdit = new QLineEdit(this);
    titleSizeEdit->setText(iniData["window"]["titlesize"].c_str());
    textSizeEdit = new QLineEdit(this);
    textSizeEdit->setText(iniData["window"]["textsize"].c_str());
    cREdit = new QLineEdit(this);
    cREdit->setText(iniData["color"]["r"].c_str());
    cGEdit = new QLineEdit(this);
    cGEdit->setText(iniData["color"]["g"].c_str());
    cBEdit = new QLineEdit(this);
    cBEdit->setText(iniData["color"]["b"].c_str());
    cAlphaEdit = new QLineEdit(this);
    cAlphaEdit->setText(iniData["color"]["alpha"].c_str());
    saveAndUseButton = new QPushButton("保存并使用", this);

    QHBoxLayout* HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(xEdit);
    formLayout->addRow("X:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(yEdit);
    formLayout->addRow("Y:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(titleEdit);
    formLayout->addRow("Title:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(textEdit);
    formLayout->addRow("Text:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(titleSizeEdit);
    formLayout->addRow("Title Size:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(textSizeEdit);
    formLayout->addRow("Text Size:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(cREdit);
    formLayout->addRow("R:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(cGEdit);
    formLayout->addRow("G:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(cBEdit);
    formLayout->addRow("B:", HBoxLayout);
    HBoxLayout = new QHBoxLayout();
    HBoxLayout->addWidget(cAlphaEdit);
    formLayout->addRow("Alpha:", HBoxLayout);

    mainLayout->addLayout(formLayout);
    mainLayout->addWidget(saveAndUseButton);
    connect(saveAndUseButton, &QPushButton::clicked, this, &mainWindow::saveAndUse);
    this->setFixedSize(400, 300);
    this->setLayout(mainLayout);
}

void mainWindow::saveAndUse() {
    mINI::INIFile iniFile("settings.ini");
    mINI::INIStructure iniData;
    iniFile.read(iniData);

    iniData["window"]["winx"] = xEdit->text().toStdString();
    iniData["window"]["winy"] = yEdit->text().toStdString();
    iniData["window"]["title"] = titleEdit->text().toStdString();
    iniData["window"]["text"] = textEdit->text().toStdString();
    iniData["window"]["titlesize"] = titleSizeEdit->text().toStdString();
    iniData["window"]["textsize"] = textSizeEdit->text().toStdString();
    iniData["color"]["r"] = cREdit->text().toStdString();
    iniData["color"]["g"] = cGEdit->text().toStdString();
    iniData["color"]["b"] = cBEdit->text().toStdString();
    iniData["color"]["alpha"] = cAlphaEdit->text().toStdString();


    iniFile.write(iniData);
    std::cout << "Settings saved to settings.ini\n" << std::endl;

    this->mark->setTitle(titleEdit->text().toStdString());
    this->mark->setText(textEdit->text().toStdString());
    this->mark->setTitleSize(titleSizeEdit->text().toInt());
    this->mark->setTextSize(textSizeEdit->text().toInt());
    this->mark->move(xEdit->text().toInt(), yEdit->text().toInt());
    this->mark->setColor("rgba(" + cREdit->text().toStdString() + ", "
                          + cGEdit->text().toStdString() + ", "
                          + cBEdit->text().toStdString() + ", "
                          + cAlphaEdit->text().toStdString() + ")");
}