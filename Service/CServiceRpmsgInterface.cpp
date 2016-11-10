#include "CServiceRpmsgInterface.h"

double CServiceRpmsgInterface::getTemperature()
{
    QDBusInterface iface(SERVICE_NAME, PROPERTY_PATH, "", QDBusConnection::systemBus());
    if (iface.isValid())
    {
        QDBusReply<double> reply = iface.call("getTemperature");

        if (reply.isValid())
        {
            return reply.value();
        }
    }
    return 0;
}

double CServiceRpmsgInterface::getPressure()
{
    QDBusInterface iface(SERVICE_NAME, PROPERTY_PATH, "", QDBusConnection::systemBus());
    if (iface.isValid())
    {
        QDBusReply<double> reply = iface.call("getPressure");

        if (reply.isValid())
        {
            return reply.value();
        }
    }
    return 0;
}

quint32 CServiceRpmsgInterface::getHeartBeat()
{
    QDBusInterface iface(SERVICE_NAME, PROPERTY_PATH, "", QDBusConnection::systemBus());
    if (iface.isValid())
    {
        QDBusReply<quint32> reply = iface.call("getHeartBeat");

        if (reply.isValid())
        {
            return reply.value();
        }
    }
    return 0;
}
