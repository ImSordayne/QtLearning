#include <QObject>
#include <QSystemTrayIcon>
#include <QMessageBox>
#include "watermark.h"

class sysTray : public QObject {
    Q_OBJECT
    QSystemTrayIcon *trayIcon;
    watermark *mark;

public slots:
    void onIconClicked(QSystemTrayIcon::ActivationReason reason);
public:
    sysTray(watermark &mark);
};