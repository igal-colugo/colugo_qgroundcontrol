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
    id: widgetAsioInit
    anchors.fill: parent
    visible: true

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)
    property int columnWidth: (parent.width - (grid.columns * grid.columnSpacing)) / (grid.columns)
    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    property var _activeVehicleCoordinate: _activeVehicle ? _activeVehicle.coordinate : QtPositioning.coordinate()
    // The following properties relate to a simple camera
    property var _flyViewSettings: QGroundControl.settingsManager.flyViewSettings

    GridLayout {

        id: grid

        columns: 6
        rows: 4
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("Obox init:", grid.width, grid.height)
        }

        QGCLabel {

            id: _mainlabel

            text: qsTr("OBOX INIT")
            font.family: ScreenTools.demiboldFontFamily
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            //Layout.preferredHeight: rowHeight
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCLabel {

            id: _latitudelabel

            text: qsTr("LAT")
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
        }

        QGCTextField {
            id: _latitude

            maximumLength: 10
            text: {
                _fromVehicleLocation.checked ? _activeVehicleCoordinate.latitude : _latitude.text
            }

            Layout.rowSpan: 1
            Layout.columnSpan: 4

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _longitudelabel

            text: qsTr("LON")
            color: "White"

            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
        }

        QGCTextField {
            id: _longitude

            maximumLength: 10
            text: {
                _fromVehicleLocation.checked ? _activeVehicleCoordinate.latitude : _latitude.text
            }

            Layout.rowSpan: 1
            Layout.columnSpan: 4

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _initOboxButton

            showBorder: true
            text: qsTr("INIT")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (_activeVehicle !== null) {
                    _activeVehicle.setAsioInitLocation(_latitude.text,
                                                       _longitude.text)
                }
            }
        }

        QGCCheckBox {
            id: _fromVehicleLocation
            text: qsTr("VEHICLE")
            textColor: "white"

            Layout.rowSpan: 1
            Layout.columnSpan: 4

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }
    }
}
