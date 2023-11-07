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
    color:              Qt.rgba(0.0,0.0,0.0,0.50)
    visible:            true
    radius:     _margins

    property int _heightTotal: mainlabel.height + firstRow.height + secondRow.height+lastRow.height + (_margins*5)

    property var    _activeVehicle:             QGroundControl.multiVehicleManager.activeVehicle

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
        anchors.margins:            _margins
        spacing:                    _butMargins
        visible:                    true

        QGCLabel {
            id: _longitudelabel
            text:                   qsTr("Longitude")
            font.pointSize:         12
            color:                  "white"
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            leftPadding:_butMargins
        }
        QGCTextField {
            id:                     _longitude
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            maximumLength:          9
            font.pointSize:         12
            text:                   qsTr("0")
        }
    }

    RowLayout {
        id:                         secondRow
        anchors.top:                firstRow.bottom
        anchors.margins:            _margins
        spacing:                    _butMargins
        visible:                    true

        QGCLabel {
            text:                   qsTr("Latitude")
            font.pointSize:         12
            color:                  "white"
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            leftPadding:_butMargins
        }
        QGCTextField {
            id:                     _latitude
            Layout.preferredWidth:  (_initialize_obox_panel_rectangle.width - _butMargins*2) / 2
            maximumLength:          5
            font.pointSize:         12
            text:                   qsTr("0")
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
            text:           qsTr("Set")
            leftPadding:    0
            Layout.leftMargin: _margins
            onReleased: {
                //if( videoChannelCombo.currentIndex >= 0 )
                    //joystickManager.cameraManagement.setSysAutoSnapshotCommand(_snapInerval.text,_snapCount.text,_snapInf.checked, videoChannelCombo.currentIndex);
            }
        }
    }
}


