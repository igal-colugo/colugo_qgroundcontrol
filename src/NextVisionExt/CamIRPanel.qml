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
    anchors.fill: parent
    visible: true

    property var _irNrLevels: ["Low", "Medium", "High"]

    GridLayout {

        id: grid

        columns: 6
        rows: 10
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 5
        rowSpacing: 5

        onWidthChanged: {
            console.log("IR mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("IR")
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

            id: _dayModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("DAY")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysSensorDayCommand()
            }
        }

        QGCButton {

            id: _thermalModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("THERMAL")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysSensorIrCommand()
            }
        }

        QGCButton {

            id: _whitehotModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("WhiteHot")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrPolarityWHCommand()
            }
        }

        QGCButton {

            id: _blackhotModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("BlackHot")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrPolarityBHCommand()
            }
        }

        QGCButton {

            id: _colorModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Color.P")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrColorPCommand()
            }
        }

        QGCButton {

            id: _nucModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("NUC")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrNUCCommand()
            }
        }

        QGCButton {

            id: _bwpModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("B/W.P")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysIrBWPCommand()
            }
        }

        QGCLabel {

            id: _irNrLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("IR Noise Reduction")
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
            id: _irNrLevelsCombo
            sizeToContents: true
            centeredLabel: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.mediumFontPointSize
            model: _irNrLevels

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _rtrctModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Set")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
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
