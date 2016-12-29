#include <stdio.h>
#include "stepcounter.h"

static const char *SERVICE = SERVICE_NAME;
static const char *PATH = "/";

StepCounter::StepCounter(QObject *parent) :
    QObject(parent)
{
    m_dbusRegistered = false;
    m_isConnected = false;
    m_currentSteps = -1;

    SensorManagerInterface& sm = SensorManagerInterface::instance();

    sm.loadPlugin("stepcountersensor");
    sm.registerSensorInterface<StepCounterSensorChannelInterface>("stepcountersensor");

    sensor = StepCounterSensorChannelInterface::interface("stepcountersensor");

    if (sensor == NULL || !sensor->isValid())
    {
        printf("pedometerd: Unable to get session: %s\n" << sm.errorString();
    }
    else
    {
        sensor->start();
        sensor->setStandbyOverride(true);
        update();
    }
}

StepCounter::~StepCounter()
{
    if (m_dbusRegistered)
    {
        QDBusConnection connection = QDBusConnection::sessionBus();
        connection.unregisterObject(PATH);
        connection.unregisterService(SERVICE);

        printf("pedometerd: unregistered from dbus sessionBus\n");
    }

    if (sensor)
    {
        setAutoUpdate(false);
        sensor->stop();
        delete sensor;
        printf("pedometerd: sensor session removed\n");
    }
}

bool StepCounter::registerDBus()
{
    if (!m_dbusRegistered)
    {
        // DBus
        QDBusConnection connection = QDBusConnection::sessionBus();
        if (!connection.registerService(SERVICE))
        {
            QCoreApplication::quit();
            return false;
        }

        if (!connection.registerObject(PATH, this))
        {
            QCoreApplication::quit();
            return false;
        }
        m_dbusRegistered = true;

        printf("pedometerd: succesfully registered to dbus sessionBus \"%s\"\n", SERVICE);
    }
    return true;
}

void StepCounter::quit()
{
    printf("pedometerd: quit requested from dbus\n");
    QCoreApplication::quit();
}

void StepCounter::update()
{
    if (sensor)
    {
        m_currentSteps = sensor->steps().x();
    }
}

void StepCounter::dataChanged(const Unsigned &data)
{
    m_currentSteps = data.x();

    emit steps(m_currentSteps);
}

int StepCounter::getSteps()
{
    if (!m_isConnected)
        update();

    return m_currentSteps;
}

void StepCounter::setAutoUpdate(const bool &value)
{
    if (sensor)
    {
        if (value == m_isConnected)
            return;

        if (value)
        {
            update();
            connect(sensor, SIGNAL(StepCounterChanged(const Unsigned&)), this, SLOT(dataChanged(const Unsigned&)));
        }
        else
        {
            disconnect(sensor, SIGNAL(StepCounterChanged(const Unsigned&)), this, SLOT(dataChanged(const Unsigned&)));
        }

        m_isConnected = value;
    }
}

QString StepCounter::getVersion()
{
    return APPVERSION;
}
