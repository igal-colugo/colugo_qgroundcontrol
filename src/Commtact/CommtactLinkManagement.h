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
    Q_INVOKABLE void setADTUnitModeCommand(uint adt_unit_mode);
    Q_INVOKABLE void setADTSymbolRateCommand(uint adt_symbol_rate);
    Q_INVOKABLE void setADTAesEncryptionCommand(uint adt_aes_encryption);
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

    CommtactLinkProtocol::commtact_adt_operational_modes_report_t _adt_operational_modes_report;

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

    //-----------------------------------------------

  public slots:
    void _commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message, int message_size);
};

#endif // COMMTACT_MANAGEMENT_H
