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

Item {
    id: ethernetSettingsMode
    anchors.fill: parent
    visible: true

    GridLayout {
        id: gdtMainGrid

        columns: 2
        rows: 1
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        QGCGroupBox {
            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                        / (parent.rows)
            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing))
                                          / (parent.columns)
            id: gdtEthernetSettingsGroupBox
            height: cellRowHeight * Layout.rowSpan
            width: cellColumnWidth * Layout.columnSpan
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            title: qsTr("GDT")
            label: Text {
                color: "#808080"
                text: gdtEthernetSettingsGroupBox.title
            }

            GridLayout {
                id: gdtEthernetSettingsGrid

                columns: 1
                rows: 2
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2

                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)
                    id: gdtEthernetSetGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan
                    Layout.rowSpan: 1
                    Layout.columnSpan: 1
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("SETTINGS")
                    label: Text {
                        color: "#808080"
                        text: gdtEthernetSetGroupBox.title
                    }

                    GridLayout {
                        id: gdtEthernetSetGrid

                        columns: 12
                        rows: 5
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetIPPortLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("ICD IP(x.x.x.x:port):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetIPPortField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(?:\:?)(?:[0-9][0-9][0-9][0-9][0-9]?)$/
                            }
                            text: qsTr("127.0.0.1:3000")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetIPPortButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDIPAddressCommand(
                                            _setEthernetIPPortField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setSubnetMaskLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("SUBNET(x.x.x.x):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setSubnetMaskField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
                            }
                            text: qsTr("255.255.255.0")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setSubnetMaskButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDSubnetMaskCommand(
                                            _setSubnetMaskField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setDefaultGatewayLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("DEFAULT GATEWAY(x.x.x.x):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setDefaultGatewayField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
                            }
                            text: qsTr("127.0.0.1")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setDefaultGatewayButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDDefaultGatewayCommand(
                                            _setDefaultGatewayField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetHostIPPortLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("HOST IP(x.x.x.x:port):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetHostIPPortField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(?:\:?)(?:[0-9][0-9][0-9][0-9][0-9]?)$/
                            }
                            text: qsTr("127.0.0.1:3001")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetHostIPPortButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDHostIPCommand(
                                            _setEthernetHostIPPortField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetDiscoveryPortLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("DISCOVERY PORT:")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetDiscoveryPortField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:[0-9][0-9][0-9][0-9][0-9]?)$/
                            }
                            text: qsTr("56000")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetDiscoveryPortButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDDiscoveryPortCommand(
                                            parseInt(
                                                _setEthernetDiscoveryPortField.text))
                            }
                        }
                    }
                }
                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)
                    id: gdtEthernetGetGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan
                    Layout.rowSpan: 1
                    Layout.columnSpan: 1
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("REPORT")
                    label: Text {
                        color: "#808080"
                        text: gdtEthernetGetGroupBox.title
                    }

                    GridLayout {
                        id: gdtEthernetGetGrid

                        columns: 12
                        rows: 5
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD IP:")
                                        + QGroundControl.commtactLinkManagement.commonICDIPAddress
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD PORT:")
                                        + QGroundControl.commtactLinkManagement.commonICDPort
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD SUBNET:")
                                        + QGroundControl.commtactLinkManagement.commonICDSubnetMask
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD GATEWAY:") + QGroundControl.commtactLinkManagement.commonICDDefaultGateway
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("HOST IP:") + QGroundControl.commtactLinkManagement.commonICDHostIPAddress
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD HOST PORT:")
                                        + QGroundControl.commtactLinkManagement.commonICDHostPort
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("DISCOVERY PORT:") + QGroundControl.commtactLinkManagement.commonICDDiscoveryPort
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                    }
                }
            }
        }

        QGCGroupBox {
            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                        / (parent.rows)
            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing))
                                          / (parent.columns)
            id: adtEthernetSettingsGroupBox
            height: cellRowHeight * Layout.rowSpan
            width: cellColumnWidth * Layout.columnSpan
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            title: qsTr("ADT")
            label: Text {
                color: "#808080"
                text: adtEthernetSettingsGroupBox.title
            }

            GridLayout {
                id: adtEthernetSettingsGrid

                columns: 1
                rows: 2
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2

                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)
                    id: adtEthernetSetGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan
                    Layout.rowSpan: 1
                    Layout.columnSpan: 1
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("SETTINGS")
                    label: Text {
                        color: "#808080"
                        text: adtEthernetSetGroupBox.title
                    }

                    GridLayout {
                        id: adtEthernetSetGrid

                        columns: 12
                        rows: 5
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetADTIPPortLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("ICD IP(x.x.x.x:port):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetADTIPPortField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(?:\:?)(?:[0-9][0-9][0-9][0-9][0-9]?)$/
                            }
                            text: qsTr("127.0.0.1:52000")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setEthernetADTIPPortButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDIPAddressCommand(
                                            _setEthernetADTIPPortField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTSubnetMaskLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("SUBNET(x.x.x.x):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTSubnetMaskField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
                            }
                            text: qsTr("255.255.255.0")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTSubnetMaskButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDSubnetMaskCommand(
                                            _setADTSubnetMaskField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTDefaultGatewayLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("DEFAULT GATEWAY(x.x.x.x):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTDefaultGatewayField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/
                            }
                            text: qsTr("127.0.0.1")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTDefaultGatewayButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDDefaultGatewayCommand(
                                            _setADTDefaultGatewayField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTEthernetHostIPPortLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("HOST IP(x.x.x.x:port):")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTEthernetHostIPPortField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(?:\:?)(?:[0-9][0-9][0-9][0-9][0-9]?)$/
                            }
                            text: qsTr("127.0.0.1:52001")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTEthernetHostIPPortButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDHostIPCommand(
                                            _setADTEthernetHostIPPortField.text)
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTEthernetDiscoveryPortLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("DISCOVERY PORT:")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTEthernetDiscoveryPortField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            //maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            validator: RegExpValidator {
                                regExp: /^(?:[0-9][0-9][0-9][0-9][0-9]?)$/
                            }
                            text: qsTr("57000")
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setADTEthernetDiscoveryPortButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                QGroundControl.commtactLinkManagement.setCommonEthernetICDDiscoveryPortCommand(
                                            parseInt(
                                                _setADTEthernetDiscoveryPortField.text))
                            }
                        }
                    }
                }
                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)
                    id: adtEthernetGetGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan
                    Layout.rowSpan: 1
                    Layout.columnSpan: 1
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("REPORT")
                    label: Text {
                        color: "#808080"
                        text: adtEthernetGetGroupBox.title
                    }

                    GridLayout {
                        id: adtEthernetGetGrid

                        columns: 12
                        rows: 5
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD IP:") + QGroundControl.commtactLinkManagement.commonICDADTIPAddress
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD PORT:")
                                        + QGroundControl.commtactLinkManagement.commonICDADTPort
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD SUBNET:") + QGroundControl.commtactLinkManagement.commonICDADTSubnetMask
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD GATEWAY:") + QGroundControl.commtactLinkManagement.commonICDADTDefaultGateway
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("HOST IP:") + QGroundControl.commtactLinkManagement.commonICDADTHostIPAddress
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("ICD HOST PORT:")
                                        + QGroundControl.commtactLinkManagement.commonICDADTHostPort
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                qsTr("DISCOVERY PORT:") + QGroundControl.commtactLinkManagement.commonICDADTDiscoveryPort
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                    }
                }
            }
        }
    }
}
