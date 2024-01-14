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

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)
    property int columnWidth: (parent.width - (grid.columns * grid.columnSpacing)) / (grid.columns)

    GridLayout {
        id: grid

        columns: 8
        rows: 6
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("Position mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("POSITION")
            font.family: ScreenTools.demiboldFontFamily
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            //Layout.preferredHeight: rowHeight
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 8
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCComboBox {
            id: localUnstabPosCombo
            sizeToContents: true
            centeredLabel: true
            model: _positionsStr

            Layout.rowSpan: 1
            Layout.columnSpan: 8

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _rollLabel

            text: qsTr("RLL")
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            //Layout settings
            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 1
            Layout.fillHeight: false
            Layout.fillWidth: false
        }

        FactTextField {

            id: _localPosRoll
            maximumLength: 6
            fact: _localPositionRoll

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 3
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _pitchLabel

            text: qsTr("PTCH")
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 1
            Layout.fillHeight: false
            Layout.fillWidth: false
        }

        FactTextField {

            id: _localPosPitch
            maximumLength: 6
            fact: _localPositionPitch

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 3
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: localPosBtn

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 8

            Layout.preferredHeight: rowHeight
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

            id: _elevatorLabel

            text: qsTr("EL")
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 1
            Layout.fillHeight: false
            Layout.fillWidth: false
        }

        FactTextField {

            id: _globalPosElev
            maximumLength: 6
            fact: _globalPositionElevation

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 3
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _azimuthLabel

            text: qsTr("AZ")
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 1
            Layout.fillHeight: false
            Layout.fillWidth: false
        }

        FactTextField {

            id: _globalPosAz
            maximumLength: 6
            fact: _globalPositionAzimuth

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 3
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: globalPosBtn

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 8

            Layout.preferredHeight: rowHeight
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
