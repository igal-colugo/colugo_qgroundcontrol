

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
import QGroundControl.FactSystem 1.0

//-------------------------------------------------------------------------
//-- GPS Indicator
Item {
    id: _root
    width: (gps_1ValuesColumn.x + gps_1ValuesColumn.width + gps_2Icon.width
            + gps_2ValuesColumn.width + _primaryParemeterGPSLabel.width
            + _primaryParemeterGPSValue.width) * 1.1
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    ///< FactPanelController
    property bool showIndicator: true

    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    property var _parameterManager: (_activeVehicle) ? _activeVehicle.parameterManager : null
    property bool _parametersReady: (_activeVehicle) ? _activeVehicle.parameterManager.parametersReady : false
    property Fact _sensGPSPrimeParameter: null

    Component {
        id: gps_1Info

        Rectangle {
            width: gps_1Col.width + ScreenTools.defaultFontPixelWidth * 3
            height: gps_1Col.height + ScreenTools.defaultFontPixelHeight * 2
            radius: ScreenTools.defaultFontPixelHeight * 0.5
            color: qgcPal.window
            border.color: qgcPal.text

            Column {
                id: gps_1Col
                spacing: ScreenTools.defaultFontPixelHeight * 0.5
                width: Math.max(gps_1Grid.width, gps_1Label.width)
                anchors.margins: ScreenTools.defaultFontPixelHeight
                anchors.centerIn: parent

                QGCLabel {
                    id: gps_1Label
                    text: (_activeVehicle && _activeVehicle.gps.count.value
                           >= 0) ? qsTr("GPS 1 Status") : qsTr(
                                       "GPS 1 Data Unavailable")
                    font.family: ScreenTools.demiboldFontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                GridLayout {
                    id: gps_1Grid
                    visible: (_activeVehicle
                              && _activeVehicle.gps.count.value >= 0)
                    anchors.margins: ScreenTools.defaultFontPixelHeight
                    columnSpacing: ScreenTools.defaultFontPixelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                    columns: 2

                    QGCLabel {
                        text: qsTr("GPS Count:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps.count.valueString : qsTr(
                                                   "N/A", "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("GPS Lock:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps.lock.enumStringValue : qsTr(
                                                   "N/A", "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("HDOP:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps.hdop.valueString : qsTr(
                                                   "--.--",
                                                   "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("VDOP:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps.vdop.valueString : qsTr(
                                                   "--.--",
                                                   "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("Course Over Ground:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps.courseOverGround.valueString : qsTr(
                                                   "--.--",
                                                   "No data to display")
                    }

                    QGCButton {
                        text: qsTr("Set as primary")
                        onClicked: {
                            if (_parameterManager) {
                                _parameterManager.sendParamSetToVehicle(
                                            1, "SENS_GPS_PRIME", 5, 0)

                                _sensGPSPrimeParameter = _parameterManager.getParameter(
                                            1, "SENS_GPS_PRIME")
                            }
                        }
                    }
                }
            }
        }
    }

    Component {
        id: gps_2Info

        Rectangle {
            width: gps_2Col.width + ScreenTools.defaultFontPixelWidth * 3
            height: gps_2Col.height + ScreenTools.defaultFontPixelHeight * 2
            radius: ScreenTools.defaultFontPixelHeight * 0.5
            color: qgcPal.window
            border.color: qgcPal.text

            Column {
                id: gps_2Col
                spacing: ScreenTools.defaultFontPixelHeight * 0.5
                width: Math.max(gps_2Grid.width, gps_2Label.width)
                anchors.margins: ScreenTools.defaultFontPixelHeight
                anchors.centerIn: parent

                QGCLabel {
                    id: gps_2Label
                    text: (_activeVehicle && _activeVehicle.gps2.count.value
                           >= 0) ? qsTr("GPS 2 Status") : qsTr(
                                       "GPS 2 Data Unavailable")
                    font.family: ScreenTools.demiboldFontFamily
                    anchors.horizontalCenter: parent.horizontalCenter
                }

                GridLayout {
                    id: gps_2Grid
                    visible: (_activeVehicle
                              && _activeVehicle.gps2.count.value >= 0)
                    anchors.margins: ScreenTools.defaultFontPixelHeight
                    columnSpacing: ScreenTools.defaultFontPixelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                    columns: 2

                    QGCLabel {
                        text: qsTr("GPS Count:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps2.count.valueString : qsTr(
                                                   "N/A", "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("GPS Lock:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps2.lock.enumStringValue : qsTr(
                                                   "N/A", "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("HDOP:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps2.hdop.valueString : qsTr(
                                                   "--.--",
                                                   "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("VDOP:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps2.vdop.valueString : qsTr(
                                                   "--.--",
                                                   "No data to display")
                    }
                    QGCLabel {
                        text: qsTr("Course Over Ground:")
                    }
                    QGCLabel {
                        text: _activeVehicle ? _activeVehicle.gps2.courseOverGround.valueString : qsTr(
                                                   "--.--",
                                                   "No data to display")
                    }

                    QGCButton {
                        text: qsTr("Set as primary")
                        onClicked: {
                            if (_parameterManager) {
                                _parameterManager.sendParamSetToVehicle(
                                            1, "SENS_GPS_PRIME", 5, 1)

                                _sensGPSPrimeParameter = _parameterManager.getParameter(
                                            1, "SENS_GPS_PRIME")
                            }
                        }
                    }
                }
            }
        }
    }

    QGCColoredImage {
        id: gps_1Icon
        width: height
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        source: "/qmlimages/Gps.svg"
        fillMode: Image.PreserveAspectFit
        sourceSize.height: height
        opacity: (_activeVehicle
                  && _activeVehicle.gps.count.value >= 0) ? 1 : 0.5
        color: qgcPal.buttonText

        MouseArea {
            anchors.fill: parent
            onClicked: {
                mainWindow.showIndicatorPopup(_root, gps_1Info)
            }
        }
    }

    QGCColoredImage {
        id: gps_2Icon
        width: height
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: gps_1ValuesColumn.right
        source: "/qmlimages/Gps.svg"
        fillMode: Image.PreserveAspectFit
        sourceSize.height: height
        opacity: (_activeVehicle
                  && _activeVehicle.gps.count.value >= 0) ? 1 : 0.5
        color: qgcPal.buttonText

        MouseArea {
            anchors.fill: parent
            onClicked: {
                mainWindow.showIndicatorPopup(_root, gps_2Info)
            }
        }
    }

    Column {
        id: gps_1ValuesColumn
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: ScreenTools.defaultFontPixelWidth / 2
        anchors.left: gps_1Icon.right

        QGCLabel {
            anchors.horizontalCenter: hdop_1Value.horizontalCenter
            visible: _activeVehicle && !isNaN(_activeVehicle.gps.hdop.value)
            color: qgcPal.buttonText
            text: _activeVehicle ? _activeVehicle.gps.count.valueString : ""
        }

        QGCLabel {
            id: hdop_1Value
            visible: _activeVehicle && !isNaN(_activeVehicle.gps.hdop.value)
            color: qgcPal.buttonText
            text: _activeVehicle ? _activeVehicle.gps.hdop.value.toFixed(1) : ""
        }
    }

    Column {
        id: gps_2ValuesColumn
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: ScreenTools.defaultFontPixelWidth / 2
        anchors.left: gps_2Icon.right

        QGCLabel {
            anchors.horizontalCenter: hdop_2Value.horizontalCenter
            visible: _activeVehicle && !isNaN(_activeVehicle.gps2.hdop.value)
            color: qgcPal.buttonText
            text: _activeVehicle ? _activeVehicle.gps2.count.valueString : ""
        }

        QGCLabel {
            id: hdop_2Value
            visible: _activeVehicle && !isNaN(_activeVehicle.gps2.hdop.value)
            color: qgcPal.buttonText
            text: _activeVehicle ? _activeVehicle.gps2.hdop.value.toFixed(
                                       1) : ""
        }
    }

    QGCLabel {
        id: _primaryParemeterGPSLabel
        text: qsTr("PRIMARY:")
        anchors.left: gps_2ValuesColumn.right
    }
    QGCLabel {
        id: _primaryParemeterGPSValue
        anchors.left: _primaryParemeterGPSLabel.right
        text: (_sensGPSPrimeParameter) ? _sensGPSPrimeParameter.valueString : qsTr(
                                             "")
    }

    on_ParametersReadyChanged: {
        if (_parametersReady === true) {

            _sensGPSPrimeParameter = _parameterManager.getParameter(
                        1, "SENS_GPS_PRIME")
        }
    }
}
