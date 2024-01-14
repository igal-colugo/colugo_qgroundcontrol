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
        rows: 6
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("NV Camera mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("NV QUICK")
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

            id: _rateModeButton

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

            id: _pilotModeButton

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

            id: _opticalZoomInModeButton

            showBorder: true
            text: qsTr("ZOOM IN")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
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
            text: qsTr("ZOOM OUT")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 3
            Layout.fillWidth: false
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
            text: qsTr("NEAR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
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
            text: qsTr("INF")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
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
            text: qsTr("FAR")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
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

            text: qsTr(" FOV")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
            Layout.columnSpan: 2
            Layout.fillHeight: false
        }

        QGCTextField {

            id: _fovDeg

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            maximumLength: 6
            font.pointSize: ScreenTools.isMobile ? point_size : 9
            text: qsTr("62.0")
        }

        QGCButton {

            id: _setCameraScreenInformationButton

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysFOVCommand(_fovDeg.text)
            }
        }
    }
}
