#include "thinkpadbattery.h"

#include <QMenu>
#include <QAction>
#include <QCloseEvent>
#include <QApplication>
#include "powerstatus.h"

#include <QDebug>

ThinkPadBattery::ThinkPadBattery(PowerStatus* powerStatus)
    : m_powerStatus(powerStatus)
{
    createActions();
    createTrayIcon();

    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(this, SIGNAL(lowBatteryLevel()), this, SLOT(showMessage()));

    QIcon icon(":/images/bat_status.png");
    m_trayIcon->setIcon(icon);
    m_trayIcon->setToolTip("25%");
    m_trayIcon->show();

    setWindowIcon(icon);
    setWindowTitle(tr("ThinkPad Battery Status"));
    resize(400, 300);
}

void ThinkPadBattery::notify()
{
    emit lowBatteryLevel();
}

void ThinkPadBattery::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::MiddleClick:
        showMessage();
        break;
    default:
        ;
    }
}

void ThinkPadBattery::showMessage()
{
    QString title("Battery: ");
    QString text("Remaning time: ");

    title.append(QString("%1").arg(m_powerStatus->getPercent())).append("%");

    if(m_powerStatus->isCharging())
    {
        title.append(" [charging]");
        text.append(QString("%1").arg(m_powerStatus->getCharging()));
    }
    else if(m_powerStatus->isRunning())
    {
        text.append(QString("%1").arg(m_powerStatus->getRunning()));
    }
    else
        text.append("Unknown");

    m_trayIcon->showMessage(
            title,
            text.append(" [min]"),
            QSystemTrayIcon::Information,
            5 * 1000);
}


void ThinkPadBattery::closeEvent(QCloseEvent *event)
{
    if (m_trayIcon->isVisible()) {
        hide();
        event->ignore();
    }
}


void ThinkPadBattery::createActions()
{
    m_quitAction = new QAction(tr("&Quit"), this);
    connect(m_quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void ThinkPadBattery::createTrayIcon()
{
    m_trayIconMenu = new QMenu(this);
    m_trayIconMenu->addAction(m_quitAction);

    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setContextMenu(m_trayIconMenu);
}

