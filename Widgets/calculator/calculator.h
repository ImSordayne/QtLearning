/*
 * @Author: illusoryday 455248805@qq.com
 * @FilePath: \QtLearning\Widgets\calculator\calculator.h
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QVector>
#include <cmath>
#include <memory>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    // 构造函数:初始化计算器界面和状态
    explicit Calculator(QWidget *parent = nullptr);
    // 析构函数:清理资源
    ~Calculator();

private slots:
    // onDigitClicked:处理数字按钮(0-9)点击事件
    void onDigitClicked();
    // onOperatorClicked:处理运算符按钮(+、-、×、÷)点击事件
    void onOperatorClicked();
    // onEqualsClicked:处理等号按钮点击事件,执行计算
    void onEqualsClicked();
    // onClearClicked:处理清除按钮点击事件,清除当前输入
    void onClearClicked();
    // onClearAllClicked:处理全部清除按钮点击事件,重置所有状态
    void onClearAllClicked();
    // onBackspaceClicked:处理退格按钮点击事件,删除最后一个字符
    void onBackspaceClicked();
    // onDecimalClicked:处理小数点按钮点击事件,添加小数点
    void onDecimalClicked();
    // onPlusMinusClicked:处理正负号按钮点击事件,切换当前数值的正负
    void onPlusMinusClicked();
    // onPercentClicked:处理百分号按钮点击事件,将当前数值转换为百分比
    void onPercentClicked();

protected:
    // keyPressEvent:重写键盘事件处理函数,支持键盘输入
    void keyPressEvent(QKeyEvent *event) override;

private:
    // createUI:创建并布局所有UI组件
    void createUI();
    // setupConnections:连接按钮信号与槽函数
    void setupConnections();
    // updateDisplay:更新显示框的内容
    void updateDisplay();
    // calculateResult:根据存储的值、当前值和运算符执行计算
    void calculateResult();
    
    // UI组件
    QLineEdit *display;  // display:显示输入和计算结果的文本框
    
    // 按钮
    QVector<QPushButton*> digitButtons;  // digitButtons:数字按钮(0-9)的集合
    QPushButton *btnDecimal;             // btnDecimal:小数点按钮
    QPushButton *btnEquals;              // btnEquals:等号按钮
    QPushButton *btnClear;               // btnClear:清除当前输入按钮(CE)
    QPushButton *btnClearAll;            // btnClearAll:全部清除按钮(C)
    QPushButton *btnBackspace;           // btnBackspace:退格删除按钮
    QPushButton *btnPlusMinus;           // btnPlusMinus:正负号切换按钮(±)
    QPushButton *btnPercent;             // btnPercent:百分比转换按钮(%)
    
    // 运算符按钮
    QPushButton *btnAdd;                 // btnAdd:加法运算符按钮(+)
    QPushButton *btnSubtract;            // btnSubtract:减法运算符按钮(-)
    QPushButton *btnMultiply;            // btnMultiply:乘法运算符按钮(×)
    QPushButton *btnDivide;              // btnDivide:除法运算符按钮(÷)
    
    // 计算状态
    double currentValue;        // currentValue:当前显示或输入的值
    double storedValue;         // storedValue:存储的上一个操作数,用于连续计算
    QString currentOperator;    // currentOperator:当前选中的运算符
    bool waitingForOperand;     // waitingForOperand:是否等待下一个操作数输入
    bool hasDecimal;            // hasDecimal:当前输入是否已包含小数点
    bool isPlaceholder;         // 是否处于占位状态
};

#endif // CALCULATOR_H