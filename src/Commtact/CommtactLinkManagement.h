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

class CommtactLinkManagement : public QGCTool
{
    Q_OBJECT

  public:
    CommtactLinkManagement(QGCApplication *app, QGCToolbox *toolbox);
    // Override from QGCTool
    virtual void setToolbox(QGCToolbox *toolbox);

    std::atomic<uint8_t> m_should_enable_rescue_btn{};

    // UI state management functions (called periodically, their result is meant to update Qml proprties dynamically)
    Q_INVOKABLE bool getShouldUiEnabledRescueElement();

    //---------------------- Properties ---------------------------------
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

    QStringList operationalModeTypeStrings(void) const;
    QStringList gdtAntennaSelectTypeStrings(void) const;
    QStringList gdtPedestalTrackModeTypeStrings(void) const;
    QStringList gdtTddOperationalModeTypeStrings(void) const;
    QStringList gdtFrequencyModeTypeStrings(void) const;
    QStringList gdtUnitModeTypeStrings(void) const;
    QStringList gdtSymbolRateTypeStrings(void) const;
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
    //-------------------------------------------------------------------

  protected:
    CommtactLinkManager *_commtactLinkManager = nullptr;

  private:
    uint8_t _transmitterOperationalMode;
    uint8_t _gdtAntennaSelect;
    uint8_t _gdtPedestalTrackMode;
    uint8_t _gdtTddOperationalMode;
    uint8_t _gdtFrequencyMode;
    uint8_t _gdtUnitMode;
    uint8_t _gdtSymbolRate;
    uint8_t _gdtAesEncryption;

    CommtactLinkProtocol::commtact_gdt_operational_modes_report_t _operational_modes_report;
    CommtactLinkProtocol::commtact_gdt_status_report_t _gdt_status_report;

  signals:
    void transmitterOperationalModeChanged();
    void gdtAntennaSelectChanged();
    void gdtPedestalTrackModeChanged();
    void gdtTddOperationalModeChanged();
    void gdtFrequencyModeChanged();
    void gdtUnitModeChanged();
    void gdtSymbolRateChanged();
    void gdtAesEncryptionChanged();

  public slots:
    void _commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message);
};

#endif // COMMTACT_MANAGEMENT_H
