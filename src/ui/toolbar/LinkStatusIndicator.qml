

/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/
import QtQuick 2.11
import QtQuick.Layouts 1.11

import QGroundControl 1.0
import QGroundControl.Controls 1.0
import QGroundControl.MultiVehicleManager 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Palette 1.0

//-------------------------------------------------------------------------
//-- Link status
Item {
    id: _root
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    width: (telemIcon.width + linkStatusLostPercentLabel.width) * 1.1

    property bool showIndicator: true //_hasTelemetry

    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle

    Component {
        id: telemRSSIInfo
        Rectangle {
            width: telemCol.width + ScreenTools.defaultFontPixelWidth * 3
            height: telemCol.height + ScreenTools.defaultFontPixelHeight * 2
            radius: ScreenTools.defaultFontPixelHeight * 0.5
            color: qgcPal.window
            border.color: qgcPal.text
            Column {
                id: telemCol
                spacing: ScreenTools.defaultFontPixelHeight * 0.5
                width: Math.max(telemGrid.width, telemLabel.width)
                anchors.margins: ScreenTools.defaultFontPixelHeight
                anchors.centerIn: parent
                QGCLabel {
                    id: telemLabel
                    text: qsTr("Link status")
                    font.family: ScreenTools.demiboldFontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                }
                GridLayout {
                    id: telemGrid
                    anchors.margins: ScreenTools.defaultFontPixelHeight
                    columnSpacing: ScreenTools.defaultFontPixelWidth
                    columns: 2
                    anchors.horizontalCenter: parent.horizontalCenter

                    QGCLabel {
                        text: qsTr("Link name:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.vehicleLinkManager.primaryLinkName : qsTr(
                                                   "")
                    }
                    QGCLabel {
                        text: qsTr("Messages recieved:")
                    }
                    QGCLabel {
                        text: _activeVehicle.mavlinkReceivedCount
                    }
                    QGCLabel {
                        text: qsTr("Messages sent:")
                    }
                    QGCLabel {
                        text: _activeVehicle.mavlinkSentCount
                    }
                    QGCLabel {
                        text: qsTr("Messages lost:")
                    }
                    QGCLabel {
                        text: _activeVehicle.mavlinkLossCount
                    }
                    QGCLabel {
                        text: qsTr("Link quality %:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? (100.0 - _activeVehicle.mavlinkLossPercent).toFixed(
                                                   1) + '%' : qsTr(
                                                   "Not Connected")
                    }
                }
            }
        }
    }
    QGCColoredImage {
        id: telemIcon
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: height
        sourceSize.height: height
        source: "/qmlimages/TelemRSSI.svg"
        fillMode: Image.PreserveAspectFit
        color: qgcPal.buttonText
    }
    QGCLabel {
        id: linkStatusLostPercentLabel
        text: _activeVehicle ? (100.0 - _activeVehicle.mavlinkLossPercent).toFixed(
                                   1) + '%' : qsTr("Not Connected")
        anchors.left: telemIcon.right
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            mainWindow.showIndicatorPopup(_root, telemRSSIInfo)
        }
    }
}
