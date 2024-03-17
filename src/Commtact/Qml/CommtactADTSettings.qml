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
    id: adtSettingsMode
    anchors.fill: parent
    visible: true

    GridLayout {
        id: adtMainGrid

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

            id: adtSettingsGroupBox
            height: cellRowHeight
            width: cellColumnWidth * Layout.columnSpan
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            title: qsTr("ADT SETTINGS")
            label: Text {
                color: "#808080"
                text: adtSettingsGroupBox.title
            }

            GridLayout {
                id: adtSettingsGrid

                columns: 12
                rows: 10
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2

                QGCLabel {

                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "MODE:"
                    font.family: ScreenTools.demiboldFontFamily

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                }
                QGCComboBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtOperationalMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.adtOperationalModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtSetModeButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 1

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        if (_adtOperationalMode.currentIndex === 0) {
                            setModeDialog.visible = true
                        } else if (_adtOperationalMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setADTOperationalModeCommand(
                                        2)
                        } else if (_adtOperationalMode.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setADTOperationalModeCommand(
                                        1)
                        } else if (_adtOperationalMode.currentIndex === 3) {
                            QGroundControl.commtactLinkManagement.setADTOperationalModeCommand(
                                        3)
                        } else if (_adtOperationalMode.currentIndex === 4) {
                            QGroundControl.commtactLinkManagement.setADTOperationalModeCommand(
                                        124)
                        } else if (_adtOperationalMode.currentIndex === 5) {
                            QGroundControl.commtactLinkManagement.setADTOperationalModeCommand(
                                        127)
                        } else if (_adtOperationalMode.currentIndex === 6) {
                            QGroundControl.commtactLinkManagement.setADTOperationalModeCommand(
                                        130)
                        }
                    }

                    MessageDialog {
                        id: setModeDialog
                        visible: false
                        icon: StandardIcon.Warning
                        standardButtons: StandardButton.Yes | StandardButton.No
                        title: qsTr("Set ADT turn off")
                        text: qsTr("Turn off ADT.Is this really what you want?")

                        onYes: {
                            QGroundControl.commtactLinkManagement.setADTOperationalModeCommand(
                                        0)
                            deleteDialog.visible = false
                        }
                        onNo: deleteDialog.visible = false
                    }
                }

                QGCLabel {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "ANTENNA:"
                    font.family: ScreenTools.demiboldFontFamily

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                }
                QGCComboBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtAntennaSelect
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.adtAntennaSelectTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtSetAntennaSelectButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 1

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        if (_gdtAntennaSelect.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        0)
                        } else if (_gdtAntennaSelect.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        1)
                        } else if (_gdtAntennaSelect.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        2)
                        } else if (_gdtAntennaSelect.currentIndex === 3) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        5)
                        } else if (_gdtAntennaSelect.currentIndex === 4) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        6)
                        } else if (_gdtAntennaSelect.currentIndex === 5) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        7)
                        } else if (_gdtAntennaSelect.currentIndex === 6) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        8)
                        } else if (_gdtAntennaSelect.currentIndex === 7) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        10)
                        } else if (_gdtAntennaSelect.currentIndex === 8) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        11)
                        } else if (_gdtAntennaSelect.currentIndex === 9) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        12)
                        } else if (_gdtAntennaSelect.currentIndex === 10) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        13)
                        } else if (_gdtAntennaSelect.currentIndex === 11) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        14)
                        } else if (_gdtAntennaSelect.currentIndex === 12) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        15)
                        } else if (_gdtAntennaSelect.currentIndex === 13) {
                            QGroundControl.commtactLinkManagement.setADTAntennaSelectCommand(
                                        22)
                        }
                    }
                }

                QGCLabel {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "SYMB RATE:"
                    font.family: ScreenTools.demiboldFontFamily

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                }
                QGCComboBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtSymbolRate
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.adtSymbolRateTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtSetSymbolRateButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 1

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        if (_adtSymbolRate.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setADTSymbolRateCommand(
                                        0)
                        } else if (_adtSymbolRate.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setADTSymbolRateCommand(
                                        1)
                        } else if (_adtSymbolRate.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setADTSymbolRateCommand(
                                        2)
                        }
                    }
                }

                QGCLabel {

                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "TDD:"
                    font.family: ScreenTools.demiboldFontFamily

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                }
                QGCComboBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtTddOperationalMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.adtTddOperationalModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtSetTddOperationalModeButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 1

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        if (_adtTddOperationalMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setADTTddOperationalModeCommand(
                                        0)
                        } else if (_adtTddOperationalMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setADTTddOperationalModeCommand(
                                        1)
                        }
                    }
                }

                QGCLabel {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "FREQ:"
                    font.family: ScreenTools.demiboldFontFamily

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                }
                QGCComboBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtFrequencyMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.adtFrequencyModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {

                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtSetFrequencyModeButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 1

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        if (_adtFrequencyMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setADTFrequencyModeCommand(
                                        0)
                        } else if (_adtFrequencyMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setADTFrequencyModeCommand(
                                        1)
                        }
                    }
                }

                QGCLabel {

                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "UNIT:"
                    font.family: ScreenTools.demiboldFontFamily

                    horizontalAlignment: Text.AlignLeft
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                }
                QGCComboBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtUnitMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.adtUnitModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _adtSetUnitModeButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 1

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        if (_adtUnitMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setADTUnitModeCommand(
                                        0)
                        } else if (_adtUnitMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setADTUnitModeCommand(
                                        6)
                        } else if (_adtUnitMode.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setADTUnitModeCommand(
                                        11)
                        }
                    }
                }

                QGCCheckBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _videoTransmitEnableCheckBox
                    text: qsTr("VIDEO ENABLE")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    checked: (QGroundControl.commtactLinkManagement.adtVideoTransmitEnable
                              === 1) ? true : false

                    onClicked: {
                        if (_videoTransmitEnableCheckBox.checked) {
                            QGroundControl.commtactLinkManagement.setADTVideoTransmitEnableCommand(
                                        1)
                        } else {
                            QGroundControl.commtactLinkManagement.setADTVideoTransmitEnableCommand(
                                        0)
                        }
                    }
                }
                QGCCheckBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _enableAesEncryption
                    text: qsTr("AES ENCRYPTION")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    checked: (QGroundControl.commtactLinkManagement.adtAesEncryption
                              === 1) ? true : false

                    onClicked: {
                        if (_enableAesEncryption.checked) {
                            QGroundControl.commtactLinkManagement.setADTAesEncryptionCommand(
                                        1)
                        } else {
                            QGroundControl.commtactLinkManagement.setADTAesEncryptionCommand(
                                        0)
                        }
                    }
                }
                QGCCheckBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _enableTelemetrySeparation
                    text: qsTr("TELEM SEPARATION")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    checked: (QGroundControl.commtactLinkManagement.adtTelemetryMetadataSeparation
                              === 1) ? true : false

                    onClicked: {
                        if (_enableTelemetrySeparation.checked) {
                            QGroundControl.commtactLinkManagement.setADTTelemetryMetadataSeparationCommand(
                                        1)
                        } else {
                            QGroundControl.commtactLinkManagement.setADTTelemetryMetadataSeparationCommand(
                                        0)
                        }
                    }
                }

                QGCLabel {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _setFrequencyLabel

                    height: ScreenTools.defaultFontPixelHeight

                    text: qsTr("FREQUENCY:")

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft
                }
                QGCTextField {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _setFrequencyField

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft

                    maximumLength: 4
                    font.pointSize: ScreenTools.isMobile ? point_size : 9
                    text: qsTr("2200")
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _setFrequencyButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                    Layout.fillWidth: false
                    Layout.fillHeight: true
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        if (parseInt(_setFrequencyField.text) >= 2150
                                && parseInt(_setFrequencyField.text) <= 2400) {
                            QGroundControl.commtactLinkManagement.setADTOperationalFrequencyCommand(
                                        parseInt(_setFrequencyField.text))
                        }
                    }
                }

                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: adtSetVideoGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan
                    Layout.rowSpan: 5
                    Layout.columnSpan: 12
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("VIDEO SETTINGS")
                    label: Text {
                        color: "#808080"
                        text: adtSetVideoGroupBox.title
                    }

                    GridLayout {
                        id: adtVideoGrid

                        columns: 12
                        rows: 3
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setVideoRateLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("VIDEO RATE:")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCComboBox {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _adtVideoRateComboBox
                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignCenter

                            model: QGroundControl.commtactLinkManagement.adtVideoRateTypeStrings
                            currentIndex: 0
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _adtSetVideoRate

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
                                if (_adtVideoRateComboBox.currentIndex === 0) {
                                    QGroundControl.commtactLinkManagement.setADTVideoRateCommand(
                                                0)
                                } else if (_adtVideoRateComboBox.currentIndex === 1) {
                                    QGroundControl.commtactLinkManagement.setADTVideoRateCommand(
                                                1)
                                } else if (_adtVideoRateComboBox.currentIndex === 2) {
                                    QGroundControl.commtactLinkManagement.setADTVideoRateCommand(
                                                2)
                                }
                            }
                        }

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setVideoSourceLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("VIDEO SOURCE:")

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft
                        }
                        QGCComboBox {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _adtVideoSourceComboBox
                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                            Layout.fillWidth: false
                            Layout.fillHeight: false
                            Layout.alignment: Qt.AlignCenter

                            model: QGroundControl.commtactLinkManagement.adtVideoSourceTypeStrings
                            currentIndex: 0
                        }
                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _adtSetVideoSource

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
                                if (_adtVideoSourceComboBox.currentIndex === 0) {
                                    QGroundControl.commtactLinkManagement.setADTVideoSourceCommand(
                                                0)
                                } else if (_adtVideoSourceComboBox.currentIndex === 1) {
                                    QGroundControl.commtactLinkManagement.setADTVideoSourceCommand(
                                                1)
                                }
                            }
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
            id: adtReportGroupBox
            height: cellRowHeight
            width: cellColumnWidth * Layout.columnSpan
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignCenter
            title: qsTr("ADT REPORT:") + qsTr(
                       "FW:") + QGroundControl.commtactLinkManagement.commonICDADTFWVersion + qsTr(
                       " SW0:")
                   + QGroundControl.commtactLinkManagement.commonICDADTSW0Version + qsTr(
                       " SW1:") + QGroundControl.commtactLinkManagement.commonICDADTSW1Version
            label: Text {
                color: "#808080"
                text: adtReportGroupBox.title
            }

            GridLayout {
                id: adtReportGrid

                columns: 12
                rows: 9
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2

                QGCGroupBox {

                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: adtOperationalModesReportGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan

                    Layout.rowSpan: 3
                    Layout.columnSpan: 12
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("OPERATIONAL MODES REPORT")
                    label: Text {
                        color: "#808080"
                        text: adtOperationalModesReportGroupBox.title
                    }

                    GridLayout {
                        id: adtOperationalModesReportGrid

                        columns: 12
                        rows: 4
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 0) {
                                    qsTr("MODE:TX OFF")
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 1) {
                                    qsTr("MODE:TX HIGH")
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 2) {
                                    qsTr("MODE:TX LOW")
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 3) {
                                    qsTr("MODE:IBIT")
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 124) {
                                    qsTr("MODE:24dBm")
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 127) {
                                    qsTr("MODE:27dBm")
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 130) {
                                    qsTr("MODE:30dBm")
                                } else {
                                    qsTr("MODE:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: {
                                if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 0) {
                                    "Red"
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 1) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 2) {
                                    "Yellow"
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 3) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 124) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 127) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.adtTransmitterOperationalMode === 130) {
                                    "Green"
                                } else {
                                    "White"
                                }
                            }

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtAntennaSelect === 0) {
                                    qsTr("ANTENNA:ANTENNA1")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 1) {
                                    qsTr("ANTENNA:ANTENNA2")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 3) {
                                    qsTr("ANTENNA:AUTO ANTENNA1")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 4) {
                                    qsTr("ANTENNA:AUTO ANTENNA2")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 5) {
                                    qsTr("ANTENNA:ANTENNA 2 SP4T-1")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 6) {
                                    qsTr("ANTENNA:ANTENNA 2 SP4T-2")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 7) {
                                    qsTr("ANTENNA:ANTENNA 2 SP4T-3")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 8) {
                                    qsTr("ANTENNA:ANTENNA 2 SP4T-4")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 10) {
                                    qsTr("ANTENNA:ANTENNA 6P NO.1")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 11) {
                                    qsTr("ANTENNA:ANTENNA 6P NO.2")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 12) {
                                    qsTr("ANTENNA:ANTENNA 6P NO.3")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 13) {
                                    qsTr("ANTENNA:ANTENNA 6P NO.4")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 14) {
                                    qsTr("ANTENNA:ANTENNA 6P NO.5")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 15) {
                                    qsTr("ANTENNA:ANTENNA 6P NO.6")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 16) {
                                    qsTr("ANTENNA:AUTO ANTENNA 6P NO.1")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 17) {
                                    qsTr("ANTENNA:AUTO ANTENNA 6P NO.2")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 18) {
                                    qsTr("ANTENNA:AUTO ANTENNA 6P NO.3")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 19) {
                                    qsTr("ANTENNA:AUTO ANTENNA 6P NO.4")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 20) {
                                    qsTr("ANTENNA:AUTO ANTENNA 6P NO.5")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 21) {
                                    qsTr("ANTENNA:AUTO ANTENNA 6P NO.6")
                                } else if (QGroundControl.commtactLinkManagement.adtAntennaSelect
                                           === 22) {
                                    qsTr("ANTENNA:ADVANCED")
                                } else {
                                    qsTr("ANTENNA:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr(" ")
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true
                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtTddOperationalMode
                                        === 0) {
                                    qsTr("TDD:SLAVE")
                                } else if (QGroundControl.commtactLinkManagement.adtTddOperationalMode === 1) {
                                    qsTr("TDD:MASTER")
                                } else {
                                    qsTr("TDD:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtFrequencyMode === 0) {
                                    qsTr("FREQ:FIX")
                                } else if (QGroundControl.commtactLinkManagement.adtFrequencyMode
                                           === 1) {
                                    qsTr("FREQ:HOPPING")
                                } else {
                                    qsTr("FREQ:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtUnitMode === 0) {
                                    qsTr("UNIT:NORMAL")
                                } else if (QGroundControl.commtactLinkManagement.adtUnitMode
                                           === 3) {
                                    qsTr("UNIT:MISSION RELAY")
                                } else if (QGroundControl.commtactLinkManagement.adtUnitMode
                                           === 4) {
                                    qsTr("UNIT:RELAY")
                                } else if (QGroundControl.commtactLinkManagement.adtUnitMode
                                           === 5) {
                                    qsTr("UNIT:MISSION")
                                } else if (QGroundControl.commtactLinkManagement.adtUnitMode
                                           === 6) {
                                    qsTr("UNIT:TRANSIT")
                                } else if (QGroundControl.commtactLinkManagement.adtUnitMode
                                           === 11) {
                                    qsTr("UNIT:NORMAL TMO")
                                } else if (QGroundControl.commtactLinkManagement.adtUnitMode
                                           === 51) {
                                    qsTr("UNIT:MISSION TMO")
                                } else {
                                    qsTr("UNIT:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtSymbolRate === 0) {
                                    qsTr("SYMBOL RATE:16MSPS")
                                } else if (QGroundControl.commtactLinkManagement.adtSymbolRate
                                           === 1) {
                                    qsTr("SYMBOL RATE:8MSPS")
                                } else if (QGroundControl.commtactLinkManagement.adtSymbolRate
                                           === 2) {
                                    qsTr("SYMBOL RATE:4MSPS")
                                } else {
                                    qsTr("SYMBOL:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtAesEncryption === 0) {
                                    qsTr("AES ENC:DISABLED")
                                } else if (QGroundControl.commtactLinkManagement.adtAesEncryption
                                           === 1) {
                                    qsTr("AES ENC:ENABLED")
                                } else {
                                    qsTr("AES ENC:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtVideoTransmitEnable
                                        === 0) {
                                    qsTr("VIDEO:DISABLED")
                                } else if (QGroundControl.commtactLinkManagement.adtVideoTransmitEnable === 1) {
                                    qsTr("VIDEO:ENABLED")
                                } else {
                                    qsTr("SYMBOL:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtVideoRate === 0) {
                                    qsTr("VIDEO RATE:1.8Mbps")
                                } else if (QGroundControl.commtactLinkManagement.adtVideoRate
                                           === 1) {
                                    qsTr("VIDEO RATE:1.2Mbps")
                                } else if (QGroundControl.commtactLinkManagement.adtVideoRate
                                           === 2) {
                                    qsTr("VIDEO RATE:900kbps")
                                } else {
                                    qsTr("VIDEO RATE:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtVideoSource === 0) {
                                    qsTr("VIDEO SOURCE:ANALOG")
                                } else if (QGroundControl.commtactLinkManagement.adtVideoSource
                                           === 1) {
                                    qsTr("VIDEO SOURCE:DIGITAL")
                                } else {
                                    qsTr("VIDEO SOURCE:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: {
                                if (QGroundControl.commtactLinkManagement.adtTelemetryMetadataSeparation === 0) {
                                    qsTr("TELEM SEP:DISABLED")
                                } else if (QGroundControl.commtactLinkManagement.adtTelemetryMetadataSeparation === 1) {
                                    qsTr("TELEM SEP:ENABLED")
                                } else {
                                    qsTr("TELEM SEP:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                    }
                }

                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: adtStatusReportGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan
                    Layout.rowSpan: 3
                    Layout.columnSpan: 12
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("STATUS REPORT")
                    label: Text {
                        color: "#808080"
                        text: adtStatusReportGroupBox.title
                    }

                    GridLayout {
                        id: adtStatusReportGrid

                        columns: 12
                        rows: 7
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("LINK RSSI:")
                                  + QGroundControl.commtactLinkManagement.adtLinkRSSI
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("FREQUENCY:")
                                  + QGroundControl.commtactLinkManagement.adtOperationFrequency
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("TDD SYNCED:")
                                  + QGroundControl.commtactLinkManagement.adtTDDSync
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("TRANSFERED:")
                                  + QGroundControl.commtactLinkManagement.adtLinkTransferedPackets
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("ERROR:")
                                  + QGroundControl.commtactLinkManagement.adtLinkErrorPackets
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("CRC ERROR:")
                                  + QGroundControl.commtactLinkManagement.adtLinkCRCErrorPackets
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                    }
                }

                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: gdtCBITReportGroupBox
                    height: cellRowHeight * Layout.rowSpan
                    width: cellColumnWidth * Layout.columnSpan
                    Layout.rowSpan: 3
                    Layout.columnSpan: 12
                    Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter
                    title: qsTr("CBIT REPORT")
                    label: Text {
                        color: "#808080"
                        text: gdtCBITReportGroupBox.title
                    }

                    GridLayout {
                        id: gdtCBITReportGrid

                        columns: 12
                        rows: 3
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("PA POWER OUTPUT:")
                                  + QGroundControl.commtactLinkManagement.gdtCBITPAPowerOutput
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        QGCLabel {

                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("PA RETURN POWER:")
                                  + QGroundControl.commtactLinkManagement.gdtCBITPAReturnPower
                            font.family: ScreenTools.demiboldFontFamily

                            horizontalAlignment: Text.AlignLeft
                            verticalAlignment: Text.AlignVCenter

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: true

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                        }
                        Rectangle {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: dataRecievedIndicator
                            radius: width / 2
                            color: {
                                if (QGroundControl.commtactLinkManagement.adtDataRecieved === 1) {
                                    "Green"
                                } else {
                                    "Black"
                                }
                            }

                            border.color: qgcPal.text
                            border.width: 1

                            height: cellRowHeight * Layout.rowSpan

                            Layout.alignment: Qt.AlignLeft
                            Layout.columnSpan: 4
                            Layout.fillHeight: false
                            Layout.fillWidth: false
                            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
                            Layout.preferredWidth: Layout.preferredHeight
                            Layout.maximumHeight: cellRowHeight * Layout.rowSpan
                            Layout.maximumWidth: Layout.maximumHeight
                        }
                    }
                }
            }
        }
    }
}
