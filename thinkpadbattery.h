#ifndef THINKPADBATTERY_H
#define THINKPADBATTERY_H

#include <QSystemTrayIcon>
#include <QDialog>

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class PowerStatus;
QT_END_NAMESPACE

class ThinkPadBattery : public QDialog
{
    Q_OBJECT

public:
    ThinkPadBattery(PowerStatus* powerStatus);
    void notify();

signals:
    void lowBatteryLevel();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void showMessage();

private:
    void createActions();
    void createTrayIcon();

    QAction *m_quitAction;
    QSystemTrayIcon *m_trayIcon;
    QMenu *m_trayIconMenu;
    PowerStatus* m_powerStatus;
};

#endif // THINKPADBATTERY_H
