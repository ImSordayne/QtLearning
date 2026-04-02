#include "mainwindow.h"
#include <QApplication>
#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <cmath>
#include <algorithm>

// ==================== SidebarButton 实现 ====================

SidebarButton::SidebarButton(const QString& iconPath, const QString& text, QWidget *parent)
    : QWidget(parent)
    , m_iconPath(iconPath)
    , m_text(text)
    , m_isHovered(false)
    , m_isActive(false)
    , m_expandedWidth(200)
{
    setFixedHeight(60);
    setMinimumWidth(200);  // 最小宽度设为200，确保显示文字
    setMaximumWidth(300);
    
    // 加载图标
    m_icon.load(iconPath);
    if (m_icon.isNull()) {
        m_icon = QPixmap(32, 32);
        m_icon.fill(Qt::transparent);
        QPainter painter(&m_icon);
        painter.setPen(Qt::white);
        painter.drawText(m_icon.rect(), Qt::AlignCenter, text.left(1));
    }
    
    // 设置颜色
    m_activeColor = QColor(52, 152, 219);    // 激活状态 - 蓝色
    m_hoverColor = QColor(41, 128, 185);     // 悬停状态 - 深蓝色
    m_normalColor = QColor(44, 62, 80);      // 正常状态 - 深灰色
    
    // 设置鼠标跟踪
    setMouseTracking(true);
    
    // 设置样式
    setStyleSheet("background: transparent;");
}

void SidebarButton::setExpandedWidth(int width)
{
    m_expandedWidth = width;
    update();
}

void SidebarButton::setActive(bool active)
{
    m_isActive = active;
    update();
}

void SidebarButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
    QWidget::mousePressEvent(event);
}

void SidebarButton::enterEvent(QEnterEvent *event)
{
    m_isHovered = true;
    update();
    QWidget::enterEvent(event);
}

void SidebarButton::leaveEvent(QEvent *event)
{
    m_isHovered = false;
    update();
    QWidget::leaveEvent(event);
}

void SidebarButton::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    int currentWidth = width();
    
    QColor backgroundColor;
    if (m_isActive) {
        backgroundColor = m_activeColor;
    } else if (m_isHovered) {
        backgroundColor = m_hoverColor;
    } else {
        backgroundColor = m_normalColor;
    }
    
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    
    QRect backgroundRect(0, 0, currentWidth, height());
    painter.drawRoundedRect(backgroundRect, 8, 8);
    
    QRect iconRect(10, (height() - 32) / 2, 32, 32);
    painter.drawPixmap(iconRect, m_icon);
    
    // 始终显示文字
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(11);
    font.setBold(true);
    painter.setFont(font);
    
    QRect textRect(52, 0, currentWidth - 60, height());
    painter.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, m_text);
    
    QWidget::paintEvent(event);
}

// ==================== ExpandingSidebar 实现 ====================

ExpandingSidebar::ExpandingSidebar(QWidget *parent)
    : QWidget(parent)
    , m_activeButton(nullptr)
    , m_isExpanded(true)  // 默认展开
{
    setFixedWidth(200);  // 默认展开宽度
    setStyleSheet(
        "QWidget {"
        "    background-color: #2c3e50;"
        "    border-right: 2px solid #34495e;"
        "}"
    );
    
    m_layout = new QVBoxLayout(this);
    m_layout->setSpacing(10);
    m_layout->setContentsMargins(0, 20, 0, 20);
    m_layout->setAlignment(Qt::AlignTop);
    
    m_animation = new QPropertyAnimation(this, "width");
    m_animation->setDuration(300);
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    
    setMouseTracking(true);
}

void ExpandingSidebar::addButton(SidebarButton *button)
{
    m_buttons.append(button);
    m_layout->addWidget(button);
    
    connect(button, &SidebarButton::clicked, this, &ExpandingSidebar::onButtonClicked);
}

void ExpandingSidebar::setActiveButton(SidebarButton *button)
{
    for (SidebarButton *btn : m_buttons) {
        if (btn != button) {
            btn->setActive(false);
        }
    }
    
    if (button) {
        button->setActive(true);
        m_activeButton = button;
    }
}

void ExpandingSidebar::enterEvent(QEnterEvent *event)
{
    // 展开侧边栏
    if (!m_isExpanded) {
        m_isExpanded = true;
        
        m_animation->stop();
        m_animation->setStartValue(width());
        m_animation->setEndValue(200);
        m_animation->start();
    }
    
    QWidget::enterEvent(event);
}

void ExpandingSidebar::leaveEvent(QEvent *event)
{
    // 检查鼠标是否还在侧边栏区域内
    QPoint globalPos = QCursor::pos();
    QPoint localPos = mapFromGlobal(globalPos);
    
    if (rect().contains(localPos)) {
        QWidget::leaveEvent(event);
        return;
    }
    
    // 检查鼠标是否在按钮上
    bool mouseOnButton = false;
    for (SidebarButton *button : m_buttons) {
        QPoint buttonLocalPos = button->mapFromGlobal(globalPos);
        if (button->rect().contains(buttonLocalPos)) {
            mouseOnButton = true;
            break;
        }
    }
    
    // 如果鼠标不在任何按钮上，收缩侧边栏
    if (!mouseOnButton) {
        m_isExpanded = false;
        
        m_animation->stop();
        m_animation->setStartValue(width());
        m_animation->setEndValue(60);
        m_animation->start();
    }
    
    QWidget::leaveEvent(event);
}

void ExpandingSidebar::onButtonClicked()
{
    SidebarButton *button = qobject_cast<SidebarButton*>(sender());
    if (button) {
        setActiveButton(button);
        emit widthChanged(width());
    }
}

// ==================== ChartWidget 实现 ====================

ChartWidget::ChartWidget(QWidget *parent)
    : QWidget(parent)
    , m_chartType(ChartType::DONUT_CHART)
    , m_value(0.0)
    , m_animationValue(0.0)
    , m_total(100.0)
    , m_title("")
    , m_unit("%")
    , m_maxDataPoints(60)
{
    m_chartColor = QColor(52, 152, 219);    // 蓝色
    m_gridColor = QColor(220, 220, 220);    // 浅灰色
    m_textColor = QColor(44, 62, 80);       // 深蓝色
    m_bgColor = QColor(255, 255, 255);      // 白色
    
    // 初始化数据点
    for (int i = 0; i < m_maxDataPoints; ++i) {
        m_dataPoints.append(0.0);
    }
    
    // 初始化动画
    m_animation = new QPropertyAnimation(this, "animationValue");
    m_animation->setDuration(800);  // 动画持续时间800ms
    m_animation->setEasingCurve(QEasingCurve::OutCubic);
    
    setMinimumSize(400, 300);
}

void ChartWidget::setAnimationValue(double value)
{
    m_animationValue = value;
    update();  // 触发重绘
}

void ChartWidget::setValue(double value)
{
    m_value = qBound(0.0, value, 100.0);
    addDataPoint(m_value);
    
    // 启动动画从当前值到目标值
    m_animation->stop();
    m_animation->setStartValue(m_animationValue);
    m_animation->setEndValue(m_value);
    m_animation->start();
}

void ChartWidget::setChartType(ChartType type)
{
    m_chartType = type;
    update();
}

void ChartWidget::setTitle(const QString &title)
{
    m_title = title;
    update();
}

void ChartWidget::setUnit(const QString &unit)
{
    m_unit = unit;
    update();
}

void ChartWidget::setTotal(double total)
{
    m_total = total;
    update();
}

void ChartWidget::addDataPoint(double value)
{
    m_dataPoints.append(value);
    while (m_dataPoints.size() > m_maxDataPoints) {
        m_dataPoints.removeFirst();
    }
    update();
}

void ChartWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    
    // 计算图表区域
    int margin = 20;
    m_chartRect = QRectF(margin, margin, 
                        width() - 2 * margin, 
                        height() - 2 * margin);
}

void ChartWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // 绘制背景
    painter.fillRect(rect(), m_bgColor);
    
    if (m_chartType == ChartType::DONUT_CHART) {
        drawDonutChart(painter);
    } else {
        drawLineChart(painter);
    }
}

void ChartWidget::drawRoundedDonutEnd(QPainter &painter, const QPointF &center,
                                       float outerRadius, float innerRadius,
                                       float startAngle, float sweepAngle)
{
    // 计算圆弧终点的位置
    float endAngleRad = (startAngle - sweepAngle) * M_PI / 180.0;
    float midRadius = (outerRadius + innerRadius) / 2.0;
    
    // 终点圆心位置
    QPointF endCenter(center.x() + midRadius * cos(endAngleRad),
                      center.y() - midRadius * sin(endAngleRad));
    
    // 绘制圆形端点
    float endRadius = (outerRadius - innerRadius) / 2.0;
    painter.drawEllipse(endCenter, endRadius, endRadius);
}

void ChartWidget::drawDonutChart(QPainter &painter)
{
    // 计算圆环参数
    QPointF center = m_chartRect.center();
    float outerRadius = qMin(m_chartRect.width(), m_chartRect.height()) * 0.4;
    float innerRadius = outerRadius * 0.6;
    
    // 绘制背景圆环
    painter.setPen(Qt::NoPen);
    painter.setBrush(m_gridColor);
    painter.drawEllipse(center, outerRadius, outerRadius);
    
    // 绘制内圆（挖孔效果）
    painter.setBrush(m_bgColor);
    painter.drawEllipse(center, innerRadius, innerRadius);
    
    // 使用动画值绘制进度圆环
    float angle = 360.0 * (m_animationValue / 100.0);
    
    if (angle > 0) {
        QLinearGradient gradient(center.x() - outerRadius, center.y(),
                                center.x() + outerRadius, center.y());
        gradient.setColorAt(0, m_chartColor.lighter(120));
        gradient.setColorAt(1, m_chartColor.darker(120));
        
        // 设置画笔 - 使用 RoundCap 自动产生圆形端点
        QPen pen;
        pen.setCapStyle(Qt::RoundCap);  // 圆头端点
        pen.setBrush(gradient);
        pen.setWidth(outerRadius - innerRadius);
        
        painter.setPen(pen);
        painter.setBrush(Qt::NoBrush);
        
        // 绘制圆环弧线
        float radius = (outerRadius + innerRadius) / 2.0;
        QRectF arcRect(center.x() - radius, center.y() - radius,
                       radius * 2, radius * 2);
        
        // 从顶部开始（-90度），顺时针绘制
        // 注意：drawArc 的角度单位是1/16度
        painter.drawArc(arcRect, -90 * 16, -angle * 16);
    }
    
    // 绘制中心文字
    painter.setPen(m_textColor);
    QFont font = painter.font();
    font.setPointSize(16);
    font.setBold(true);
    painter.setFont(font);
    
    QString valueText = QString("%1%2").arg(m_animationValue, 0, 'f', 1).arg(m_unit);
    QRectF textRect(center.x() - outerRadius, center.y() - 30,
                    outerRadius * 2, 60);
    painter.drawText(textRect, Qt::AlignCenter, valueText);
    
    font.setPointSize(12);
    font.setBold(false);
    painter.setFont(font);
    
    if (!m_title.isEmpty()) {
        QRectF titleRect(center.x() - outerRadius, center.y() + 40,
                        outerRadius * 2, 30);
        painter.drawText(titleRect, Qt::AlignCenter, m_title);
    }
}

void ChartWidget::drawLineChart(QPainter &painter)
{
    // 绘制网格
    drawGrid(painter);
    
    if (m_dataPoints.isEmpty()) return;
    
    // 计算数据点位置
    QVector<QPointF> points;
    double maxValue = 100.0;
    
    float xStep = m_chartRect.width() / (m_maxDataPoints - 1);
    
    for (int i = 0; i < m_dataPoints.size(); ++i) {
        float x = m_chartRect.left() + i * xStep;
        float y = m_chartRect.bottom() - (m_dataPoints[i] / maxValue) * m_chartRect.height();
        points.append(QPointF(x, y));
    }
    
    // 绘制折线（使用圆头端点）
    QPen linePen(m_chartColor);
    linePen.setWidth(3);
    linePen.setCapStyle(Qt::RoundCap);
    linePen.setJoinStyle(Qt::RoundJoin);
    painter.setPen(linePen);
    
    for (int i = 0; i < points.size() - 1; ++i) {
        painter.drawLine(points[i], points[i + 1]);
    }
    
    // 绘制数据点
    painter.setBrush(m_chartColor);
    for (int i = 0; i < points.size(); ++i) {
        if (i % 5 == 0 || i == points.size() - 1) {
            painter.drawEllipse(points[i], 4, 4);
        }
    }
    
    // 绘制标题和数值
    painter.setPen(m_textColor);
    QFont font = painter.font();
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);
    
    QString titleText = QString("%1 - Current: %2%3")
                        .arg(m_title)
                        .arg(m_animationValue, 0, 'f', 1)
                        .arg(m_unit);
    
    QRectF titleRect(m_chartRect.left(), m_chartRect.top() - 30,
                    m_chartRect.width(), 25);
    painter.drawText(titleRect, Qt::AlignLeft | Qt::AlignVCenter, titleText);
}

void ChartWidget::drawGrid(QPainter &painter)
{
    painter.setPen(QPen(m_gridColor, 1, Qt::DotLine));
    
    // 绘制水平网格线
    int horizontalLines = 5;
    for (int i = 0; i <= horizontalLines; ++i) {
        float y = m_chartRect.top() + (m_chartRect.height() / horizontalLines) * i;
        painter.drawLine(m_chartRect.left(), y, m_chartRect.right(), y);
    }
    
    // 绘制垂直网格线
    int verticalLines = 10;
    for (int i = 0; i <= verticalLines; ++i) {
        float x = m_chartRect.left() + (m_chartRect.width() / verticalLines) * i;
        painter.drawLine(x, m_chartRect.top(), x, m_chartRect.bottom());
    }
    
    // 绘制边框
    painter.setPen(QPen(m_textColor, 2));
    painter.drawRect(m_chartRect);
}

// ==================== CPUMonitor 实现 ====================

CPUMonitor::CPUMonitor(QWidget *parent)
    : QWidget(parent)
    , m_currentUsage(0.0)
{
    // 主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // 标题栏布局
    m_headerLayout = new QHBoxLayout();
    
    m_titleLabel = new QLabel("CPU Usage Monitor");
    m_titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 28px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "}"
    );
    
    // 图表切换按钮
    m_chartToggle = new QCheckBox("Show Line Chart");
    m_chartToggle->setStyleSheet(
        "QCheckBox {"
        "    font-size: 14px;"
        "    color: #34495e;"
        "    spacing: 10px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 20px;"
        "    height: 20px;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: #3498db;"
        "    border: 2px solid #2980b9;"
        "}"
    );
    
    // 历史数据滑块
    m_historySlider = new QSlider(Qt::Horizontal);
    m_historySlider->setRange(10, 120);
    m_historySlider->setValue(60);
    m_historySlider->setMaximumWidth(200);
    m_historySlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #bbb;"
        "    background: white;"
        "    height: 10px;"
        "    border-radius: 5px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #3498db;"
        "    border: 1px solid #777;"
        "    height: 10px;"
        "    border-radius: 5px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #2c3e50;"
        "    width: 20px;"
        "    height: 20px;"
        "    margin: -5px 0;"
        "    border-radius: 10px;"
        "}"
    );
    
    QLabel *sliderLabel = new QLabel("History (sec):");
    sliderLabel->setStyleSheet("color: #34495e; font-size: 14px;");
    
    // 添加到标题栏
    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();
    m_headerLayout->addWidget(sliderLabel);
    m_headerLayout->addWidget(m_historySlider);
    m_headerLayout->addSpacing(20);
    m_headerLayout->addWidget(m_chartToggle);
    
    // 图表区域
    m_chartLayout = new QVBoxLayout();
    
    m_chartWidget = new ChartWidget();
    m_chartWidget->setTitle("CPU Usage");
    m_chartWidget->setUnit("%");
    
    m_chartLayout->addWidget(m_chartWidget);
    
    // 添加到主布局
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addLayout(m_chartLayout, 1);
    
    // 设置定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &CPUMonitor::updateCPUUsage);
    m_timer->start(1000);
    
    // 初始化CPU数据
#ifdef _WIN32
    m_prevIdleTime.dwLowDateTime = m_prevIdleTime.dwHighDateTime = 0;
    m_prevKernelTime.dwLowDateTime = m_prevKernelTime.dwHighDateTime = 0;
    m_prevUserTime.dwLowDateTime = m_prevUserTime.dwHighDateTime = 0;
#endif
    
    updateCPUUsage();
}

void CPUMonitor::setChartType(ChartType type)
{
    m_chartWidget->setChartType(type);
}

void CPUMonitor::updateCPUUsage()
{
    double cpuUsage = 0.0;
    
#ifdef _WIN32
    FILETIME idleTime, kernelTime, userTime;
    
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        if (m_prevIdleTime.dwLowDateTime != 0 || m_prevIdleTime.dwHighDateTime != 0) {
            ULONGLONG idle = ((ULONGLONG)idleTime.dwHighDateTime << 32) | idleTime.dwLowDateTime;
            ULONGLONG kernel = ((ULONGLONG)kernelTime.dwHighDateTime << 32) | kernelTime.dwLowDateTime;
            ULONGLONG user = ((ULONGLONG)userTime.dwHighDateTime << 32) | userTime.dwLowDateTime;
            
            ULONGLONG prevIdle = ((ULONGLONG)m_prevIdleTime.dwHighDateTime << 32) | m_prevIdleTime.dwLowDateTime;
            ULONGLONG prevKernel = ((ULONGLONG)m_prevKernelTime.dwHighDateTime << 32) | m_prevKernelTime.dwLowDateTime;
            ULONGLONG prevUser = ((ULONGLONG)m_prevUserTime.dwHighDateTime << 32) | m_prevUserTime.dwLowDateTime;
            
            ULONGLONG sys = (kernel - prevKernel) + (user - prevUser);
            ULONGLONG idleDelta = idle - prevIdle;
            
            if (sys > 0) {
                cpuUsage = ((double)(sys - idleDelta) / sys) * 100.0;
            }
        }
        
        m_prevIdleTime = idleTime;
        m_prevKernelTime = kernelTime;
        m_prevUserTime = userTime;
    }
#elif defined(__linux__)
    FILE* file = fopen("/proc/stat", "r");
    if (file) {
        char buffer[256];
        fgets(buffer, sizeof(buffer), file);
        fclose(file);
        
        long user, nice, system, idle, iowait, irq, softirq;
        sscanf(buffer, "cpu %ld %ld %ld %ld %ld %ld %ld", 
               &user, &nice, &system, &idle, &iowait, &irq, &softirq);
        
        static long prevTotal = 0, prevIdle = 0;
        
        long total = user + nice + system + idle + iowait + irq + softirq;
        long idleTotal = idle;
        
        if (prevTotal > 0) {
            long totalDiff = total - prevTotal;
            long idleDiff = idleTotal - prevIdle;
            
            if (totalDiff > 0) {
                cpuUsage = ((double)(totalDiff - idleDiff) / totalDiff) * 100.0;
            }
        }
        
        prevTotal = total;
        prevIdle = idleTotal;
    }
#else
    static double simulatedUsage = 0.0;
    simulatedUsage = fmod(simulatedUsage + QRandomGenerator::global()->bounded(-5.0, 10.0), 100.0);
    if (simulatedUsage < 0) simulatedUsage = 0;
    if (simulatedUsage > 100) simulatedUsage = 100;
    cpuUsage = simulatedUsage;
#endif
    
    m_currentUsage = cpuUsage;
    m_chartWidget->setValue(cpuUsage);
}

// ==================== MemoryMonitor 实现 ====================

MemoryMonitor::MemoryMonitor(QWidget *parent)
    : QWidget(parent)
    , m_currentUsage(0.0)
    , m_totalMemory(0.0)
    , m_usedMemory(0.0)
{
    // 主布局
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(20);
    m_mainLayout->setContentsMargins(30, 30, 30, 30);
    
    // 标题栏布局
    m_headerLayout = new QHBoxLayout();
    
    m_titleLabel = new QLabel("Memory Usage Monitor");
    m_titleLabel->setStyleSheet(
        "QLabel {"
        "    font-size: 28px;"
        "    font-weight: bold;"
        "    color: #2c3e50;"
        "}"
    );
    
    // 图表切换按钮
    m_chartToggle = new QCheckBox("Show Line Chart");
    m_chartToggle->setStyleSheet(
        "QCheckBox {"
        "    font-size: 14px;"
        "    color: #34495e;"
        "    spacing: 10px;"
        "}"
        "QCheckBox::indicator {"
        "    width: 20px;"
        "    height: 20px;"
        "}"
        "QCheckBox::indicator:checked {"
        "    background-color: #9b59b6;"
        "    border: 2px solid #8e44ad;"
        "}"
    );
    
    // 历史数据滑块
    m_historySlider = new QSlider(Qt::Horizontal);
    m_historySlider->setRange(10, 120);
    m_historySlider->setValue(60);
    m_historySlider->setMaximumWidth(200);
    m_historySlider->setStyleSheet(
        "QSlider::groove:horizontal {"
        "    border: 1px solid #bbb;"
        "    background: white;"
        "    height: 10px;"
        "    border-radius: 5px;"
        "}"
        "QSlider::sub-page:horizontal {"
        "    background: #9b59b6;"
        "    border: 1px solid #777;"
        "    height: 10px;"
        "    border-radius: 5px;"
        "}"
        "QSlider::handle:horizontal {"
        "    background: #2c3e50;"
        "    width: 20px;"
        "    height: 20px;"
        "    margin: -5px 0;"
        "    border-radius: 10px;"
        "}"
    );
    
    QLabel *sliderLabel = new QLabel("History (sec):");
    sliderLabel->setStyleSheet("color: #34495e; font-size: 14px;");
    
    // 添加到标题栏
    m_headerLayout->addWidget(m_titleLabel);
    m_headerLayout->addStretch();
    m_headerLayout->addWidget(sliderLabel);
    m_headerLayout->addWidget(m_historySlider);
    m_headerLayout->addSpacing(20);
    m_headerLayout->addWidget(m_chartToggle);
    
    // 图表区域
    m_chartLayout = new QVBoxLayout();
    
    m_chartWidget = new ChartWidget();
    m_chartWidget->setTitle("Memory Usage");
    m_chartWidget->setUnit("%");
    
    m_chartLayout->addWidget(m_chartWidget);
    
    // 添加到主布局
    m_mainLayout->addLayout(m_headerLayout);
    m_mainLayout->addLayout(m_chartLayout, 1);
    
    // 设置定时器
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MemoryMonitor::updateMemoryUsage);
    m_timer->start(1000);
    
    updateMemoryUsage();
}

void MemoryMonitor::setChartType(ChartType type)
{
    m_chartWidget->setChartType(type);
}

void MemoryMonitor::updateMemoryUsage()
{
    double memoryUsage = 0.0;
    double totalMemory = 0.0;
    double usedMemory = 0.0;
    
#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&memInfo)) {
        totalMemory = (double)memInfo.ullTotalPhys / (1024 * 1024 * 1024);
        usedMemory = (double)(memInfo.ullTotalPhys - memInfo.ullAvailPhys) / (1024 * 1024 * 1024);
        memoryUsage = (usedMemory / totalMemory) * 100.0;
    }
#elif defined(__linux__)
    struct sysinfo memInfo;
    if (sysinfo(&memInfo) == 0) {
        totalMemory = (double)memInfo.totalram * memInfo.mem_unit / (1024 * 1024 * 1024);
        usedMemory = (double)(memInfo.totalram - memInfo.freeram) * memInfo.mem_unit / (1024 * 1024 * 1024);
        memoryUsage = (usedMemory / totalMemory) * 100.0;
    }
#else
    static double simulatedUsage = 0.0;
    simulatedUsage = fmod(simulatedUsage + QRandomGenerator::global()->bounded(-2.0, 5.0), 100.0);
    if (simulatedUsage < 0) simulatedUsage = 0;
    if (simulatedUsage > 100) simulatedUsage = 100;
    memoryUsage = simulatedUsage;
    totalMemory = 16.0;
    usedMemory = totalMemory * memoryUsage / 100.0;
#endif
    
    m_currentUsage = memoryUsage;
    m_totalMemory = totalMemory;
    m_usedMemory = usedMemory;
    
    m_chartWidget->setValue(memoryUsage);
    
    // 更新图表标题显示详细信息
    QString details = QString("Memory: %1 GB / %2 GB")
                      .arg(m_usedMemory, 0, 'f', 2)
                      .arg(m_totalMemory, 0, 'f', 2);
    m_chartWidget->setTitle(details);
}

// ==================== MainWindow 实现 ====================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    
    setWindowTitle("System Monitor");
    resize(1200, 800);
    
    setStyleSheet(
        "QMainWindow {"
        "    background-color: #f5f7fa;"
        "}"
    );
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    m_centralWidget = new QWidget();
    setCentralWidget(m_centralWidget);
    
    m_mainLayout = new QHBoxLayout(m_centralWidget);
    m_mainLayout->setSpacing(0);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    
    createSidebar();
    createContentArea();
}

void MainWindow::createSidebar()
{
    m_sidebar = new ExpandingSidebar();
    
    m_cpuButton = new SidebarButton("res/cpu.png", "CPU Usage");
    m_memoryButton = new SidebarButton("res/memory.png", "Memory Usage");
    
    m_sidebar->addButton(m_cpuButton);
    m_sidebar->addButton(m_memoryButton);
    
    connect(m_cpuButton, &SidebarButton::clicked, this, &MainWindow::onCpuButtonClicked);
    connect(m_memoryButton, &SidebarButton::clicked, this, &MainWindow::onMemoryButtonClicked);
    
    m_mainLayout->addWidget(m_sidebar);
}

void MainWindow::createContentArea()
{
    m_contentStack = new QStackedWidget();
    m_contentStack->setStyleSheet(
        "QStackedWidget {"
        "    background-color: #f5f7fa;"
        "}"
    );
    
    m_cpuMonitor = new CPUMonitor();
    m_memoryMonitor = new MemoryMonitor();
    
    connect(m_cpuMonitor->chartToggle(), &QCheckBox::toggled, 
            this, &MainWindow::onCpuChartToggle);
    connect(m_cpuMonitor->historySlider(), &QSlider::valueChanged,
            this, &MainWindow::onCpuHistoryChanged);
    
    connect(m_memoryMonitor->chartToggle(), &QCheckBox::toggled,
            this, &MainWindow::onMemoryChartToggle);
    connect(m_memoryMonitor->historySlider(), &QSlider::valueChanged,
            this, &MainWindow::onMemoryHistoryChanged);
    
    m_contentStack->addWidget(m_cpuMonitor);
    m_contentStack->addWidget(m_memoryMonitor);
    
    m_mainLayout->addWidget(m_contentStack, 1);
    
    onCpuButtonClicked();
}

void MainWindow::onCpuButtonClicked()
{
    m_contentStack->setCurrentWidget(m_cpuMonitor);
    m_sidebar->setActiveButton(m_cpuButton);
}

void MainWindow::onMemoryButtonClicked()
{
    m_contentStack->setCurrentWidget(m_memoryMonitor);
    m_sidebar->setActiveButton(m_memoryButton);
}

void MainWindow::onCpuChartToggle(bool checked)
{
    ChartType type = checked ? ChartType::LINE_CHART : ChartType::DONUT_CHART;
    m_cpuMonitor->setChartType(type);
}

void MainWindow::onMemoryChartToggle(bool checked)
{
    ChartType type = checked ? ChartType::LINE_CHART : ChartType::DONUT_CHART;
    m_memoryMonitor->setChartType(type);
}

void MainWindow::onCpuHistoryChanged(int value)
{
    qDebug() << "CPU history changed to:" << value << "seconds";
}

void MainWindow::onMemoryHistoryChanged(int value)
{
    qDebug() << "Memory history changed to:" << value << "seconds";
}