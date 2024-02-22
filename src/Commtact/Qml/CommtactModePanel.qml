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

//import "../../QmlControls"
Item {
    id: widgetMode
    anchors.fill: parent
    visible: true

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)
    property int columnWidth: (parent.width - (grid.columns * grid.columnSpacing)) / (grid.columns)

    GridLayout {

        id: grid

        columns: 6
        rows: 5
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

            text: qsTr("MODE")
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
            Layout.columnSpan: 3

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

            id: _rateAidModeButton

            showBorder: true
            text: qsTr("RATE AID")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(2)
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
                joystickManager.epsilonCameraManagement.setCameraModeCommand(8)
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
                joystickManager.epsilonCameraManagement.setCameraModeCommand(9)
                console.log("Pilot btn:", _pilotModeButton.width, grid.height)
            }
        }

        QGCButton {

            id: _geoLockModeButton

            showBorder: true
            text: qsTr("GEO LCK")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(7)
            }
        }

        QGCButton {

            id: _trackStationaryModeButton

            showBorder: true
            text: qsTr("TRCK STAT")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(3)
            }
        }

        QGCButton {

            id: _trackVehicleModeButton

            showBorder: true
            text: qsTr("TRCK VHCL")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(4)
            }
        }

        QGCButton {

            id: _trackSceneModeButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("TRCK SCN")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(5)
            }
        }

        QGCButton {

            id: _trackStaticModeButton

            showBorder: true
            text: qsTr("TRCK STAC")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraModeCommand(6)
            }
        }
    }
}
