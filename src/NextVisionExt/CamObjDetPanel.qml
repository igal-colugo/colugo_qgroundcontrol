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

    property var _odNetTypes: ["Human & Vehicle", "Fire & Smoke", "Human Overboard", "Marine Vessel"]

    GridLayout {
        id: grid

        columns: 6
        rows: 6
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

            text: qsTr("OBJ DET")
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

            id: _odEnableButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("OD Enable")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysObjDetOnCommand()
            }
        }

        QGCButton {

            id: _odDisableButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("OD Disable")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysObjDetOffCommand()
            }
        }

        QGCLabel {

            id: detectorTypeLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("Detector Type")
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
            id: netTypeCombo
            sizeToContents: true
            centeredLabel: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            model: _odNetTypes

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _setTypeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Set")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (netTypeCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysObjDetSetNetTypeCommand(
                                netTypeCombo.currentIndex)
            }
        }

        QGCLabel {

            id: _thresholdLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("Conf thres[%]")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.mediumFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCTextField {

            id: _confThres
            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            text: qsTr("")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _setThresholdButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Set")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysObjDetSetConfThresCommand(
                            _confThres.text)
            }
        }

        QGCLabel {

            id: _firethresholdLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("Fire thres")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.mediumFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCTextField {

            id: _fireThres
            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            text: qsTr("")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _setFireThresholdButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Set")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysObjDetSetFireThresCommand(
                            _fireThres.text)
            }
        }
    }
}
