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

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("EPS QUICK")
            font.family: ScreenTools.demiboldFontFamily
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
            text: qsTr("RATE")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(1)
            }
        }

        QGCButton {

            id: _pilotModeButton

            showBorder: true
            text: qsTr("PLT")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(9)
            }
        }

        QGCButton {

            id: _stowModeButton

            showBorder: true
            text: qsTr("STW")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(8)
            }
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
                joystickManager.epsilonCameraManagement.setCameraOrderCommand(0)
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
                joystickManager.epsilonCameraManagement.setCameraOrderCommand(1)
            }
        }

        QGCButton {

            id: _opticalZoomInModeButton

            showBorder: true
            text: qsTr("OZOOMI")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.epsilonCameraManagement.setOpticalZoomSpeed(6)
            }
            onReleased: {
                joystickManager.epsilonCameraManagement.setOpticalZoomSpeed(0)
            }
        }

        QGCButton {

            id: _opticalZoomOutModeButton

            showBorder: true
            text: qsTr("OZOOMO")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.epsilonCameraManagement.setOpticalZoomSpeed(-6)
            }
            onReleased: {
                joystickManager.epsilonCameraManagement.setOpticalZoomSpeed(0)
            }
        }

        QGCButton {

            id: _digitalZoomInModeButton

            showBorder: true
            text: qsTr("DZOOMI")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.epsilonCameraManagement.setSysZoomInCommand()
            }
        }

        QGCButton {

            id: _digitalZoomOutModeButton

            showBorder: true
            text: qsTr("DZOOMO")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.epsilonCameraManagement.setSysZoomOutCommand()
            }
        }

        QGCButton {

            id: _autoFocusModeButton

            showBorder: true
            text: qsTr("AUTO")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.epsilonCameraManagement.setFocusModeCommand(0)
            }
        }

        QGCButton {

            id: _manualFocusModeButton

            showBorder: true
            text: qsTr("MAN")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.epsilonCameraManagement.setFocusModeCommand(1)
            }
        }

        QGCButton {

            id: _oneShotFocusModeButton

            showBorder: true
            text: qsTr("SHOT")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onPressed: {
                joystickManager.epsilonCameraManagement.setFocusModeCommand(2)
            }
        }

        QGCLabel {

            id: _setCameraScreenInformationLabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("INFO")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCTextField {

            id: _setCameraScreenInformationField

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            maximumLength: 4
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            text: qsTr("2047")
        }

        QGCButton {

            id: _setCameraScreenInformationButton

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (parseInt(_setCameraScreenInformationField.text) >= 0
                        && parseInt(
                            _setCameraScreenInformationField.text) <= 2047) {
                    joystickManager.epsilonCameraManagement.setCameraScreenInformationCommand(
                                parseInt(_setCameraScreenInformationField.text))
                }
            }
        }
    }
}
