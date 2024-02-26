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

    Q_INVOKABLE void setGDTOperationalModeCommand(uint transmitter_operational_mode);
    // UI state management functions (called periodically, their result is meant to update Qml proprties dynamically)
    Q_INVOKABLE bool getShouldUiEnabledRescueElement();

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

  protected:
    CommtactLinkManager *_commtactLinkManager = nullptr;

  private:
    uint8_t _transmitterOperationalMode;

  signals:
    void transmitterOperationalModeChanged();

  public slots:
    void _commtactLinkMessageReceived(CommtactLinkInterface *link, CommtactLinkProtocol::commtact_link_message_t message);
};

#endif // COMMTACT_MANAGEMENT_H
