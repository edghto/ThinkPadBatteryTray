/*
 * Start point for application
 */
#include <QtGui>

#include "powerstatus.h"
#include "thinkpadbattery.h"

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(ThinkPadBattery);
    PowerStatus powerStatus;

    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        QMessageBox::critical(0, QObject::tr("ThinkPadBattery"),
                              QObject::tr("I couldn't detect any system tray "
                                          "on this system."));
        return 1;
    }
    QApplication::setQuitOnLastWindowClosed(false);

    ThinkPadBattery thinkPadBattery(&powerStatus);
    thinkPadBattery.hide();
    powerStatus.registerCallback(&thinkPadBattery);
    powerStatus.start();

    return app.exec();
}

