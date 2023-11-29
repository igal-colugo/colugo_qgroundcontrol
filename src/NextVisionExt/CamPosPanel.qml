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
    id: element
    anchors.fill: parent

    visible: true

    property var _positionsStr: ["Local Position", "Un-stabilized Position"]

    property Fact _localPositionRoll: QGroundControl.settingsManager.appSettings.localPositionRoll
    property Fact _localPositionPitch: QGroundControl.settingsManager.appSettings.localPositionPitch
    property Fact _globalPositionElevation: QGroundControl.settingsManager.appSettings.globalPositionElevation
    property Fact _globalPositionAzimuth: QGroundControl.settingsManager.appSettings.globalPositionAzimuth

    GridLayout {
        id: grid

        columns: 6
        rows: 7
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 5
        rowSpacing: 5

        onWidthChanged: {
            console.log("Record mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("POSITION")
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

        QGCComboBox {
            id: localUnstabPosCombo
            sizeToContents: true
            centeredLabel: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.mediumFontPointSize
            model: _positionsStr

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _rollLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("Roll")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 3
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        FactTextField {

            id: _localPosRoll
            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            fact: _localPositionRoll

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _pitchLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("Pitch")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 3
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        FactTextField {

            id: _localPosPitch
            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            fact: _localPositionPitch

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: localPosBtn

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Set")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (localUnstabPosCombo.currentIndex === 0)
                    joystickManager.cameraManagement.setSysModeLocalPositionCommand(
                                _localPosPitch.text, _localPosRoll.text)
                else if (localUnstabPosCombo.currentIndex === 1)
                    joystickManager.cameraManagement.setSysModeUnstabilizedPositionCommand(
                                _localPosPitch.text, _localPosRoll.text)
            }
        }

        QGCLabel {

            id: globalposlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("GLOBAL")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCLabel {

            id: _elevatorLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("Elevation")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 3
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        FactTextField {

            id: _globalPosElev
            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            fact: _globalPositionElevation

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _azimuthLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("Azimuth")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 3
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        FactTextField {

            id: _globalPosAz
            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            fact: _globalPositionAzimuth

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: globalPosBtn

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Set Global Position")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysModeGlobalPositionCommand(
                            _globalPosElev.text, _globalPosAz.text)
            }
        }
    }
}
