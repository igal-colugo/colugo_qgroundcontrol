import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2
import QtPositioning 5.3

import QGroundControl 1.0
import QGroundControl.Palette 1.0
import QGroundControl.Controls 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FactControls 1.0
import QGroundControl.SettingsManager 1.0

Item {
    anchors.fill: parent
    visible: true

    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    property var _activeVehicleCoordinate: _activeVehicle ? _activeVehicle.coordinate : QtPositioning.coordinate()
    // The following properties relate to a simple camera
    property var _flyViewSettings: QGroundControl.settingsManager.flyViewSettings

    GridLayout {

        id: grid

        columns: 2
        rows: 4
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 5
        rowSpacing: 5

        onWidthChanged: {
            console.log("Obox init:", grid.width, grid.height)
        }

        QGCLabel {

            id: _mainlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("OBOX INIT")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCLabel {

            id: _latitudelabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("LAT")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.columnSpan: 1
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCTextField {
            id: _latitude

            maximumLength: 10
            font.pointSize: 10
            text: {
                _fromVehicleLocation.checked ? _activeVehicleCoordinate.latitude : _latitude.text
            }

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _longitudelabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("LON")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 1
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCTextField {
            id: _longitude

            maximumLength: 10
            font.pointSize: 10
            text: {
                _fromVehicleLocation.checked ? _activeVehicleCoordinate.latitude : _latitude.text
            }

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _initOboxButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("INIT")

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setCameraOrderCommand(1)
            }
        }

        QGCCheckBox {
            id: _fromVehicleLocation
            text: qsTr("VEHICLE")
            textColor: "white"
            textFontPointSize: ScreenTools.isMobile ? point_size : 9

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }
    }
}
