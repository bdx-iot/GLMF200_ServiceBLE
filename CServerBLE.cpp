#include "CServerBLE.h"

uint32_t float754tofloat11073(float value)
{
    uint8_t  exponent = 0xFE; //exponent is -2
    uint32_t mantissa = (uint32_t)(value * 100);

    return (((uint32_t)exponent) << 24) | mantissa;
}

CServerBLE::CServerBLE()
{
    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(mainLoop()));
}

CServerBLE::~CServerBLE()
{
    delete m_timer;
}

void CServerBLE::initServer()
{
    QLowEnergyAdvertisingData m_advertisingData;                                              //serveur
    m_advertisingData.setDiscoverability(QLowEnergyAdvertisingData::DiscoverabilityGeneral);  //Mode d'accessibilité
    m_advertisingData.setLocalName("OS #21 BLE");                                             //Nom du serveur
    m_advertisingData.setServices(QList<QBluetoothUuid>() <<
                                QBluetoothUuid::HeartRate <<
                                QBluetoothUuid::HealthThermometer <<
                                QBluetoothUuid::BloodPressure
                                );                         //Ajout des services prédéfinis

    //Création de la caractéristique
    QLowEnergyCharacteristicData charData;
    //spécification de la caractéristique
    charData.setUuid(QBluetoothUuid::HeartRateMeasurement);
    charData.setValue(QByteArray(2, 0));
    //spécifique à la documentation bluetooth
    charData.setProperties(QLowEnergyCharacteristic::Notify);
    //Ajout d'un descripteur
    const QLowEnergyDescriptorData clientConfig(QBluetoothUuid::ClientCharacteristicConfiguration, QByteArray(2, 0));
    charData.addDescriptor(clientConfig);
    //Couplage du service avec la caractéristique créée.
    QLowEnergyServiceData serviceData;
    serviceData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceData.setUuid(QBluetoothUuid::HeartRate);
    serviceData.addCharacteristic(charData);

    //Création de la charactéristique
    QLowEnergyCharacteristicData charTemperature;
    //spécification de la caractéristique voulue
    charTemperature.setUuid(QBluetoothUuid::TemperatureMeasurement);
    charTemperature.setValue(QByteArray(2, 0));
    //spécifique à la documentation bluetooth
    charTemperature.setProperties(QLowEnergyCharacteristic::Indicate);
    //Ajout d'un descripteur
    const QLowEnergyDescriptorData clientConfigTemp(QBluetoothUuid::ClientCharacteristicConfiguration,  QByteArray(2, 0));
    charTemperature.addDescriptor(clientConfigTemp);
    //Couplage du service avec la caractéristique créée.
    QLowEnergyServiceData serviceTemperatureData;
    serviceTemperatureData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    serviceTemperatureData.setUuid(QBluetoothUuid::HealthThermometer);
    serviceTemperatureData.addCharacteristic(charTemperature);

    //Création de la charactéristique
    QLowEnergyCharacteristicData charPressure;
    //spécification de la caractéristique voulue
    charPressure.setUuid(QBluetoothUuid::BloodPressureMeasurement);
    charPressure.setValue(QByteArray(2, 0));
    //spécifique à la documentation bluetooth
    charPressure.setProperties(QLowEnergyCharacteristic::Indicate);
    //Ajout d'un descripteur
    const QLowEnergyDescriptorData clientConfigPressure(QBluetoothUuid::ClientCharacteristicConfiguration,  QByteArray(2, 0));
    charPressure.addDescriptor(clientConfigPressure);
    //Couplage du service avec la caractéristique créée.
    QLowEnergyServiceData servicePressureData;
    servicePressureData.setType(QLowEnergyServiceData::ServiceTypePrimary);
    servicePressureData.setUuid(QBluetoothUuid::BloodPressure);
    servicePressureData.addCharacteristic(charPressure);

    //advertise
    m_bleController = QLowEnergyController::createPeripheral();
    m_serviceHeartBeat = m_bleController->addService(serviceData);
    m_serviceTemperature = m_bleController->addService(serviceTemperatureData);
    m_servicePressure = m_bleController->addService(servicePressureData);
    m_bleController->startAdvertising(QLowEnergyAdvertisingParameters(), m_advertisingData, m_advertisingData);

    m_timer->start(500);

    connect(m_bleController, SIGNAL(stateChanged(QLowEnergyController::ControllerState)), this, SLOT(controllerStateChanged(QLowEnergyController::ControllerState)));
}

void CServerBLE::mainLoop()
{
    quint32 heartbeat = CServiceRpmsgInterface::getHeartBeat();
    float   temperature = CServiceRpmsgInterface::getTemperature();
    float   pressure = CServiceRpmsgInterface::getPressure();

    updatePressure(pressure);
    updateTemperature(temperature);
    updateHeartBeat(heartbeat);
}

void CServerBLE::controllerStateChanged(QLowEnergyController::ControllerState state)
{
    if (state == QLowEnergyController::UnconnectedState)
        m_bleController->startAdvertising(QLowEnergyAdvertisingParameters(), m_advertisingData, m_advertisingData);
}

void CServerBLE::updatePressure(float pressure)
{
    quint32 dataToSend = float754tofloat11073(pressure);

    QByteArray value;
    value.append(char(1)); // Flags à 1 pour afficher les valeurs en kPa
    //Ajout des valeurs sur les 3 données (Systolic, Diastolic, MAP). Voir documentation
    value.append(reinterpret_cast<const char*>(&dataToSend), sizeof(dataToSend));
    value.append(reinterpret_cast<const char*>(&dataToSend), sizeof(dataToSend));
    value.append(reinterpret_cast<const char*>(&dataToSend), sizeof(dataToSend));

    QLowEnergyCharacteristic characteristic = m_servicePressure->characteristic(QBluetoothUuid::BloodPressureMeasurement);
    Q_ASSERT(characteristic.isValid());
    m_servicePressure->writeCharacteristic(characteristic, value);
}

void CServerBLE::updateTemperature(float temperature)
{
    quint32 dataToSend = float754tofloat11073(temperature);

    //Création de la donnée
    QByteArray value;
    value.append(char(0)); // Flags spécifiant le format de la valeur

    //Stockage du quint32 dans un QByteArray
    value.append(reinterpret_cast<const char*>(&dataToSend), sizeof(dataToSend)); // Valeur à envoyer

    // récupération de la caratéristique du service
    QLowEnergyCharacteristic characteristic = m_serviceTemperature->characteristic(QBluetoothUuid::TemperatureMeasurement);
    Q_ASSERT(characteristic.isValid());

    //mise à jour de la donnée
    m_serviceTemperature->writeCharacteristic(characteristic, value); // Potentially causes notification.
}

void CServerBLE::updateHeartBeat(quint32 heartBeat)
{
    quint8 valueToInt8 = heartBeat; // nécessaire pour le protocol

    QByteArray value;
    value.append(char(0));
    value.append(char(valueToInt8));

    QLowEnergyCharacteristic characteristic = m_serviceHeartBeat->characteristic(QBluetoothUuid::HeartRateMeasurement);
    Q_ASSERT(characteristic.isValid());

    m_serviceHeartBeat->writeCharacteristic(characteristic, value);
}
