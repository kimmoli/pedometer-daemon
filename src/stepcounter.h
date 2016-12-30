#ifndef STEPCOUNTER_H
#define STEPCOUNTER_H

#include <QObject>
#include <sensord-qt5/stepcountersensor_i.h>
#include <sensord-qt5/sensormanagerinterface.h>
#include <QtDBus/QtDBus>

#define SERVICE_NAME "com.kimmoli.stpcntrd"

class QDBusInterface;
class StepCounter : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", SERVICE_NAME)

public:
    explicit StepCounter(QObject *parent = 0);
    ~StepCounter();
    bool registerDBus();

public slots:
    QString getVersion();
    int getSteps();
    Q_NOREPLY void setAutoUpdate(const bool& value);
    Q_NOREPLY void quit();

signals:
    void steps(int);

private slots:
    void dataChanged(const Unsigned& data);

private:
    void update();

    StepCounterSensorChannelInterface* sensor;
    int m_currentSteps;
    bool m_isConnected;
    bool m_dbusRegistered;
};

#endif // STEPCOUNTER_H
