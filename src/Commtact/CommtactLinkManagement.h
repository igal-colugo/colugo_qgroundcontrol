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
    //------------------- GDT commands ----------------------------------
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

  protected:
    CommtactLinkManager *_commtactLinkManager = nullptr;

  private:
    QTimer _updateTimer;

    //--------------- GDT fields --------------------
    uint8_t _transmitterOperationalMode;
    uint8_t _gdtAntennaSelect;
    uint8_t _gdtPedestalTrackMode;
    uint8_t _gdtTddOperationalMode;
    uint8_t _gdtFrequencyMode;
    uint8_t _gdtUnitMode;
    uint8_t _gdtSymbolRate;
    uint8_t _gdtAesEncryption;

    int8_t _gdtTDDSync;
    int8_t _gdtLinkRSSI;
    uint16_t _gdtLinkTransferedPackets;
    uint16_t _gdtLinkErrorPackets;
    uint16_t _gdtLinkCRCErrorPackets;

    uint16_t _gdtOperationFrequency;

    int16_t _gdtCBITPAPowerOutput;
    int16_t _gdtCBITPAReturnPower;

    CommtactLinkProtocol::commtact_gdt_operational_modes_report_t _operational_modes_report;
    CommtactLinkProtocol::commtact_gdt_status_report_t _gdt_status_report;
    CommtactLinkProtocol::commtact_gdt_constant_frequency_report_t _gdt_constant_frequency_report;
    CommtactLinkProtocol::commtact_gdt_cbit_report_t _gdt_cbit_report;
    CommtactLinkProtocol::commtact_gdt_mission_adt_status_report_t _gdt_mission_adt_status_report;

    //--------------- Adt fields --------------------
    uint8_t _adtTransmitterOperationalMode;
    uint8_t _adtAntennaSelect;
    uint8_t _adtTddOperationalMode;
    uint8_t _adtFrequencyMode;
    uint8_t _adtVideoTransmitEnable;
    uint8_t _adtUnitMode;
    uint8_t _adtSymbolRate;
    uint8_t _adtAesEncryption;
    uint8_t _adtTelemetryMetadataSeparation;

    int8_t _adtTDDSync;
    int8_t _adtLinkRSSI;
    uint16_t _adtLinkTransferedPackets;
    uint16_t _adtLinkErrorPackets;
    uint16_t _adtLinkCRCErrorPackets;

    uint16_t _adtOperationFrequency;

    int16_t _adtCBITPAPowerOutput;
    int16_t _adtCBITPAReturnPower;

    uint8_t _adtVideoRate;
    uint8_t _adtVideoSource;

    CommtactLinkProtocol::commtact_adt_operational_modes_report_t _adt_operational_modes_report;
    CommtactLinkProtocol::commtact_adt_constant_frequency_report_t _adt_constant_frequency_report;
    CommtactLinkProtocol::commtact_adt_status_report_t _adt_status_report;

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

  public slots:
    void _commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message, int message_size);
};

#endif // COMMTACT_MANAGEMENT_H
