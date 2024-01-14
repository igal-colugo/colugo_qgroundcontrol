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
    id: widgetMode
    anchors.fill: parent
    visible: true

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)
    property int columnWidth: (parent.width - (grid.columns * grid.columnSpacing)) / (grid.columns)

    GridLayout {

        id: grid

        columns: 6
        rows: 7
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("Camera mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("MODE")
            font.family: ScreenTools.demiboldFontFamily
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            //Layout.preferredHeight: rowHeight
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCButton {

            id: _holdModeButton

            showBorder: true
            text: qsTr("HOLD")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeHoldCommand()
            }
        }

        QGCButton {

            id: _obsAidModeButton

            showBorder: true
            text: qsTr("OBS")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeObsCommand()
            }
        }

        QGCButton {

            id: _grrModeButton

            showBorder: true
            text: qsTr("GRR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeGrrCommand()
            }
        }

        QGCButton {

            id: _pilotModeButton

            showBorder: true
            text: qsTr("PILOT")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModePilotCommand()
            }
        }

        QGCButton {

            id: _stowModeButton

            showBorder: true
            text: qsTr("STOW")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeStowCommand()
            }
        }

        QGCButton {

            id: _eprModeButton

            showBorder: true
            text: qsTr("EPR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeEprCommand()
            }
        }

        QGCButton {

            id: _trackModeButton

            showBorder: true
            text: qsTr("TRACK")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeTrackCommand()
            }
        }

        QGCButton {

            id: _rtrctModeButton

            showBorder: true
            text: qsTr("RTRCT")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeRetractCommand()
            }
        }

        QGCButton {

            id: _rtrctrModeButton

            showBorder: true
            text: qsTr("RTRCTR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
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
            text: qsTr("NDIR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysNadirCommand()
            }
        }

        QGCButton {

            id: _nadirScanModeButton

            showBorder: true
            text: qsTr("NDIR SCN")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysNadirScanCommand()
            }
        }

        QGCButton {

            id: _scanModeButton

            showBorder: true
            text: qsTr("2D SC")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysMode2DScanCommand()
            }
        }

        QGCButton {

            id: _motorsOffModeButton

            showBorder: true
            text: qsTr("MOTORS OFF")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setCameraMotorState(true)
            }
        }

        QGCButton {

            id: _motorsOnModeButton

            showBorder: true
            text: qsTr("MOTORS ON")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setCameraMotorState(true)
            }
        }
    }
}
