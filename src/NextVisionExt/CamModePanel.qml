import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2

import QGroundControl 1.0
import QGroundControl.Palette 1.0
import QGroundControl.Controls 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FactControls 1.0
import QGroundControl.SettingsManager 1.0

Item {
    anchors.fill: parent
    visible: true

    GridLayout {

        id: grid

        columns: 6
        rows: 7
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 5
        rowSpacing: 5

        onWidthChanged: {
            console.log("Camera mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("MODE")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCButton {

            id: _holdModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("HOLD")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeHoldCommand()
            }
        }

        QGCButton {

            id: _obsAidModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("OBS")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeObsCommand()
            }
        }

        QGCButton {

            id: _grrModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("GRR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeGrrCommand()
            }
        }

        QGCButton {

            id: _pilotModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("PILOT")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModePilotCommand()
            }
        }

        QGCButton {

            id: _stowModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("STOW")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeStowCommand()
            }
        }

        QGCButton {

            id: _eprModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("EPR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeEprCommand()
            }
        }

        QGCButton {

            id: _trackModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("TRACK")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeTrackCommand()
            }
        }

        QGCButton {

            id: _rtrctModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("RTRCT")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeRetractCommand()
            }
        }

        QGCButton {

            id: _rtrctrModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("RTRCTR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeRetractUnlockCommand(
                            )
            }
        }

        QGCButton {

            id: _nadirModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Nadir")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysNadirCommand()
            }
        }

        QGCButton {

            id: _nadirScanModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Nadir Scan")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysNadirScanCommand()
            }
        }

        QGCButton {

            id: _scanModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("2D Scan")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysMode2DScanCommand()
            }
        }

        QGCButton {

            id: _motorsOffModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Motors Off")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setCameraMotorState(true)
            }
        }

        QGCButton {

            id: _motorsOnModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Motors On")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setCameraMotorState(true)
            }
        }
    }
}
