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

    id: widgetQuick

    anchors.fill: parent

    //color: Qt.rgba(0.0, 0.0, 0.0, 0.25)
    visible: true

    GridLayout {
        id: grid

        columns: 6
        rows: 6
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 5
        rowSpacing: 5

        onWidthChanged: {
            console.log("NV Camera mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("QUICK")
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

            id: _rateModeButton

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

            id: _pilotModeButton

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

            id: _dayModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("DAY")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

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
            Layout.columnSpan: 3

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

            id: _opticalZoomInModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Zoom In")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.cameraManagement.setSysZoomInCommand()
            }
            onReleased: {
                joystickManager.cameraManagement.setSysZoomStopCommand()
            }
        }

        QGCButton {

            id: _opticalZoomOutModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Zoom Out")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.cameraManagement.setSysZoomOutCommand()
            }
            onReleased: {
                joystickManager.cameraManagement.setSysZoomStopCommand()
            }
        }

        QGCButton {

            id: _autoFocusModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Near")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.cameraManagement.setSysSetFocusState(true,
                                                                     false,
                                                                     false)
            }
            onReleased: {
                joystickManager.cameraManagement.setSysSetFocusState(false,
                                                                     false,
                                                                     false)
            }
        }

        QGCButton {

            id: _manualFocusModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Inf")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.cameraManagement.setSysSetFocusState(false,
                                                                     false,
                                                                     true)
            }
            onReleased: {
                joystickManager.cameraManagement.setSysSetFocusState(false,
                                                                     false,
                                                                     false)
            }
        }

        QGCButton {

            id: _oneShotFocusModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Far")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.cameraManagement.setSysSetFocusState(false,
                                                                     true,
                                                                     false)
            }
            onReleased: {
                joystickManager.cameraManagement.setSysSetFocusState(false,
                                                                     false,
                                                                     false)
            }
        }

        QGCLabel {

            id: _setCameraScreenInformationLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr(" FOV[deg]")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.smallFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCTextField {

            id: _fovDeg

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            text: qsTr("62.0")
        }

        QGCButton {

            id: _setCameraScreenInformationButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("Set")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysFOVCommand(_fovDeg.text)
            }
        }
    }
}
