#include "calculator.h"
#include <QKeyEvent>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QApplication>
#include <QPalette>

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
    , currentValue(0)
    , storedValue(0)
    , waitingForOperand(true)
    , hasDecimal(false)
{
    setWindowTitle("Qt Calculator");
    setFixedSize(660, 700);
    
    createUI();
    setupConnections();
    
    // 初始化显示
    updateDisplay();
}

Calculator::~Calculator()
{
}

void Calculator::createUI()
{
    // 设置窗口背景色
    this->setStyleSheet("background-color: #2c3e50;");
    
    // 主布局
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // 主显示
    display = new QLineEdit();
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setFont(QFont("Digital-7", 32));
    display->setText("0");
    display->setMinimumHeight(70);
    display->setStyleSheet(
        "QLineEdit {"
        "    border: 2px solid #34495e;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "    background: #1c2833;"
        "    color: #27ae60;"
        "    font-weight: bold;"
        "}"
    );
    mainLayout->addWidget(display);
    
    // 按钮网格布局
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(8);
    gridLayout->setContentsMargins(0, 10, 0, 0);
    
    // 创建所有按钮
    btnClearAll = new QPushButton("AC");
    btnClear = new QPushButton("C");
    btnBackspace = new QPushButton("⌫");
    btnDivide = new QPushButton("÷");
    
    btnMultiply = new QPushButton("×");
    btnSubtract = new QPushButton("-");
    btnAdd = new QPushButton("+");
    btnEquals = new QPushButton("=");
    
    btnDecimal = new QPushButton(".");
    btnPlusMinus = new QPushButton("±");
    btnPercent = new QPushButton("%");
    
    // 创建数字按钮
    for (int i = 0; i < 10; ++i) {
        digitButtons.append(new QPushButton(QString::number(i)));
    }
    
    // 设置数字按钮样式（蓝色）
    QString digitStyle = 
        "QPushButton {"
        "    font-size: 20px;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "    min-height: 80px;"
        "    border: 2px solid #3498db;"
        "    border-radius: 40px;"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #3498db, stop:1 #2980b9);"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #5dade2, stop:1 #3498db);"
        "    border: 2px solid #5dade2;"
        "}"
        "QPushButton:pressed {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #2980b9, stop:1 #1f618d);"
        "    border: 2px solid #2980b9;"
        "}";
    
    // 设置运算符按钮样式（橙色）
    QString operatorStyle = 
        "QPushButton {"
        "    font-size: 22px;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "    min-height: 80px;"
        "    border: 2px solid #e67e22;"
        "    border-radius: 10px;"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #e67e22, stop:1 #d35400);"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #f39c12, stop:1 #e67e22);"
        "    border: 2px solid #f39c12;"
        "}"
        "QPushButton:pressed {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #d35400, stop:1 #ba4a00);"
        "    border: 2px solid #d35400;"
        "}";
    
    // 设置功能按钮样式（灰色）
    QString functionStyle = 
        "QPushButton {"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "    min-height: 80px;"
        "    border: 2px solid #95a5a6;"
        "    border-radius: 10px;"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #95a5a6, stop:1 #7f8c8d);"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #bdc3c7, stop:1 #95a5a6);"
        "    border: 2px solid #bdc3c7;"
        "}"
        "QPushButton:pressed {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #7f8c8d, stop:1 #616a6b);"
        "    border: 2px solid #7f8c8d;"
        "}";
    
    // 设置等号按钮样式（绿色）
    QString equalsStyle = 
        "QPushButton {"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    min-width: 80px;"
        "    min-height: 80px;"
        "    border: 2px solid #2ecc71;"
        "    border-radius: 10px;"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #2ecc71, stop:1 #27ae60);"
        "    color: white;"
        "}"
        "QPushButton:hover {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #58d68d, stop:1 #2ecc71);"
        "    border: 2px solid #58d68d;"
        "}"
        "QPushButton:pressed {"
        "    background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "                                      stop:0 #27ae60, stop:1 #229954);"
        "    border: 2px solid #27ae60;"
        "}";
    
    // 应用样式
    for (QPushButton *btn : digitButtons) {
        btn->setStyleSheet(digitStyle);
    }
    
    // 运算符按钮应用橙色样式
    btnAdd->setStyleSheet(operatorStyle);
    btnSubtract->setStyleSheet(operatorStyle);
    btnMultiply->setStyleSheet(operatorStyle);
    btnDivide->setStyleSheet(operatorStyle);
    
    // 等号按钮应用绿色样式
    btnEquals->setStyleSheet(equalsStyle);
    
    // 功能按钮应用灰色样式
    QVector<QPushButton*> functionButtons = {
        btnClearAll, btnClear, btnBackspace, 
        btnDecimal, btnPlusMinus, btnPercent
    };
    
    for (QPushButton *btn : functionButtons) {
        btn->setStyleSheet(functionStyle);
    }
    
    // 布局按钮
    // 第一行：AC C ⌫ ÷
    int row = 0, col = 0;
    gridLayout->addWidget(btnClearAll, row, col++);
    gridLayout->addWidget(btnClear, row, col++);
    gridLayout->addWidget(btnBackspace, row, col++);
    gridLayout->addWidget(btnDivide, row, col++);
    
    // 第二行：7 8 9 ×
    row++; col = 0;
    gridLayout->addWidget(digitButtons[7], row, col++); // 7
    gridLayout->addWidget(digitButtons[8], row, col++); // 8
    gridLayout->addWidget(digitButtons[9], row, col++); // 9
    gridLayout->addWidget(btnMultiply, row, col++);
    
    // 第三行：4 5 6 -
    row++; col = 0;
    gridLayout->addWidget(digitButtons[4], row, col++); // 4
    gridLayout->addWidget(digitButtons[5], row, col++); // 5
    gridLayout->addWidget(digitButtons[6], row, col++); // 6
    gridLayout->addWidget(btnSubtract, row, col++);
    
    // 第四行：1 2 3 +
    row++; col = 0;
    gridLayout->addWidget(digitButtons[1], row, col++); // 1
    gridLayout->addWidget(digitButtons[2], row, col++); // 2
    gridLayout->addWidget(digitButtons[3], row, col++); // 3
    gridLayout->addWidget(btnAdd, row, col++);
    
    // 第五行：0 . ± =
    row++; col = 0;
    gridLayout->addWidget(digitButtons[0], row, col++); // 0
    gridLayout->addWidget(btnDecimal, row, col++);
    gridLayout->addWidget(btnPlusMinus, row, col++);
    gridLayout->addWidget(btnEquals, row, col++);
    
    // 第六行：百分比单独一行，占满宽度
    row++; col = 0;
    gridLayout->addWidget(btnPercent, row, col, 1, 4); // 占满4列
    
    mainLayout->addLayout(gridLayout);
    
    // 设置窗口属性
    setLayout(mainLayout);
}

void Calculator::setupConnections()
{
    // 数字按钮
    for (int i = 0; i < 10; ++i) {
        connect(digitButtons[i], &QPushButton::clicked, this, &Calculator::onDigitClicked);
    }
    
    // 运算符按钮
    connect(btnAdd, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(btnSubtract, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(btnMultiply, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    connect(btnDivide, &QPushButton::clicked, this, &Calculator::onOperatorClicked);
    
    // 功能按钮
    connect(btnEquals, &QPushButton::clicked, this, &Calculator::onEqualsClicked);
    connect(btnClear, &QPushButton::clicked, this, &Calculator::onClearClicked);
    connect(btnClearAll, &QPushButton::clicked, this, &Calculator::onClearAllClicked);
    connect(btnBackspace, &QPushButton::clicked, this, &Calculator::onBackspaceClicked);
    connect(btnDecimal, &QPushButton::clicked, this, &Calculator::onDecimalClicked);
    connect(btnPlusMinus, &QPushButton::clicked, this, &Calculator::onPlusMinusClicked);
    connect(btnPercent, &QPushButton::clicked, this, &Calculator::onPercentClicked);
}

void Calculator::updateDisplay()
{
    QString text = QString::number(currentValue, 'g', 15);
    
    if (text.isEmpty()) {
        text = "0";
    }
    
    display->setText(text);
}

void Calculator::onDigitClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString digit = button->text();
    
    // 获取当前显示文本
    QString currentText = display->text();
    
    if (waitingForOperand) {
        // 等待新操作数，直接替换
        currentText = digit;
        waitingForOperand = false;
        hasDecimal = false;
        isPlaceholder = false;
    } else {
        // 正在输入中，追加数字
        // 如果当前是"0"且没有小数点，则替换
        if (currentText == "0" && !hasDecimal) {
            currentText = digit;
        } else {
            if(currentText.endsWith(".0") && isPlaceholder) {
                currentText.chop(1);    //移除末尾0
                currentText += digit;
                isPlaceholder = false;
            } else {
                currentText += digit;
            }
        }
    }
    
    // 更新显示
    display->setText(currentText);
    // 更新数值
    currentValue = currentText.toDouble();
}

void Calculator::onOperatorClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    
    // 如果已经有运算符在等待，先计算之前的结果
    if (!waitingForOperand && !currentOperator.isEmpty()) {
        calculateResult();
    } else if (!waitingForOperand) {
        // 存储当前值，等待下一个操作数
        storedValue = currentValue;
    }
    
    currentOperator = button->text();
    waitingForOperand = true;
    hasDecimal = false;
}

void Calculator::onEqualsClicked()
{
    if (!currentOperator.isEmpty() && !waitingForOperand) {
        calculateResult();
        currentOperator.clear();
    }
}

void Calculator::calculateResult()
{
    double result = currentValue;
    
    if (currentOperator == "+") {
        result = storedValue + currentValue;
    } else if (currentOperator == "-") {
        result = storedValue - currentValue;
    } else if (currentOperator == "×") {
        result = storedValue * currentValue;
    } else if (currentOperator == "÷") {
        if (currentValue == 0) {
            QMessageBox::warning(this, "错误", "不能除以零！");
            onClearAllClicked();
            return;
        }
        result = storedValue / currentValue;
    }
    
    currentValue = result;
    waitingForOperand = true;
    storedValue = result;
    updateDisplay();
}

void Calculator::onClearClicked()
{
    currentValue = 0;
    hasDecimal = false;
    waitingForOperand = false;
    display->setText("0");
}

void Calculator::onClearAllClicked()
{
    currentValue = 0;
    storedValue = 0;
    currentOperator.clear();
    waitingForOperand = true;
    hasDecimal = false;
    display->setText("0");
}

void Calculator::onBackspaceClicked()
{
    QString text = display->text();
    
    if (text.length() > 1) {
        text.chop(1);
        if (text.endsWith('.')) {
            if(isPlaceholder) {
                text.chop(1);
                isPlaceholder = false;
            } else {
                text += '0';
                isPlaceholder = true;
            }
        }
        display->setText(text);
        currentValue = text.toDouble();
    } else {
        display->setText("0");
        currentValue = 0;
        waitingForOperand = true;
        hasDecimal = false;
    }
}

void Calculator::onDecimalClicked()
{
    QString currentText = display->text();
    
    if (waitingForOperand) {
        // 等待操作数时按小数点，显示"0."
        currentText = "0.0";
        waitingForOperand = false;
        hasDecimal = true;
        isPlaceholder = true;
        display->setText(currentText);
        currentValue = currentText.toDouble();
        return;
    }
    
    // 如果当前文本还没有小数点，添加小数点
    if (!currentText.contains('.')) {
        currentText += ".0";
        hasDecimal = true;
        isPlaceholder = true;
        display->setText(currentText);
        currentValue = currentText.toDouble();
    }
}

void Calculator::onPlusMinusClicked()
{
    currentValue = -currentValue;
    updateDisplay();
}

void Calculator::onPercentClicked()
{
    currentValue = currentValue / 100.0;
    updateDisplay();
}

void Calculator::keyPressEvent(QKeyEvent *event)
{
    // 首先检查是否为数字键
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        int digit = event->key() - Qt::Key_0;
        digitButtons[digit]->click();
        return;
    }
    
    switch (event->key()) {
    case Qt::Key_Plus:
        btnAdd->click();
        break;
        
    case Qt::Key_Minus:
        btnSubtract->click();
        break;
        
    case Qt::Key_Asterisk:
    case Qt::Key_X:
        btnMultiply->click();
        break;
        
    case Qt::Key_Slash:
        btnDivide->click();
        break;
        
    case Qt::Key_Enter:
    case Qt::Key_Return:
    case Qt::Key_Equal:
        btnEquals->click();
        break;
        
    case Qt::Key_Backspace:
        btnBackspace->click();
        break;
        
    case Qt::Key_Delete:
        btnClear->click();
        break;
        
    case Qt::Key_Escape:
        btnClearAll->click();
        break;
        
    case Qt::Key_Period:
    case Qt::Key_Comma:
        btnDecimal->click();
        break;
        
    case Qt::Key_Percent:
        btnPercent->click();
        break;
        
    default:
        QWidget::keyPressEvent(event);
    }
}
