#include "powerstatus.h"

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMutexLocker>
#include "thinkpadbattery.h"

const QString PowerStatus::PERCENT  = "/sys/devices/platform/smapi/BAT0/remaining_percent";
const QString PowerStatus::CHARGING = "/sys/devices/platform/smapi/BAT0/remaining_charging_time";
const QString PowerStatus::RUNNING  = "/sys/devices/platform/smapi/BAT0/remaining_running_time_now";

PowerStatus::PowerStatus()
    : m_percent(0), m_charging(0), m_running(0),
    m_treshold(5), m_isCharging(false), m_isRunning(false),
    m_listener(NULL)
{
}

void PowerStatus::registerLowBatterySlot(ThinkPadBattery* thinkPad)
{
    m_listener = thinkPad;
    QObject::connect(this, SIGNAL(lowBatteryLevel()), m_listener, SLOT(showMessage()));
    QObject::connect(this, SIGNAL(newBatterLevel()), m_listener, SLOT(updatedTray()));
}

void PowerStatus::run()
{
    for(;;)
    {
        bool ok = false;
        unsigned int percent, charging, running;
        bool isRunning, isCharging;

        percent    = getStat(PowerStatus::PERCENT).toInt(&ok);
        charging   = getStat(PowerStatus::CHARGING).toInt(&ok);
        isCharging = ok;
        running    = getStat(PowerStatus::RUNNING).toInt(&ok);
        isRunning  = ok;

        {
            QMutexLocker locker(&m_mutex);
            m_percent    = percent;
            m_charging   = charging;
            m_running    = running;
            m_isCharging = isCharging;
            m_isRunning  = isRunning;
        }

        if(m_percent <= m_treshold && !m_isCharging && m_listener)
        {
            emit lowBatteryLevel();
        }
        emit newBatterLevel();

        msleep( 10 * 1000);
    }
}

unsigned int PowerStatus::getPercent()
{
    QMutexLocker locker(&m_mutex);
    return m_percent;
}

unsigned int PowerStatus::getCharging()
{
    QMutexLocker locker(&m_mutex);
    return m_charging;
}

unsigned int PowerStatus::getRunning()
{
    QMutexLocker locker(&m_mutex);
    return m_running;
}

bool PowerStatus::isCharging()
{
    QMutexLocker locker(&m_mutex);
    return m_isCharging;
}

bool PowerStatus::isRunning()
{
    QMutexLocker locker(&m_mutex);
    return m_isRunning;
}

QString PowerStatus::getStat(const QString& stat)
{
    QFile statFile(stat);
    statFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&statFile);
    return in.readLine();
}
