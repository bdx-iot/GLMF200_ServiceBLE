#ifndef CSERVERBLE_H
#define CSERVERBLE_H

#include <QtBluetooth/qlowenergyadvertisingdata.h>
#include <QtBluetooth/qlowenergyadvertisingparameters.h>
#include <QtBluetooth/qlowenergycharacteristic.h>
#include <QtBluetooth/qlowenergycharacteristicdata.h>
#include <QtBluetooth/qlowenergydescriptordata.h>
#include <QtBluetooth/qlowenergycontroller.h>
#include <QtBluetooth/qlowenergyservice.h>
#include <QtBluetooth/qlowenergyservicedata.h>
#include <QtCore/qtimer.h>
#include <QtEndian>

#include "Service/CServiceRpmsgInterface.h"

class CServerBLE : public QObject
{
    Q_OBJECT
public:
    CServerBLE();
    ~CServerBLE();

public:
    void initServer();

protected slots:
    void mainLoop();
    void controllerStateChanged(QLowEnergyController::ControllerState state);

protected:
    void updatePressure(float pressure);
    void updateTemperature(float temperature);
    void updateHeartBeat(quint32 heartBeat);

private:
    QLowEnergyAdvertisingData   m_advertisingData;
    QLowEnergyController        *m_bleController;
    QLowEnergyService           *m_serviceHeartBeat;
    QLowEnergyService           *m_serviceTemperature;
    QLowEnergyService           *m_servicePressure;
    QTimer                      *m_timer;
};

#endif // CSERVERBLE_H
