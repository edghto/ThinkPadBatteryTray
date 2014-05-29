#ifndef POWERSTATUS_H
#define POWERSTATUS_H

#include <QThread>
#include <QMutex>
#include <QObject>

QT_BEGIN_NAMESPACE
class QString;
class ThinkPadBattery;
QT_END_NAMESPACE

class PowerStatus : public QThread
{
    Q_OBJECT
public:
    PowerStatus();
    void run();
    void registerLowBatterySlot(ThinkPadBattery* thinkPad);

    /*
     * Return status of battery in percantage
     */
    unsigned int getPercent();

    /*
     * Return remaining charging time in minutes (if is charging)
     */
    unsigned int getCharging();

    /*
     * Return remaining running time in minutes (if battery is used)
     */
    unsigned int getRunning();

    bool isCharging();
    bool isRunning();

    static const QString PERCENT;
    static const QString CHARGING;
    static const QString RUNNING;

signals:
    void lowBatteryLevel();
    void newBatterLevel();

protected:
    unsigned int m_percent;
    unsigned int m_charging;
    unsigned int m_running;
    unsigned int m_treshold;

    bool m_isCharging;
    bool m_isRunning;

    ThinkPadBattery* m_listener;
    unsigned int m_debug;

    QMutex m_mutex;
    QString getStat(const QString& stat);
};

#endif // POWERSTATUS_H
