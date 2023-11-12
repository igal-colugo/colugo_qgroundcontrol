import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs  1.2
import QtQuick.Layouts  1.2

import QGroundControl               1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controllers   1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0
import QGroundControl.SettingsManager 1.0

Rectangle {
    id: _initialize_obox_panel_rectangle
    height:             _heightTotal
    color:              Qt.rgba(0.0,0.0,0.0,0.25)
    visible:            true
    radius:     _margins

    property int _heightTotal: mainlabel.height + lastRow.height + (_margins*6)

    property var    _activeVehicle:             QGroundControl.multiVehicleManager.activeVehicle

    // The following properties relate to a simple camera
    property var    _flyViewSettings:                           QGroundControl.settingsManager.flyViewSettings

    QGCLabel {
        id:                 mainlabel
        text:               qsTr("OBOX config")
        anchors.margins:    _margins
        font.family:        ScreenTools.demiboldFontFamily
        font.pointSize:     ScreenTools.largeFontPointSize
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.top:        parent.top
        color:              "white"
        height:             ScreenTools.defaultFontPixelHeight
    }

    RowLayout {
        id:                         lastRow
        anchors.top:                mainlabel.bottom
        spacing:                    _butMargins
        anchors.margins:            _margins * 4
        visible:                    true

        QGCButton {
            id:             setAsioDay
            showBorder:     true
            font.pointSize: ScreenTools.isMobile? point_size : ScreenTools.smallFontPointSize
            pointSize:      ScreenTools.isMobile? point_size : ScreenTools.defaultFontPointSize
            text:           qsTr("Set day")
            leftPadding:    0
            Layout.leftMargin: _margins
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2 - _margins*2) / 2
            onReleased: {
                if( _activeVehicle !== null)
                    _activeVehicle.setAsioSensorType(1);
            }
        }

        QGCButton {
            id:             setAsioThermal
            showBorder:     true
            font.pointSize: ScreenTools.isMobile? point_size : ScreenTools.smallFontPointSize
            pointSize:      ScreenTools.isMobile? point_size : ScreenTools.defaultFontPointSize
            text:           qsTr("Set thermal")
            leftPadding:    0
            Layout.leftMargin: _margins
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2 - _margins*2) / 2
            onReleased: {
                if( _activeVehicle !== null)
                    _activeVehicle.setAsioSensorType(2);
            }
        }
    }
}


