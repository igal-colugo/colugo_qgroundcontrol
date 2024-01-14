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
    id: widgetIR
    anchors.fill: parent
    visible: true

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)
    property int columnWidth: (parent.width - (grid.columns * grid.columnSpacing)) / (grid.columns)
    property var _irNrLevels: ["Low", "Medium", "High"]

    GridLayout {

        id: grid

        columns: 6
        rows: 7
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("IR mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("IR")
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

            id: _dayModeButton

            showBorder: true
            text: qsTr("DAY")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysSensorDayCommand()
            }
        }

        QGCButton {

            id: _thermalModeButton

            showBorder: true
            text: qsTr("THERMAL")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysSensorIrCommand()
            }
        }

        QGCButton {

            id: _whitehotModeButton

            showBorder: true
            text: qsTr("WHT HOT")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrPolarityWHCommand()
            }
        }

        QGCButton {

            id: _blackhotModeButton

            showBorder: true
            text: qsTr("BLK HOT")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrPolarityBHCommand()
            }
        }

        QGCButton {

            id: _colorModeButton

            showBorder: true
            text: qsTr("CLR P")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrColorPCommand()
            }
        }

        QGCButton {

            id: _nucModeButton

            showBorder: true
            text: qsTr("NUC")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrNUCCommand()
            }
        }

        QGCButton {

            id: _bwpModeButton

            showBorder: true
            text: qsTr("B/W.P")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrBWPCommand()
            }
        }

        QGCLabel {

            id: _irNrLabel

            text: qsTr("NOISE REDUCTION")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCComboBox {
            id: _irNrLevelsCombo
            sizeToContents: true
            centeredLabel: true
            model: _irNrLevels

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _rtrctModeButton

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (_irNrLevelsCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysSetIRNoiseReductionLevelCommand(
                                _irNrLevelsCombo.currentIndex)
            }
        }
    }
}
