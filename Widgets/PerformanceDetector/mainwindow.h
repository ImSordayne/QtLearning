#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QMouseEvent>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPixmap>
#include <QSlider>
#include <QCheckBox>
#include <QVector>
#include <QPointF>
#include <QPen>
#include <QBrush>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QTime>
#include <QRandomGenerator>
#include <QPainterPath>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <sys/sysinfo.h>
#endif

// ==================== 图表类型枚举 ====================
enum class ChartType {
    DONUT_CHART,    // 圆环图
    LINE_CHART      // 折线图
};

// ==================== 侧边栏按钮 ====================
class SidebarButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int expandedWidth READ expandedWidth WRITE setExpandedWidth)

public:
    explicit SidebarButton(const QString& iconPath, const QString& text, QWidget *parent = nullptr);
    
    int expandedWidth() const { return m_expandedWidth; }
    void setExpandedWidth(int width);
    
    void setActive(bool active);
    bool isActive() const { return m_isActive; }
    
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_iconPath;
    QString m_text;
    QPixmap m_icon;
    bool m_isHovered;
    bool m_isActive;
    int m_expandedWidth;
    QColor m_activeColor;
    QColor m_hoverColor;
    QColor m_normalColor;
};

// ==================== 可扩展侧边栏 ====================
class ExpandingSidebar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setFixedWidth)

public:
    explicit ExpandingSidebar(QWidget *parent = nullptr);
    
    void addButton(SidebarButton *button);
    void setActiveButton(SidebarButton *button);
    
signals:
    void widthChanged(int width);
    
protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    
private slots:
    void onButtonClicked();
    
private:
    QVBoxLayout *m_layout;
    QVector<SidebarButton*> m_buttons;
    SidebarButton *m_activeButton;
    QPropertyAnimation *m_animation;
    bool m_isExpanded;
};

// ==================== 自定义图表控件 ====================

class ChartWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double animationValue READ animationValue WRITE setAnimationValue)

public:
    explicit ChartWidget(QWidget *parent = nullptr);
    
    void setChartType(ChartType type);
    void setValue(double value);
    void setTitle(const QString &title);
    void setUnit(const QString &unit);
    void setTotal(double total);
    void addDataPoint(double value);
    
    double animationValue() const { return m_animationValue; }
    void setAnimationValue(double value);
    
protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    
private:
    void drawDonutChart(QPainter &painter);
    void drawLineChart(QPainter &painter);
    void drawGrid(QPainter &painter);
    void drawRoundedDonutEnd(QPainter &painter, const QPointF &center, 
                              float outerRadius, float innerRadius, 
                              float startAngle, float sweepAngle);
    
    ChartType m_chartType;
    double m_value;
    double m_animationValue;
    double m_total;
    QString m_title;
    QString m_unit;
    QVector<double> m_dataPoints;
    QColor m_chartColor;
    QColor m_gridColor;
    QColor m_textColor;
    QColor m_bgColor;
    
    int m_maxDataPoints;
    QRectF m_chartRect;
    
    QPropertyAnimation *m_animation;
};

// ==================== CPU监控页面 ====================
class CPUMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit CPUMonitor(QWidget *parent = nullptr);
    
    void setChartType(ChartType type);
    
    QCheckBox* chartToggle() const { return m_chartToggle; }
    QSlider* historySlider() const { return m_historySlider; }
    
public slots:
    void updateCPUUsage();

private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;
    QVBoxLayout *m_chartLayout;
    
    QLabel *m_titleLabel;
    QCheckBox *m_chartToggle;
    QSlider *m_historySlider;
    
    ChartWidget *m_chartWidget;
    QTimer *m_timer;
    
    double m_currentUsage;
    
#ifdef _WIN32
    FILETIME m_prevIdleTime;
    FILETIME m_prevKernelTime;
    FILETIME m_prevUserTime;
#endif
};

// ==================== 内存监控页面 ====================
class MemoryMonitor : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryMonitor(QWidget *parent = nullptr);
    
    void setChartType(ChartType type);
    
    QCheckBox* chartToggle() const { return m_chartToggle; }
    QSlider* historySlider() const { return m_historySlider; }
    
public slots:
    void updateMemoryUsage();

private:
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_headerLayout;
    QVBoxLayout *m_chartLayout;
    
    QLabel *m_titleLabel;
    QCheckBox *m_chartToggle;
    QSlider *m_historySlider;
    
    ChartWidget *m_chartWidget;
    QTimer *m_timer;
    
    double m_currentUsage;
    double m_totalMemory;
    double m_usedMemory;
};

// ==================== 主窗口 ====================
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onCpuButtonClicked();
    void onMemoryButtonClicked();
    void onCpuChartToggle(bool checked);
    void onMemoryChartToggle(bool checked);
    void onCpuHistoryChanged(int value);
    void onMemoryHistoryChanged(int value);

private:
    void setupUI();
    void createSidebar();
    void createContentArea();
    
    QWidget *m_centralWidget;
    QHBoxLayout *m_mainLayout;
    
    ExpandingSidebar *m_sidebar;
    SidebarButton *m_cpuButton;
    SidebarButton *m_memoryButton;
    
    QStackedWidget *m_contentStack;
    CPUMonitor *m_cpuMonitor;
    MemoryMonitor *m_memoryMonitor;
};

#endif // MAINWINDOW_H