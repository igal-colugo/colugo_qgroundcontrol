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

    property int adtRowHeight: (parent.height
                                - (adtGrid.rows * adtGrid.rowSpacing)) / (adtGrid.rows)
    property int adtColumnWidth: (parent.width - (adtGrid.columns * adtGrid.columnSpacing))
                                 / (adtGrid.columns)

    QGCFlickable {
        clip: true
        anchors.fill: parent
        anchors.margins: ScreenTools.defaultFontPixelWidth
        flickableDirection: Flickable.VerticalFlick

        QGCGroupBox {
            anchors.fill: parent
            title: qsTr("ADT SETTINGS")

            GridLayout {
                id: adtGrid

                columns: 6
                rows: 7
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2

                onWidthChanged: {
                    console.log("Commtact adt:", adtGrid.width,
                                adtGrid.height)
                }

                QGCLabel {

                    height: ScreenTools.defaultFontPixelHeight

                    text: qsTr("MODE")
                    font.family: ScreenTools.demiboldFontFamily
                    color: "White"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                }
                QGCLabel {
                    height: ScreenTools.defaultFontPixelHeight

                    text: QGroundControl.commtactLinkManagement.transmitterOperationalMode
                    font.family: ScreenTools.demiboldFontFamily
                    color: "White"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                }
                QGCButton {

                    id: _adtTxOffButton

                    showBorder: true
                    text: qsTr("TX OFF")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: adtRowHeight
                    Layout.preferredWidth: adtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                    0)
                    }
                }
                QGCLabel {

                    id: _adtSetCameraScreenInformationLabel

                    height: ScreenTools.defaultFontPixelHeight

                    text: qsTr("INFO")
                    color: "White"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: adtRowHeight
                    Layout.preferredWidth: adtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft
                }
                QGCTextField {

                    id: _adtSetCameraScreenInformationField

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: adtRowHeight
                    Layout.preferredWidth: adtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft

                    maximumLength: 4
                    font.pointSize: ScreenTools.isMobile ? point_size : 9
                    text: qsTr("2047")
                }
                QGCButton {

                    id: _adtSetCameraScreenInformationButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: adtRowHeight
                    Layout.preferredWidth: adtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft

                    onReleased: {
                        if (parseInt(_adtSetCameraScreenInformationField.text) >= 0
                                && parseInt(
                                    _adtSetCameraScreenInformationField.text) <= 2047) {
                            joystickManager.epsilonCameraManagement.setCameraScreenInformationCommand(
                                        parseInt(
                                            _adtSetCameraScreenInformationField.text))
                        }
                    }
                }
            }
        }
    }
}
