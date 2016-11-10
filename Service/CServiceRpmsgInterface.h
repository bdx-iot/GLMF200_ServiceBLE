#ifndef CSERVICERPMSGINTERFACE_H
#define CSERVICERPMSGINTERFACE_H

#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>

#define SERVICE_NAME        "amplitude.systemes.service.rpmsg"
#define PROPERTY_PATH       "/"

class CServiceRpmsgInterface
{
public:
    static double      getTemperature();
    static double      getPressure();
    static quint32     getHeartBeat();
};

#endif // CSERVICERPMSGINTERFACE_H
