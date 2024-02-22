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

class CommtactLinkManagement : public QObject
{
    Q_OBJECT

  public:
    explicit CommtactLinkManagement(QObject *parent = nullptr);
    CommtactLinkManagement(QGCApplication *app, QGCToolbox *toolbox);
    CommtactLinkManagement(CommtactLinkManager *commtactLinkManager);

    void sendGimbalCommand(float cam_roll_yaw, float cam_pitch);

    std::atomic<uint8_t> m_should_enable_rescue_btn{};

    Q_INVOKABLE void setCameraModeCommand(uint control_mode);
    // UI state management functions (called periodically, their result is meant to update Qml proprties dynamically)
    Q_INVOKABLE bool getShouldUiEnabledRescueElement();

  protected:
    CommtactLinkManager *_commtactLinkManager;

  private:
  public slots:
};

#endif // COMMTACT_MANAGEMENT_H
