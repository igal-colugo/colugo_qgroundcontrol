import QtQuick          2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles  1.4
import QtQuick.Dialogs  1.2
import QtQuick.Layouts  1.2
import QtPositioning    5.3

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

    property int _heightTotal: mainlabel.height + firstRow.height + secondRow.height+lastRow.height + (_margins*8)

    property var    _activeVehicle:             QGroundControl.multiVehicleManager.activeVehicle
    property var    _activeVehicleCoordinate:   _activeVehicle ? _activeVehicle.coordinate : QtPositioning.coordinate()
    // The following properties relate to a simple camera
    property var    _flyViewSettings:                           QGroundControl.settingsManager.flyViewSettings

    QGCLabel {
        id:                 mainlabel
        text:               qsTr("OBOX init")
        anchors.margins:    _margins
        font.family:        ScreenTools.demiboldFontFamily
        font.pointSize:     ScreenTools.largeFontPointSize
        anchors.horizontalCenter:  parent.horizontalCenter
        anchors.top:        parent.top
        color:              "white"
        height:             ScreenTools.defaultFontPixelHeight
    }

    RowLayout {
        id:                         firstRow
        anchors.top:                mainlabel.bottom
        anchors.margins:            _margins * 4
        spacing:                    _butMargins
        visible:                    true

        QGCLabel {
            id: _longitudelabel
            text:                   qsTr("Latitude")
            font.pointSize:         12
            color:                  "white"
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            leftPadding:_butMargins
        }
        QGCTextField {
            id:                     _latitude
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            maximumLength:          10
            font.pointSize:         12
            text:                  {
               _fromVehicleLocation.checked? _activeVehicleCoordinate.latitude : _latitude.text
            }
        }
    }

    RowLayout {
        id:                         secondRow
        anchors.top:                firstRow.bottom
        anchors.margins:            _margins
        spacing:                    _butMargins
        visible:                    true

        QGCLabel {
            text:                   qsTr("Longitude")
            font.pointSize:         12
            color:                  "white"
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            leftPadding:_butMargins
        }
        QGCTextField {
            id:                     _longitude
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            maximumLength:          10
            font.pointSize:         12
            text:                    {
                _fromVehicleLocation.checked? _activeVehicleCoordinate.longitude : _longitude.text
            }
        }
    }

    RowLayout {
        id:                         lastRow
        anchors.top:                secondRow.bottom
        spacing:                    _butMargins
        anchors.margins:            _margins
        visible:                    true

        QGCButton {
            showBorder:     true
            font.pointSize: ScreenTools.isMobile? point_size : ScreenTools.smallFontPointSize
            pointSize:      ScreenTools.isMobile? point_size : ScreenTools.defaultFontPointSize
            text:           qsTr("Init")
            leftPadding:    0
            Layout.leftMargin: _margins
            onReleased: {
                if( _activeVehicle !== null)
                {
                    _activeVehicle.setAsioInitLocation( _latitude.text,_longitude.text);
                }
            }
        }

        QGCCheckBox {
            id:                     _fromVehicleLocation
            text:                   qsTr("get from vehicle")
            textColor:              "white"
            textFontPointSize:      ScreenTools.isMobile ? point_size : 9
        }
    }
}


