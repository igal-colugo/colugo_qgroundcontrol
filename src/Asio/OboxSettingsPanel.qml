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

//import "../QmlControls"
Item {

    anchors.fill: parent
    visible: true

    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    // The following properties relate to a simple camera
    property var _flyViewSettings: QGroundControl.settingsManager.flyViewSettings

    GridLayout {

        id: _grid

        columns: 2
        rows: 2
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 5
        rowSpacing: 5

        onWidthChanged: {
            console.log("Obox settings:", _grid.width, _grid.height)
        }

        QGCLabel {

            id: _mainlabel

            text: qsTr("OBOX CONF")
            transformOrigin: Item.Center
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCButton {

            id: _dayOboxButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("DAY")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (_activeVehicle !== null) {
                    _activeVehicle.setAsioSensorType(1)
                }
            }
        }

        QGCButton {

            id: _thermalOboxButton

            showBorder: true
            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
            text: qsTr("THERMAL")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (_activeVehicle !== null) {
                    _activeVehicle.setAsioSensorType(2)
                }
            }
        }
    }
}
