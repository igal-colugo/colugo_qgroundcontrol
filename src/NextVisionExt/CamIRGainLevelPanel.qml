import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2

import QGroundControl 1.0
import QGroundControl.Palette 1.0
import QGroundControl.Controls 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FactControls 1.0

Item {
    id: widgetIRGainLevel
    anchors.fill: parent
    visible: true

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)

    GridLayout {

        id: grid

        columns: 6
        rows: 6
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("IR gain mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("IR GAIN LVL")
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

            id: _gainincModeButton

            showBorder: true
            text: qsTr("G INCREASE")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrGainLevelCommand(3)
            }
        }

        QGCButton {

            id: _gaindecModeButton

            showBorder: true
            text: qsTr("G DECREASE")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrGainLevelCommand(2)
            }
        }

        QGCButton {

            id: _levelincModeButton

            showBorder: true
            text: qsTr("L INCREASE")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrGainLevelCommand(1)
            }
        }

        QGCButton {

            id: _leveldecModeButton

            showBorder: true
            text: qsTr("L DECREASE")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrGainLevelCommand(0)
            }
        }

        QGCButton {

            id: _resetgainlevelModeButton

            showBorder: true
            text: qsTr("RESET")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrGainLevelCommand(4)
            }
        }
    }
}
