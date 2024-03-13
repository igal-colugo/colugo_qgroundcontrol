#ifndef COMMTACT_MANAGEMENT_H
#define COMMTACT_MANAGEMENT_H

#include <QObject>
#include <QVector>
#include <QWidget>
#include <atomic>

#include "CommtactLinkManager.h"
#include "CommtactLinkProtocol.h"
#include "QGCApplication.h"
#include "Settings/SettingsManager.h"
#include <QTimer>

class CommtactLinkManagement : public QGCTool
{
    Q_OBJECT

  public:
    CommtactLinkManagement(QGCApplication *app, QGCToolbox *toolbox);
    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);
    void updateTimeout();

    std::atomic<uint8_t> m_should_enable_rescue_btn{};

    // UI state management functions (called periodically, their result is meant to update Qml proprties dynamically)
    Q_INVOKABLE bool getShouldUiEnabledRescueElement();

    //---------------------- GDT Properties ---------------------------------
    Q_PROPERTY(QStringList operationalModeTypeStrings READ operationalModeTypeStrings CONSTANT)
    Q_PROPERTY(uint transmitterOperationalMode READ getTransmitterOperationalMode WRITE setTransmitterOperationalMode NOTIFY transmitterOperationalModeChanged)
    uint getTransmitterOperationalMode()
    {
        return (uint) _transmitterOperationalMode;
    }
    void setTransmitterOperationalMode(const uint &transmitterOperationalMode)
    {
        _transmitterOperationalMode = (uint8_t) transmitterOperationalMode;
        emit transmitterOperationalModeChanged();
    }

    Q_PROPERTY(QStringList gdtAntennaSelectTypeStrings READ gdtAntennaSelectTypeStrings CONSTANT)
    Q_PROPERTY(uint gdtAntennaSelect READ getGdtAntennaSelect WRITE setGdtAntennaSelect NOTIFY gdtAntennaSelectChanged)
    uint getGdtAntennaSelect()
    {
        return (uint) _gdtAntennaSelect;
    }
    void setGdtAntennaSelect(const uint &gdtAntennaSelect)
    {
        _gdtAntennaSelect = (uint8_t) gdtAntennaSelect;
        emit gdtAntennaSelectChanged();
    }

    Q_PROPERTY(QStringList gdtPedestalTrackModeTypeStrings READ gdtPedestalTrackModeTypeStrings CONSTANT)
    Q_PROPERTY(uint gdtPedestalTrackMode READ getGdtPedestalTrackMode WRITE setGdtPedestalTrackMode NOTIFY gdtPedestalTrackModeChanged)
    uint getGdtPedestalTrackMode()
    {
        return (uint) _gdtPedestalTrackMode;
    }
    void setGdtPedestalTrackMode(const uint &gdtPedestalTrackMode)
    {
        _gdtPedestalTrackMode = (uint8_t) gdtPedestalTrackMode;
        emit gdtPedestalTrackModeChanged();
    }

    Q_PROPERTY(QStringList gdtTddOperationalModeTypeStrings READ gdtTddOperationalModeTypeStrings CONSTANT)
    Q_PROPERTY(uint gdtTddOperationalMode READ getGdtTddOperationalMode WRITE setGdtTddOperationalMode NOTIFY gdtTddOperationalModeChanged)
    uint getGdtTddOperationalMode()
    {
        return (uint) _gdtTddOperationalMode;
    }
    void setGdtTddOperationalMode(const uint &gdtTddOperationalMode)
    {
        _gdtTddOperationalMode = (uint8_t) gdtTddOperationalMode;
        emit gdtTddOperationalModeChanged();
    }

    Q_PROPERTY(QStringList gdtFrequencyModeTypeStrings READ gdtFrequencyModeTypeStrings CONSTANT)
    Q_PROPERTY(uint gdtFrequencyMode READ getGdtFrequencyMode WRITE setGdtFrequencyMode NOTIFY gdtFrequencyModeChanged)
    uint getGdtFrequencyMode()
    {
        return (uint) _gdtFrequencyMode;
    }
    void setGdtFrequencyMode(const uint &gdtFrequencyMode)
    {
        _gdtFrequencyMode = (uint8_t) gdtFrequencyMode;
        emit gdtFrequencyModeChanged();
    }

    Q_PROPERTY(QStringList gdtUnitModeTypeStrings READ gdtUnitModeTypeStrings CONSTANT)
    Q_PROPERTY(uint gdtUnitMode READ getGdtUnitMode WRITE setGdtUnitMode NOTIFY gdtUnitModeChanged)
    uint getGdtUnitMode()
    {
        return (uint) _gdtUnitMode;
    }
    void setGdtUnitMode(const uint &gdtUnitMode)
    {
        _gdtUnitMode = (uint8_t) gdtUnitMode;
        emit gdtUnitModeChanged();
    }

    Q_PROPERTY(QStringList gdtSymbolRateTypeStrings READ gdtSymbolRateTypeStrings CONSTANT)
    Q_PROPERTY(uint gdtSymbolRate READ getGdtSymbolRate WRITE setGdtSymbolRate NOTIFY gdtSymbolRateChanged)
    uint getGdtSymbolRate()
    {
        return (uint) _gdtSymbolRate;
    }
    void setGdtSymbolRate(const uint &gdtSymbolRate)
    {
        _gdtSymbolRate = (uint8_t) gdtSymbolRate;
        emit gdtSymbolRateChanged();
    }

    Q_PROPERTY(uint gdtAesEncryption READ getGdtAesEncryption WRITE setGdtAesEncryption NOTIFY gdtAesEncryptionChanged)
    uint getGdtAesEncryption()
    {
        return (uint) _gdtAesEncryption;
    }
    void setGdtAesEncryption(const uint &gdtAesEncryption)
    {
        _gdtAesEncryption = (uint8_t) gdtAesEncryption;
        emit gdtAesEncryptionChanged();
    }

    Q_PROPERTY(int gdtTDDSync READ getGdtTDDSync WRITE setGdtTDDSync NOTIFY gdtTDDSyncChanged)
    uint getGdtTDDSync()
    {
        return (int) _gdtTDDSync;
    }
    void setGdtTDDSync(const int &gdtTDDSync)
    {
        _gdtTDDSync = (int8_t) gdtTDDSync;
        emit gdtTDDSyncChanged();
    }

    Q_PROPERTY(int gdtLinkRSSI READ getGdtLinkRSSI WRITE setGdtLinkRSSI NOTIFY gdtLinkRSSIChanged)
    uint getGdtLinkRSSI()
    {
        return (int) _gdtLinkRSSI;
    }
    void setGdtLinkRSSI(const int &gdtLinkRSSI)
    {
        _gdtLinkRSSI = (int8_t) gdtLinkRSSI;
        emit gdtLinkRSSIChanged();
    }

    Q_PROPERTY(uint gdtLinkTransferedPackets READ getGdtLinkTransferedPackets WRITE setGdtLinkTransferedPackets NOTIFY gdtLinkTransferedPacketsChanged)
    uint getGdtLinkTransferedPackets()
    {
        return (uint) _gdtLinkTransferedPackets;
    }
    void setGdtLinkTransferedPackets(const uint16_t &gdtLinkTransferedPackets)
    {
        _gdtLinkTransferedPackets = (uint16_t) gdtLinkTransferedPackets;
        emit gdtLinkTransferedPacketsChanged();
    }

    Q_PROPERTY(uint gdtLinkErrorPackets READ getGdtLinkErrorPackets WRITE setGdtLinkErrorPackets NOTIFY gdtLinkErrorPacketsChanged)
    uint getGdtLinkErrorPackets()
    {
        return (uint) _gdtLinkErrorPackets;
    }
    void setGdtLinkErrorPackets(const uint16_t &gdtLinkErrorPackets)
    {
        _gdtLinkErrorPackets = (uint16_t) gdtLinkErrorPackets;
        emit gdtLinkErrorPacketsChanged();
    }

    Q_PROPERTY(uint gdtLinkCRCErrorPackets READ getGdtLinkCRCErrorPackets WRITE setGdtLinkCRCErrorPackets NOTIFY gdtLinkCRCErrorPacketsChanged)
    uint getGdtLinkCRCErrorPackets()
    {
        return (uint) _gdtLinkCRCErrorPackets;
    }
    void setGdtLinkCRCErrorPackets(const uint16_t &gdtLinkCRCErrorPackets)
    {
        _gdtLinkCRCErrorPackets = (uint16_t) gdtLinkCRCErrorPackets;
        emit gdtLinkCRCErrorPacketsChanged();
    }

    Q_PROPERTY(int gdtOperationFrequency READ getGdtOperationFrequency WRITE setGdtOperationFrequency NOTIFY gdtOperationFrequencyChanged)
    uint getGdtOperationFrequency()
    {
        return (int) _gdtOperationFrequency;
    }
    void setGdtOperationFrequency(const int &gdtOperationFrequency)
    {
        _gdtOperationFrequency = (uint16_t) gdtOperationFrequency;
        emit gdtOperationFrequencyChanged();
    }

    Q_PROPERTY(int gdtCBITPAPowerOutput READ getGdtCBITPAPowerOutput WRITE setGdtCBITPAPowerOutput NOTIFY gdtCBITPAPowerOutputChanged)
    int getGdtCBITPAPowerOutput()
    {
        return (int) _gdtCBITPAPowerOutput;
    }
    void setGdtCBITPAPowerOutput(const int &gdtCBITPAPowerOutput)
    {
        _gdtCBITPAPowerOutput = (int16_t) gdtCBITPAPowerOutput;
        emit gdtCBITPAPowerOutputChanged();
    }

    Q_PROPERTY(int gdtCBITPAReturnPower READ getGdtCBITPAReturnPower WRITE setGdtCBITPAReturnPower NOTIFY gdtCBITPAReturnPowerChanged)
    int getGdtCBITPAReturnPower()
    {
        return (int) _gdtCBITPAReturnPower;
    }
    void setGdtCBITPAReturnPower(const int &gdtCBITPAReturnPower)
    {
        _gdtCBITPAReturnPower = (int16_t) gdtCBITPAReturnPower;
        emit gdtCBITPAReturnPowerChanged();
    }

    //---------------------- ADT Properties ---------------------------------
    Q_PROPERTY(QStringList adtOperationalModeTypeStrings READ adtOperationalModeTypeStrings CONSTANT)
    Q_PROPERTY(uint adtTransmitterOperationalMode READ getAdtTransmitterOperationalMode WRITE setAdtTransmitterOperationalMode NOTIFY adtTransmitterOperationalModeChanged)
    uint getAdtTransmitterOperationalMode()
    {
        return (uint) _adtTransmitterOperationalMode;
    }
    void setAdtTransmitterOperationalMode(const uint &adtTransmitterOperationalMode)
    {
        _adtTransmitterOperationalMode = (uint8_t) adtTransmitterOperationalMode;
        emit adtTransmitterOperationalModeChanged();
    }

    Q_PROPERTY(QStringList adtAntennaSelectTypeStrings READ adtAntennaSelectTypeStrings CONSTANT)
    Q_PROPERTY(uint adtAntennaSelect READ getAdtAntennaSelect WRITE setAdtAntennaSelect NOTIFY adtAntennaSelectChanged)
    uint getAdtAntennaSelect()
    {
        return (uint) _adtAntennaSelect;
    }
    void setAdtAntennaSelect(const uint &adtAntennaSelect)
    {
        _adtAntennaSelect = (uint8_t) adtAntennaSelect;
        emit adtAntennaSelectChanged();
    }

    Q_PROPERTY(QStringList adtTddOperationalModeTypeStrings READ adtTddOperationalModeTypeStrings CONSTANT)
    Q_PROPERTY(uint adtTddOperationalMode READ getAdtTddOperationalMode WRITE setAdtTddOperationalMode NOTIFY adtTddOperationalModeChanged)
    uint getAdtTddOperationalMode()
    {
        return (uint) _adtTddOperationalMode;
    }
    void setAdtTddOperationalMode(const uint &adtTddOperationalMode)
    {
        _adtTddOperationalMode = (uint8_t) adtTddOperationalMode;
        emit adtTddOperationalModeChanged();
    }

    Q_PROPERTY(QStringList adtFrequencyModeTypeStrings READ adtFrequencyModeTypeStrings CONSTANT)
    Q_PROPERTY(uint adtFrequencyMode READ getAdtFrequencyMode WRITE setAdtFrequencyMode NOTIFY adtFrequencyModeChanged)
    uint getAdtFrequencyMode()
    {
        return (uint) _adtFrequencyMode;
    }
    void setAdtFrequencyMode(const uint &adtFrequencyMode)
    {
        _adtFrequencyMode = (uint8_t) adtFrequencyMode;
        emit adtFrequencyModeChanged();
    }

    Q_PROPERTY(uint adtVideoTransmitEnable READ getAdtVideoTransmitEnable WRITE setAdtVideoTransmitEnable NOTIFY adtVideoTransmitEnableChanged)
    uint getAdtVideoTransmitEnable()
    {
        return (uint) _adtVideoTransmitEnable;
    }
    void setAdtVideoTransmitEnable(const uint &adtVideoTransmitEnable)
    {
        _adtVideoTransmitEnable = (uint8_t) adtVideoTransmitEnable;
        emit adtVideoTransmitEnableChanged();
    }

    Q_PROPERTY(QStringList adtUnitModeTypeStrings READ adtUnitModeTypeStrings CONSTANT)
    Q_PROPERTY(uint adtUnitMode READ getAdtUnitMode WRITE setAdtUnitMode NOTIFY adtUnitModeChanged)
    uint getAdtUnitMode()
    {
        return (uint) _adtUnitMode;
    }
    void setAdtUnitMode(const uint &adtUnitMode)
    {
        _adtUnitMode = (uint8_t) adtUnitMode;
        emit adtUnitModeChanged();
    }

    Q_PROPERTY(QStringList adtSymbolRateTypeStrings READ adtSymbolRateTypeStrings CONSTANT)
    Q_PROPERTY(uint adtSymbolRate READ getAdtSymbolRate WRITE setAdtSymbolRate NOTIFY adtSymbolRateChanged)
    uint getAdtSymbolRate()
    {
        return (uint) _adtSymbolRate;
    }
    void setAdtSymbolRate(const uint &adtSymbolRate)
    {
        _adtSymbolRate = (uint8_t) adtSymbolRate;
        emit adtSymbolRateChanged();
    }

    Q_PROPERTY(uint adtAesEncryption READ getAdtAesEncryption WRITE setAdtAesEncryption NOTIFY adtAesEncryptionChanged)
    uint getAdtAesEncryption()
    {
        return (uint) _adtAesEncryption;
    }
    void setAdtAesEncryption(const uint &adtAesEncryption)
    {
        _adtAesEncryption = (uint8_t) adtAesEncryption;
        emit adtAesEncryptionChanged();
    }

    Q_PROPERTY(uint adtTelemetryMetadataSeparation READ getAdtTelemetryMetadataSeparation WRITE setAdtTelemetryMetadataSeparation NOTIFY adtTelemetryMetadataSeparationChanged)
    uint getAdtTelemetryMetadataSeparation()
    {
        return (uint) _adtTelemetryMetadataSeparation;
    }
    void setAdtTelemetryMetadataSeparation(const uint &adtTelemetryMetadataSeparation)
    {
        _adtTelemetryMetadataSeparation = (uint8_t) adtTelemetryMetadataSeparation;
        emit adtTelemetryMetadataSeparationChanged();
    }

    Q_PROPERTY(int adtTDDSync READ getAdtTDDSync WRITE setAdtTDDSync NOTIFY adtTDDSyncChanged)
    uint getAdtTDDSync()
    {
        return (int) _adtTDDSync;
    }
    void setAdtTDDSync(const int &adtTDDSync)
    {
        _adtTDDSync = (int8_t) adtTDDSync;
        emit adtTDDSyncChanged();
    }

    Q_PROPERTY(int adtLinkRSSI READ getAdtLinkRSSI WRITE setAdtLinkRSSI NOTIFY adtLinkRSSIChanged)
    uint getAdtLinkRSSI()
    {
        return (int) _adtLinkRSSI;
    }
    void setAdtLinkRSSI(const int &adtLinkRSSI)
    {
        _adtLinkRSSI = (int8_t) adtLinkRSSI;
        emit adtLinkRSSIChanged();
    }

    Q_PROPERTY(uint adtLinkTransferedPackets READ getAdtLinkTransferedPackets WRITE setAdtLinkTransferedPackets NOTIFY adtLinkTransferedPacketsChanged)
    uint getAdtLinkTransferedPackets()
    {
        return (uint) _adtLinkTransferedPackets;
    }
    void setAdtLinkTransferedPackets(const uint16_t &adtLinkTransferedPackets)
    {
        _adtLinkTransferedPackets = (uint16_t) adtLinkTransferedPackets;
        emit adtLinkTransferedPacketsChanged();
    }

    Q_PROPERTY(uint adtLinkErrorPackets READ getAdtLinkErrorPackets WRITE setAdtLinkErrorPackets NOTIFY adtLinkErrorPacketsChanged)
    uint getAdtLinkErrorPackets()
    {
        return (uint) _adtLinkErrorPackets;
    }
    void setAdtLinkErrorPackets(const uint16_t &adtLinkErrorPackets)
    {
        _adtLinkErrorPackets = (uint16_t) adtLinkErrorPackets;
        emit adtLinkErrorPacketsChanged();
    }

    Q_PROPERTY(uint adtLinkCRCErrorPackets READ getAdtLinkCRCErrorPackets WRITE setAdtLinkCRCErrorPackets NOTIFY adtLinkCRCErrorPacketsChanged)
    uint getAdtLinkCRCErrorPackets()
    {
        return (uint) _adtLinkCRCErrorPackets;
    }
    void setAdtLinkCRCErrorPackets(const uint16_t &adtLinkCRCErrorPackets)
    {
        _adtLinkCRCErrorPackets = (uint16_t) adtLinkCRCErrorPackets;
        emit adtLinkCRCErrorPacketsChanged();
    }

    Q_PROPERTY(int adtOperationFrequency READ getAdtOperationFrequency WRITE setAdtOperationFrequency NOTIFY adtOperationFrequencyChanged)
    uint getAdtOperationFrequency()
    {
        return (int) _adtOperationFrequency;
    }
    void setAdtOperationFrequency(const int &adtOperationFrequency)
    {
        _adtOperationFrequency = (uint16_t) adtOperationFrequency;
        emit adtOperationFrequencyChanged();
    }

    Q_PROPERTY(int adtCBITPAPowerOutput READ getAdtCBITPAPowerOutput WRITE setAdtCBITPAPowerOutput NOTIFY adtCBITPAPowerOutputChanged)
    int getAdtCBITPAPowerOutput()
    {
        return (int) _adtCBITPAPowerOutput;
    }
    void setAdtCBITPAPowerOutput(const int &adtCBITPAPowerOutput)
    {
        _adtCBITPAPowerOutput = (int16_t) adtCBITPAPowerOutput;
        emit adtCBITPAPowerOutputChanged();
    }

    Q_PROPERTY(int adtCBITPAReturnPower READ getAdtCBITPAReturnPower WRITE setAdtCBITPAReturnPower NOTIFY adtCBITPAReturnPowerChanged)
    int getAdtCBITPAReturnPower()
    {
        return (int) _adtCBITPAReturnPower;
    }
    void setAdtCBITPAReturnPower(const int &adtCBITPAReturnPower)
    {
        _adtCBITPAReturnPower = (int16_t) adtCBITPAReturnPower;
        emit adtCBITPAReturnPowerChanged();
    }

    Q_PROPERTY(QStringList adtVideoRateTypeStrings READ adtVideoRateTypeStrings CONSTANT)
    Q_PROPERTY(int adtVideoRate READ getAdtVideoRate WRITE setAdtVideoRate NOTIFY adtVideoRateChanged)
    int getAdtVideoRate()
    {
        return (int) _adtVideoRate;
    }
    void setAdtVideoRate(const int &adtVideoRate)
    {
        _adtVideoRate = (int16_t) adtVideoRate;
        emit adtVideoRateChanged();
    }
    Q_PROPERTY(QStringList adtVideoSourceTypeStrings READ adtVideoSourceTypeStrings CONSTANT)
    Q_PROPERTY(int adtVideoSource READ getAdtVideoSource WRITE setAdtVideoSource NOTIFY adtVideoSourceChanged)
    int getAdtVideoSource()
    {
        return (int) _adtVideoSource;
    }
    void setAdtVideoSource(const int &adtVideoSource)
    {
        _adtVideoSource = (int16_t) adtVideoSource;
        emit adtVideoSourceChanged();
    }
    //---------------------------------------------------------------

    //------------------- Common GDT Properties -------------------------
    Q_PROPERTY(uint commonICDIPAddressInt READ getCommonICDIPAddressInt WRITE setCommonICDIPAddressInt NOTIFY commonICDIPAddressIntChanged)
    uint getCommonICDIPAddressInt()
    {
        return (uint) _commonICDIPAddressInt;
    }
    void setCommonICDIPAddressInt(const uint &commonICDIPAddressInt)
    {
        _commonICDIPAddressInt = (uint) commonICDIPAddressInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDIPAddressInt;

        QString commonICDIPAddress = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                     QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDIPAddress(commonICDIPAddress);

        emit commonICDIPAddressIntChanged();
    }

    Q_PROPERTY(QString commonICDIPAddress READ getCommonICDIPAddress WRITE setCommonICDIPAddress NOTIFY commonICDIPAddressChanged)
    QString getCommonICDIPAddress()
    {
        return _commonICDIPAddress;
    }
    void setCommonICDIPAddress(const QString &commonICDIPAddress)
    {
        _commonICDIPAddress = commonICDIPAddress;

        emit commonICDIPAddressChanged();
    }

    Q_PROPERTY(uint commonICDPort READ getCommonICDPort WRITE setCommonICDPort NOTIFY commonICDPortChanged)
    uint getCommonICDPort()
    {
        return _commonICDPort;
    }
    void setCommonICDPort(const uint &commonICDPort)
    {
        _commonICDPort = commonICDPort;

        emit commonICDPortChanged();
    }

    Q_PROPERTY(uint commonICDSubnetMaskInt READ getCommonICDSubnetMaskInt WRITE setCommonICDSubnetMaskInt NOTIFY commonICDSubnetMaskIntChanged)
    uint getCommonICDSubnetMaskInt()
    {
        return (uint) _commonICDSubnetMaskInt;
    }
    void setCommonICDSubnetMaskInt(const uint &commonICDSubnetMaskInt)
    {
        _commonICDSubnetMaskInt = (uint) commonICDSubnetMaskInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDSubnetMaskInt;

        QString commonICDSubnetMask = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                      QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDSubnetMask(commonICDSubnetMask);

        emit commonICDSubnetMaskIntChanged();
    }

    Q_PROPERTY(QString commonICDSubnetMask READ getCommonICDSubnetMask WRITE setCommonICDSubnetMask NOTIFY commonICDSubnetMaskChanged)
    QString getCommonICDSubnetMask()
    {
        return _commonICDSubnetMask;
    }
    void setCommonICDSubnetMask(const QString &commonICDSubnetMask)
    {
        _commonICDSubnetMask = commonICDSubnetMask;

        emit commonICDSubnetMaskChanged();
    }

    Q_PROPERTY(uint commonICDDefaultGatewayInt READ getCommonICDDefaultGatewayInt WRITE setCommonICDDefaultGatewayInt NOTIFY commonICDDefaultGatewayIntChanged)
    uint getCommonICDDefaultGatewayInt()
    {
        return (uint) _commonICDDefaultGatewayInt;
    }
    void setCommonICDDefaultGatewayInt(const uint &commonICDDefaultGatewayInt)
    {
        _commonICDDefaultGatewayInt = (uint) commonICDDefaultGatewayInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDDefaultGatewayInt;

        QString commonICDDefaultGateway = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                          QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDDefaultGateway(commonICDDefaultGateway);

        emit commonICDDefaultGatewayIntChanged();
    }

    Q_PROPERTY(QString commonICDDefaultGateway READ getCommonICDDefaultGateway WRITE setCommonICDDefaultGateway NOTIFY commonICDDefaultGatewayChanged)
    QString getCommonICDDefaultGateway()
    {
        return _commonICDDefaultGateway;
    }
    void setCommonICDDefaultGateway(const QString &commonICDDefaultGateway)
    {
        _commonICDDefaultGateway = commonICDDefaultGateway;

        emit commonICDDefaultGatewayChanged();
    }

    Q_PROPERTY(uint commonICDHostIPAddressInt READ getCommonICDHostIPAddressInt WRITE setCommonICDHostIPAddressInt NOTIFY commonICDHostIPAddressIntChanged)
    uint getCommonICDHostIPAddressInt()
    {
        return (uint) _commonICDHostIPAddressInt;
    }
    void setCommonICDHostIPAddressInt(const uint &commonICDHostIPAddressInt)
    {
        _commonICDHostIPAddressInt = (uint) commonICDHostIPAddressInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDHostIPAddressInt;

        QString commonICDHostIPAddress = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                         QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDHostIPAddress(commonICDHostIPAddress);

        emit commonICDHostIPAddressIntChanged();
    }

    Q_PROPERTY(QString commonICDHostIPAddress READ getCommonICDHostIPAddress WRITE setCommonICDHostIPAddress NOTIFY commonICDHostIPAddressChanged)
    QString getCommonICDHostIPAddress()
    {
        return _commonICDHostIPAddress;
    }
    void setCommonICDHostIPAddress(const QString &commonICDHostIPAddress)
    {
        _commonICDHostIPAddress = commonICDHostIPAddress;

        emit commonICDHostIPAddressChanged();
    }

    Q_PROPERTY(uint commonICDHostPort READ getCommonICDHostPort WRITE setCommonICDHostPort NOTIFY commonICDHostPortChanged)
    uint getCommonICDHostPort()
    {
        return _commonICDHostPort;
    }
    void setCommonICDHostPort(const uint &commonICDHostPort)
    {
        _commonICDHostPort = commonICDHostPort;

        emit commonICDHostPortChanged();
    }

    Q_PROPERTY(uint commonICDDiscoveryPort READ getCommonICDDiscoveryPort WRITE setCommonICDDiscoveryPort NOTIFY commonICDDiscoveryPortChanged)
    uint getCommonICDDiscoveryPort()
    {
        return _commonICDDiscoveryPort;
    }
    void setCommonICDDiscoveryPort(const uint &commonICDDiscoveryPort)
    {
        _commonICDDiscoveryPort = commonICDDiscoveryPort;

        emit commonICDDiscoveryPortChanged();
    }

    //---------------------------------------------------------------

    //------------------- Common ADT Properties -------------------------
    Q_PROPERTY(uint commonICDADTIPAddressInt READ getCommonICDADTIPAddressInt WRITE setCommonICDADTIPAddressInt NOTIFY commonICDADTIPAddressIntChanged)
    uint getCommonICDADTIPAddressInt()
    {
        return (uint) _commonICDADTIPAddressInt;
    }
    void setCommonICDADTIPAddressInt(const uint &commonICDADTIPAddressInt)
    {
        _commonICDADTIPAddressInt = (uint) commonICDADTIPAddressInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDADTIPAddressInt;

        QString commonICDADTIPAddress = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                        QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDADTIPAddress(commonICDADTIPAddress);

        emit commonICDADTIPAddressIntChanged();
    }

    Q_PROPERTY(QString commonICDADTIPAddress READ getCommonICDADTIPAddress WRITE setCommonICDADTIPAddress NOTIFY commonICDADTIPAddressChanged)
    QString getCommonICDADTIPAddress()
    {
        return _commonICDADTIPAddress;
    }
    void setCommonICDADTIPAddress(const QString &commonICDADTIPAddress)
    {
        _commonICDADTIPAddress = commonICDADTIPAddress;

        emit commonICDADTIPAddressChanged();
    }

    Q_PROPERTY(uint commonICDADTPort READ getCommonICDADTPort WRITE setCommonICDADTPort NOTIFY commonICDADTPortChanged)
    uint getCommonICDADTPort()
    {
        return _commonICDADTPort;
    }
    void setCommonICDADTPort(const uint &commonICDADTPort)
    {
        _commonICDADTPort = commonICDADTPort;

        emit commonICDADTPortChanged();
    }

    Q_PROPERTY(uint commonICDADTSubnetMaskInt READ getCommonICDADTSubnetMaskInt WRITE setCommonICDADTSubnetMaskInt NOTIFY commonICDADTSubnetMaskIntChanged)
    uint getCommonICDADTSubnetMaskInt()
    {
        return (uint) _commonICDADTSubnetMaskInt;
    }
    void setCommonICDADTSubnetMaskInt(const uint &commonICDADTSubnetMaskInt)
    {
        _commonICDADTSubnetMaskInt = (uint) commonICDADTSubnetMaskInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDADTSubnetMaskInt;

        QString commonICDADTSubnetMask = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                         QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDADTSubnetMask(commonICDADTSubnetMask);

        emit commonICDADTSubnetMaskIntChanged();
    }

    Q_PROPERTY(QString commonICDADTSubnetMask READ getCommonICDADTSubnetMask WRITE setCommonICDADTSubnetMask NOTIFY commonICDADTSubnetMaskChanged)
    QString getCommonICDADTSubnetMask()
    {
        return _commonICDADTSubnetMask;
    }
    void setCommonICDADTSubnetMask(const QString &commonICDADTSubnetMask)
    {
        _commonICDADTSubnetMask = commonICDADTSubnetMask;

        emit commonICDADTSubnetMaskChanged();
    }

    Q_PROPERTY(uint commonICDADTDefaultGatewayInt READ getCommonICDADTDefaultGatewayInt WRITE setCommonICDADTDefaultGatewayInt NOTIFY commonICDADTDefaultGatewayIntChanged)
    uint getCommonICDADTDefaultGatewayInt()
    {
        return (uint) _commonICDADTDefaultGatewayInt;
    }
    void setCommonICDADTDefaultGatewayInt(const uint &commonICDADTDefaultGatewayInt)
    {
        _commonICDADTDefaultGatewayInt = (uint) commonICDADTDefaultGatewayInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDADTDefaultGatewayInt;

        QString commonICDADTDefaultGateway = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                             QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDADTDefaultGateway(commonICDADTDefaultGateway);

        emit commonICDADTDefaultGatewayIntChanged();
    }

    Q_PROPERTY(QString commonICDADTDefaultGateway READ getCommonICDADTDefaultGateway WRITE setCommonICDADTDefaultGateway NOTIFY commonICDADTDefaultGatewayChanged)
    QString getCommonICDADTDefaultGateway()
    {
        return _commonICDADTDefaultGateway;
    }
    void setCommonICDADTDefaultGateway(const QString &commonICDADTDefaultGateway)
    {
        _commonICDADTDefaultGateway = commonICDADTDefaultGateway;

        emit commonICDADTDefaultGatewayChanged();
    }

    Q_PROPERTY(uint commonICDADTHostIPAddressInt READ getCommonICDADTHostIPAddressInt WRITE setCommonICDADTHostIPAddressInt NOTIFY commonICDADTHostIPAddressIntChanged)
    uint getCommonICDADTHostIPAddressInt()
    {
        return (uint) _commonICDADTHostIPAddressInt;
    }
    void setCommonICDADTHostIPAddressInt(const uint &commonICDADTHostIPAddressInt)
    {
        _commonICDADTHostIPAddressInt = (uint) commonICDADTHostIPAddressInt;

        unsigned char *pointer_to_ip_address = (unsigned char *) &_commonICDADTHostIPAddressInt;

        QString commonICDADTHostIPAddress = QString::number((uint) * (pointer_to_ip_address + 3)) + "." + QString::number((uint) * (pointer_to_ip_address + 2)) + "." +
                                            QString::number((uint) * (pointer_to_ip_address + 1)) + "." + QString::number((uint) * (pointer_to_ip_address + 0));

        setCommonICDADTHostIPAddress(commonICDADTHostIPAddress);

        emit commonICDADTHostIPAddressIntChanged();
    }

    Q_PROPERTY(QString commonICDADTHostIPAddress READ getCommonICDADTHostIPAddress WRITE setCommonICDADTHostIPAddress NOTIFY commonICDADTHostIPAddressChanged)
    QString getCommonICDADTHostIPAddress()
    {
        return _commonICDADTHostIPAddress;
    }
    void setCommonICDADTHostIPAddress(const QString &commonICDADTHostIPAddress)
    {
        _commonICDADTHostIPAddress = commonICDADTHostIPAddress;

        emit commonICDADTHostIPAddressChanged();
    }

    Q_PROPERTY(uint commonICDADTHostPort READ getCommonICDADTHostPort WRITE setCommonICDADTHostPort NOTIFY commonICDADTHostPortChanged)
    uint getCommonICDADTHostPort()
    {
        return _commonICDADTHostPort;
    }
    void setCommonICDADTHostPort(const uint &commonICDADTHostPort)
    {
        _commonICDADTHostPort = commonICDADTHostPort;

        emit commonICDADTHostPortChanged();
    }

    Q_PROPERTY(uint commonICDADTDiscoveryPort READ getCommonICDADTDiscoveryPort WRITE setCommonICDADTDiscoveryPort NOTIFY commonICDADTDiscoveryPortChanged)
    uint getCommonICDADTDiscoveryPort()
    {
        return _commonICDADTDiscoveryPort;
    }
    void setCommonICDADTDiscoveryPort(const uint &commonICDADTDiscoveryPort)
    {
        _commonICDADTDiscoveryPort = commonICDADTDiscoveryPort;

        emit commonICDADTDiscoveryPortChanged();
    }

    //---------------------------------------------------------------

    //------------------- GDT commands ------------------------------
    QStringList operationalModeTypeStrings(void) const;
    QStringList gdtAntennaSelectTypeStrings(void) const;
    QStringList gdtPedestalTrackModeTypeStrings(void) const;
    QStringList gdtTddOperationalModeTypeStrings(void) const;
    QStringList gdtFrequencyModeTypeStrings(void) const;
    QStringList gdtUnitModeTypeStrings(void) const;
    QStringList gdtSymbolRateTypeStrings(void) const;
    //-------------------------------------------------------------------

    //------------------- ADT commands ----------------------------------
    QStringList adtOperationalModeTypeStrings(void) const;
    QStringList adtAntennaSelectTypeStrings(void) const;
    QStringList adtTddOperationalModeTypeStrings(void) const;
    QStringList adtFrequencyModeTypeStrings(void) const;
    QStringList adtUnitModeTypeStrings(void) const;
    QStringList adtSymbolRateTypeStrings(void) const;
    QStringList adtVideoRateTypeStrings(void) const;
    QStringList adtVideoSourceTypeStrings(void) const;
    //-------------------------------------------------------------------

    //------------------- GDT commands ----------------------------------
    Q_INVOKABLE void setGDTOperationalModeCommand(uint transmitter_operational_mode);
    Q_INVOKABLE void setGDTAntennaSelectCommand(uint gdt_antenna_select);
    Q_INVOKABLE void setGDTPedestalTrackModeCommand(uint gdt_pedestal_track_mode);
    Q_INVOKABLE void setGDTTddOperationalModeCommand(uint gdt_tdd_operational_mode);
    Q_INVOKABLE void setGDTFrequencyModeCommand(uint gdt_frequency_mode);
    Q_INVOKABLE void setGDTUnitModeCommand(uint gdt_unit_mode);
    Q_INVOKABLE void setGDTSymbolRateCommand(uint gdt_symbol_rate);
    Q_INVOKABLE void setGDTAesEncryptionCommand(uint gdt_aes_encryption);
    Q_INVOKABLE void getGDTRequiredMessageCommand(uint gdt_required_message);
    Q_INVOKABLE void setGDTOperationalFrequencyCommand(uint gdt_operational_frequency);
    //-------------------------------------------------------------------

    //------------------- ADT commands ----------------------------------
    Q_INVOKABLE void setADTOperationalModeCommand(uint transmitter_operational_mode);
    Q_INVOKABLE void setADTAntennaSelectCommand(uint adt_antenna_select);
    Q_INVOKABLE void setADTTddOperationalModeCommand(uint adt_tdd_operational_mode);
    Q_INVOKABLE void setADTFrequencyModeCommand(uint adt_frequency_mode);
    Q_INVOKABLE void setADTVideoTransmitEnableCommand(uint adt_video_transmit_enable);
    Q_INVOKABLE void setADTUnitModeCommand(uint adt_unit_mode);
    Q_INVOKABLE void setADTSymbolRateCommand(uint adt_symbol_rate);
    Q_INVOKABLE void setADTAesEncryptionCommand(uint adt_aes_encryption);
    Q_INVOKABLE void setADTTelemetryMetadataSeparationCommand(uint adt_telemetry_metadata_separation);
    Q_INVOKABLE void setADTOperationalFrequencyCommand(uint adt_operational_frequency);
    Q_INVOKABLE void setADTVideoRateCommand(uint adt_video_rate);
    Q_INVOKABLE void setADTVideoSourceCommand(uint adt_video_source);
    //-------------------------------------------------------------------

    //----------------- COMMON commands ---------------------------------

    Q_INVOKABLE void setCommonEthernetICDIPAddressCommand(QString icd_ip_address_port_string);
    Q_INVOKABLE void setCommonEthernetICDSubnetMaskCommand(QString icd_subnet_mask_string);
    Q_INVOKABLE void setCommonEthernetICDDefaultGatewayCommand(QString icd_default_gateway_string);
    Q_INVOKABLE void setCommonEthernetICDHostIPCommand(QString icd_host_ip_port_string);
    Q_INVOKABLE void setCommonEthernetICDDiscoveryPortCommand(uint icd_discovery_port);

    //-------------------------------------------------------------------

  protected:
    CommtactLinkManager *_commtactLinkManager = nullptr;

  private:
    QTimer _updateTimer;

    //--------------- GDT fields --------------------
    uint8_t _transmitterOperationalMode = 0;
    uint8_t _gdtAntennaSelect = 0;
    uint8_t _gdtPedestalTrackMode = 0;
    uint8_t _gdtTddOperationalMode = 0;
    uint8_t _gdtFrequencyMode = 0;
    uint8_t _gdtUnitMode = 0;
    uint8_t _gdtSymbolRate = 0;
    uint8_t _gdtAesEncryption = 0;

    int8_t _gdtTDDSync = 0;
    int8_t _gdtLinkRSSI = 0;
    uint16_t _gdtLinkTransferedPackets = 0;
    uint16_t _gdtLinkErrorPackets = 0;
    uint16_t _gdtLinkCRCErrorPackets = 0;

    uint16_t _gdtOperationFrequency = 0;

    int16_t _gdtCBITPAPowerOutput = 0;
    int16_t _gdtCBITPAReturnPower = 0;

    CommtactLinkProtocol::commtact_gdt_operational_modes_report_t _operational_modes_report;
    CommtactLinkProtocol::commtact_gdt_status_report_t _gdt_status_report;
    CommtactLinkProtocol::commtact_gdt_constant_frequency_report_t _gdt_constant_frequency_report;
    CommtactLinkProtocol::commtact_gdt_cbit_report_t _gdt_cbit_report;
    CommtactLinkProtocol::commtact_gdt_mission_adt_status_report_t _gdt_mission_adt_status_report;
    CommtactLinkProtocol::commtact_basic_ethernet_settings_report_t _common_ethernet_settings_report;

    //--------------- ADT fields --------------------
    uint8_t _adtTransmitterOperationalMode = 0;
    uint8_t _adtAntennaSelect = 0;
    uint8_t _adtTddOperationalMode = 0;
    uint8_t _adtFrequencyMode = 0;
    uint8_t _adtVideoTransmitEnable = 0;
    uint8_t _adtUnitMode = 0;
    uint8_t _adtSymbolRate = 0;
    uint8_t _adtAesEncryption = 0;
    uint8_t _adtTelemetryMetadataSeparation = 0;

    int8_t _adtTDDSync = 0;
    int8_t _adtLinkRSSI = 0;
    uint16_t _adtLinkTransferedPackets = 0;
    uint16_t _adtLinkErrorPackets = 0;
    uint16_t _adtLinkCRCErrorPackets = 0;

    uint16_t _adtOperationFrequency = 0;

    int16_t _adtCBITPAPowerOutput = 0;
    int16_t _adtCBITPAReturnPower = 0;

    uint8_t _adtVideoRate = 0;
    uint8_t _adtVideoSource = 0;

    CommtactLinkProtocol::commtact_adt_operational_modes_report_t _adt_operational_modes_report;
    CommtactLinkProtocol::commtact_adt_constant_frequency_report_t _adt_constant_frequency_report;
    CommtactLinkProtocol::commtact_adt_status_report_t _adt_status_report;

    //-----------------------------------------------

    //--------------- Common GDT fields -------------
    uint _commonICDIPAddressInt = 0;
    QString _commonICDIPAddress = "127.0.0.1";
    uint _commonICDPort = 0;
    uint _commonICDSubnetMaskInt = 0;
    QString _commonICDSubnetMask = "255.255.255.0";
    uint _commonICDDefaultGatewayInt = 0;
    QString _commonICDDefaultGateway = "127.0.0.1";
    uint _commonICDHostIPAddressInt = 0;
    QString _commonICDHostIPAddress = "127.0.0.1";
    uint _commonICDHostPort = 0;
    uint _commonICDDiscoveryPort = 0;
    //-----------------------------------------------

    //--------------- Common ADT fields -------------
    uint _commonICDADTIPAddressInt = 0;
    QString _commonICDADTIPAddress = "127.0.0.1";
    uint _commonICDADTPort = 0;
    uint _commonICDADTSubnetMaskInt = 0;
    QString _commonICDADTSubnetMask = "255.255.255.0";
    uint _commonICDADTDefaultGatewayInt = 0;
    QString _commonICDADTDefaultGateway = "127.0.0.1";
    uint _commonICDADTHostIPAddressInt = 0;
    QString _commonICDADTHostIPAddress = "127.0.0.1";
    uint _commonICDADTHostPort = 0;
    uint _commonICDADTDiscoveryPort = 0;
    //-----------------------------------------------

  signals:
    //---------------- GDT signals ------------------
    void transmitterOperationalModeChanged();
    void gdtAntennaSelectChanged();
    void gdtPedestalTrackModeChanged();
    void gdtTddOperationalModeChanged();
    void gdtFrequencyModeChanged();
    void gdtUnitModeChanged();
    void gdtSymbolRateChanged();
    void gdtAesEncryptionChanged();

    void gdtTDDSyncChanged();
    void gdtLinkRSSIChanged();
    void gdtLinkTransferedPacketsChanged();
    void gdtLinkErrorPacketsChanged();
    void gdtLinkCRCErrorPacketsChanged();

    void gdtOperationFrequencyChanged();

    void gdtCBITPAPowerOutputChanged();
    void gdtCBITPAReturnPowerChanged();
    //-----------------------------------------------

    //---------------- ADT signals ------------------
    void adtTransmitterOperationalModeChanged();
    void adtAntennaSelectChanged();
    void adtTddOperationalModeChanged();
    void adtFrequencyModeChanged();
    void adtVideoTransmitEnableChanged();
    void adtUnitModeChanged();
    void adtSymbolRateChanged();
    void adtAesEncryptionChanged();
    void adtTelemetryMetadataSeparationChanged();

    void adtTDDSyncChanged();
    void adtLinkRSSIChanged();
    void adtLinkTransferedPacketsChanged();
    void adtLinkErrorPacketsChanged();
    void adtLinkCRCErrorPacketsChanged();

    void adtOperationFrequencyChanged();

    void adtCBITPAPowerOutputChanged();
    void adtCBITPAReturnPowerChanged();

    void adtVideoRateChanged();
    void adtVideoSourceChanged();
    //-----------------------------------------------

    //---------------- Common GDT signals -----------
    void commonICDIPAddressChanged();
    void commonICDIPAddressIntChanged();
    void commonICDPortChanged();
    void commonICDSubnetMaskChanged();
    void commonICDSubnetMaskIntChanged();
    void commonICDDefaultGatewayChanged();
    void commonICDDefaultGatewayIntChanged();
    void commonICDHostIPAddressChanged();
    void commonICDHostIPAddressIntChanged();
    void commonICDHostPortChanged();
    void commonICDDiscoveryPortChanged();
    //-----------------------------------------------

    //---------------- Common ADT signals -----------
    void commonICDADTIPAddressChanged();
    void commonICDADTIPAddressIntChanged();
    void commonICDADTPortChanged();
    void commonICDADTSubnetMaskChanged();
    void commonICDADTSubnetMaskIntChanged();
    void commonICDADTDefaultGatewayChanged();
    void commonICDADTDefaultGatewayIntChanged();
    void commonICDADTHostIPAddressChanged();
    void commonICDADTHostIPAddressIntChanged();
    void commonICDADTHostPortChanged();
    void commonICDADTDiscoveryPortChanged();
    //-----------------------------------------------

  public slots:
    void _commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message, int message_size);
};

#endif // COMMTACT_MANAGEMENT_H
