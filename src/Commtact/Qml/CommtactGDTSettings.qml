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
    id: gdtSettingsMode
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

            id: gdtSettingsGroupBox
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
            title: qsTr("GDT SETTINGS")
            label: Text {
                color: "#ffe4c4"
                text: gdtSettingsGroupBox.title
            }


            GridLayout {
                id: gdtSettingsGrid

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
                    color: "White"

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

                    id: _gdtOperationalMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.operationalModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtSetModeButton

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
                        if (_gdtOperationalMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                        0)
                        } else if (_gdtOperationalMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                        2)
                        } else if (_gdtOperationalMode.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                        1)
                        } else if (_gdtOperationalMode.currentIndex === 3) {
                            QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                        3)
                        } else if (_gdtOperationalMode.currentIndex === 4) {
                            QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                        124)
                        } else if (_gdtOperationalMode.currentIndex === 5) {
                            QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                        127)
                        } else if (_gdtOperationalMode.currentIndex === 6) {
                            QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                        130)
                        }
                    }
                }

                QGCLabel {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "ANTENNA:"
                    font.family: ScreenTools.demiboldFontFamily
                    color: "White"

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

                    model: QGroundControl.commtactLinkManagement.gdtAntennaSelectTypeStrings
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
                            QGroundControl.commtactLinkManagement.setGDTAntennaSelectCommand(
                                        0)
                        } else if (_gdtAntennaSelect.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setGDTAntennaSelectCommand(
                                        1)
                        } else if (_gdtAntennaSelect.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setGDTAntennaSelectCommand(
                                        22)
                        }
                    }
                }

                QGCLabel {

                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: "PEDESTAL:"
                    font.family: ScreenTools.demiboldFontFamily
                    color: "White"

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

                    id: _gdtPedestalTrackMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.gdtPedestalTrackModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {

                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtSetPedestalTrackModeButton

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
                        if (_gdtPedestalTrackMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setGDTPedestalTrackModeCommand(
                                        0)
                        } else if (_gdtPedestalTrackMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setGDTPedestalTrackModeCommand(
                                        2)
                        } else if (_gdtPedestalTrackMode.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setGDTPedestalTrackModeCommand(
                                        3)
                        } else if (_gdtPedestalTrackMode.currentIndex === 3) {
                            QGroundControl.commtactLinkManagement.setGDTPedestalTrackModeCommand(
                                        4)
                        } else if (_gdtPedestalTrackMode.currentIndex === 4) {
                            QGroundControl.commtactLinkManagement.setGDTPedestalTrackModeCommand(
                                        5)
                        } else if (_gdtPedestalTrackMode.currentIndex === 5) {
                            QGroundControl.commtactLinkManagement.setGDTPedestalTrackModeCommand(
                                        7)
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
                    color: "White"

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

                    id: _gdtTddOperationalMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.gdtTddOperationalModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtSetTddOperationalModeButton

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
                        if (_gdtTddOperationalMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setGDTTddOperationalModeCommand(
                                        0)
                        } else if (_gdtTddOperationalMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setGDTTddOperationalModeCommand(
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
                    color: "White"

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

                    id: _gdtFrequencyMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.gdtFrequencyModeTypeStrings
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
                        if (_gdtFrequencyMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setGDTFrequencyModeCommand(
                                        0)
                        } else if (_gdtFrequencyMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setGDTFrequencyModeCommand(
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
                    color: "White"

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

                    id: _gdtUnitMode
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.gdtUnitModeTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtSetUnitModeButton

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
                        if (_gdtUnitMode.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setGDTUnitModeCommand(
                                        0)
                        } else if (_gdtUnitMode.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setGDTUnitModeCommand(
                                        3)
                        } else if (_gdtUnitMode.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setGDTUnitModeCommand(
                                        4)
                        } else if (_gdtUnitMode.currentIndex === 3) {
                            QGroundControl.commtactLinkManagement.setGDTUnitModeCommand(
                                        5)
                        } else if (_gdtUnitMode.currentIndex === 4) {
                            QGroundControl.commtactLinkManagement.setGDTUnitModeCommand(
                                        6)
                        } else if (_gdtUnitMode.currentIndex === 5) {
                            QGroundControl.commtactLinkManagement.setGDTUnitModeCommand(
                                        51)
                        } else if (_gdtUnitMode.currentIndex === 6) {
                            QGroundControl.commtactLinkManagement.setGDTUnitModeCommand(
                                        56)
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
                    color: "White"

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

                    id: _gdtSymbolRate
                    Layout.rowSpan: 1
                    Layout.columnSpan: 3

                    Layout.preferredHeight: cellRowHeight
                    Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                    Layout.maximumHeight: cellRowHeight
                    Layout.maximumWidth: cellColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignCenter

                    model: QGroundControl.commtactLinkManagement.gdtSymbolRateTypeStrings
                    currentIndex: 0
                }
                QGCButton {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: _gdtSetSymbolRateButton

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
                        if (_gdtSymbolRate.currentIndex === 0) {
                            QGroundControl.commtactLinkManagement.setGDTSymbolRateCommand(
                                        0)
                        } else if (_gdtSymbolRate.currentIndex === 1) {
                            QGroundControl.commtactLinkManagement.setGDTSymbolRateCommand(
                                        1)
                        } else if (_gdtSymbolRate.currentIndex === 2) {
                            QGroundControl.commtactLinkManagement.setGDTSymbolRateCommand(
                                        2)
                        }
                    }
                }

                QGCLabel {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    height: ScreenTools.defaultFontPixelHeight

                    text: " "
                    font.family: ScreenTools.demiboldFontFamily
                    color: "White"

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

                    checked: (QGroundControl.commtactLinkManagement.gdtAesEncryption
                              === 1) ? true : false

                    onClicked: {
                        if (_enableAesEncryption.checked) {
                            QGroundControl.commtactLinkManagement.setGDTAesEncryptionCommand(
                                        1)
                        } else {
                            QGroundControl.commtactLinkManagement.setGDTAesEncryptionCommand(
                                        0)
                        }
                    }
                }

                QGCGroupBox {
                    property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                                / (parent.rows)
                    property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                    id: gdtSetFrequencyGroupBox
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
                    title: qsTr("SET FREQUENCY")
                    label: Text {
                        color: "#ffe4c4"
                        text: gdtSetFrequencyGroupBox.title
                    }

                    GridLayout {
                        id: gdtFrequencyGrid

                        columns: 12
                        rows: 3
                        anchors.fill: parent
                        anchors.margins: 3
                        columnSpacing: 2
                        rowSpacing: 2

                        QGCLabel {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setFrequencyLabel

                            height: ScreenTools.defaultFontPixelHeight

                            text: qsTr("FREQUENCY:")
                            color: "White"

                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft
                        }

                        QGCTextField {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setFrequencyField

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft

                            maximumLength: 4
                            font.pointSize: ScreenTools.isMobile ? point_size : 9
                            text: qsTr("2200")
                        }

                        QGCButton {
                            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing)) / (parent.rows)
                            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing)) / (parent.columns)

                            id: _setFrequencyButton

                            showBorder: true
                            text: qsTr("SET")

                            Layout.rowSpan: 1
                            Layout.columnSpan: 4

                            Layout.preferredHeight: cellRowHeight
                            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
                            Layout.maximumHeight: cellRowHeight
                            Layout.maximumWidth: cellColumnWidth * Layout.columnSpan

                            Layout.fillWidth: false
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignLeft

                            onClicked: {
                                if (parseInt(_setFrequencyField.text) >= 2150
                                        && parseInt(
                                            _setFrequencyField.text) <= 2400) {
                                    QGroundControl.commtactLinkManagement.setGDTOperationalFrequencyCommand(
                                                parseInt(
                                                    _setFrequencyField.text))
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
            id: gdtReportGroupBox
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
            title: qsTr("GDT REPORT")
            label: Text {
                color: "#ffe4c4"
                text: gdtReportGroupBox.title
            }

            GridLayout {
                id: gdtReportGrid

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

                    id: gdtOperationalModesReportGroupBox
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
                        color: "#ffe4c4"
                        text: gdtOperationalModesReportGroupBox.title
                    }

                    GridLayout {
                        id: gdtOperationalModesReportGrid

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

                            text: {
                                if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                        === 0) {
                                    qsTr("MODE:TX OFF")
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 1) {
                                    qsTr("MODE:TX HIGH")
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 2) {
                                    qsTr("MODE:TX LOW")
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 3) {
                                    qsTr("MODE:IBIT")
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 124) {
                                    qsTr("MODE:24dBm")
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 127) {
                                    qsTr("MODE:27dBm")
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 130) {
                                    qsTr("MODE:30dBm")
                                } else {
                                    qsTr("MODE:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: {
                                if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                        === 0) {
                                    "Red"
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 1) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 2) {
                                    "Yellow"
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 3) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 124) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 127) {
                                    "Green"
                                } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode === 130) {
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
                                if (QGroundControl.commtactLinkManagement.gdtAntennaSelect === 0) {
                                    qsTr("ANTENNA:OMNI")
                                } else if (QGroundControl.commtactLinkManagement.gdtAntennaSelect
                                           === 1) {
                                    qsTr("ANTENNA:DIRECTIONAL")
                                } else if (QGroundControl.commtactLinkManagement.gdtAntennaSelect
                                           === 22) {
                                    qsTr("ANTENNA:ADVANCED")
                                } else {
                                    qsTr("ANTENNA:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                if (QGroundControl.commtactLinkManagement.gdtPedestalTrackMode
                                        === 0) {
                                    qsTr("PEDESTAL:STOP")
                                } else if (QGroundControl.commtactLinkManagement.gdtPedestalTrackMode === 2) {
                                    qsTr("PEDESTAL:RSSI")
                                } else if (QGroundControl.commtactLinkManagement.gdtPedestalTrackMode === 3) {
                                    qsTr("PEDESTAL:GPS")
                                } else if (QGroundControl.commtactLinkManagement.gdtPedestalTrackMode === 4) {
                                    qsTr("PEDESTAL:MANUAL")
                                } else if (QGroundControl.commtactLinkManagement.gdtPedestalTrackMode === 5) {
                                    qsTr("PEDESTAL:AZIMUTH")
                                } else if (QGroundControl.commtactLinkManagement.gdtPedestalTrackMode === 7) {
                                    qsTr("PEDESTAL:STOW")
                                } else {
                                    qsTr("PEDESTAL:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                if (QGroundControl.commtactLinkManagement.gdtTddOperationalMode
                                        === 0) {
                                    qsTr("TDD:SLAVE")
                                } else if (QGroundControl.commtactLinkManagement.gdtTddOperationalMode === 1) {
                                    qsTr("TDD:MASTER")
                                } else {
                                    qsTr("TDD:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                if (QGroundControl.commtactLinkManagement.gdtFrequencyMode === 0) {
                                    qsTr("FREQ:FIX")
                                } else if (QGroundControl.commtactLinkManagement.gdtFrequencyMode
                                           === 1) {
                                    qsTr("FREQ:HOPPING")
                                } else {
                                    qsTr("FREQ:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                if (QGroundControl.commtactLinkManagement.gdtUnitMode === 0) {
                                    qsTr("UNIT:NORMAL")
                                } else if (QGroundControl.commtactLinkManagement.gdtUnitMode
                                           === 3) {
                                    qsTr("UNIT:MISSION RELAY")
                                } else if (QGroundControl.commtactLinkManagement.gdtUnitMode
                                           === 4) {
                                    qsTr("UNIT:RELAY")
                                } else if (QGroundControl.commtactLinkManagement.gdtUnitMode
                                           === 5) {
                                    qsTr("UNIT:MISSION")
                                } else if (QGroundControl.commtactLinkManagement.gdtUnitMode
                                           === 6) {
                                    qsTr("UNIT:TRANSIT")
                                } else if (QGroundControl.commtactLinkManagement.gdtUnitMode
                                           === 51) {
                                    qsTr("UNIT:MISSION TMO")
                                } else if (QGroundControl.commtactLinkManagement.gdtUnitMode
                                           === 56) {
                                    qsTr("UNIT:MISSION TRANSIT")
                                } else {
                                    qsTr("UNIT:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                if (QGroundControl.commtactLinkManagement.gdtSymbolRate === 0) {
                                    qsTr("SYMBOL RATE:16MSPS")
                                } else if (QGroundControl.commtactLinkManagement.gdtSymbolRate
                                           === 1) {
                                    qsTr("SYMBOL RATE:8MSPS")
                                } else if (QGroundControl.commtactLinkManagement.gdtSymbolRate
                                           === 2) {
                                    qsTr("SYMBOL RATE:4MSPS")
                                } else {
                                    qsTr("SYMBOL:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                if (QGroundControl.commtactLinkManagement.gdtAesEncryption === 0) {
                                    qsTr("AES ENC:DISABLED")
                                } else if (QGroundControl.commtactLinkManagement.gdtAesEncryption
                                           === 1) {
                                    qsTr("AES ENC:ENABLED")
                                } else {
                                    qsTr("AES ENC:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                if (QGroundControl.commtactLinkManagement.gdtSymbolRate === 0) {
                                    qsTr("SYMBOL RATE:16MSPS")
                                } else if (QGroundControl.commtactLinkManagement.gdtSymbolRate
                                           === 1) {
                                    qsTr("SYMBOL RATE:8MSPS")
                                } else if (QGroundControl.commtactLinkManagement.gdtSymbolRate
                                           === 2) {
                                    qsTr("SYMBOL RATE:4MSPS")
                                } else {
                                    qsTr("SYMBOL:NAN")
                                }
                            }
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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

                    id: gdtStatusReportGroupBox
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
                        color: "#ffe4c4"
                        text: gdtStatusReportGroupBox.title
                    }

                    GridLayout {
                        id: gdtStatusReportGrid

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
                                  + QGroundControl.commtactLinkManagement.gdtLinkRSSI
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                  + QGroundControl.commtactLinkManagement.gdtOperationFrequency
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                                  + QGroundControl.commtactLinkManagement.gdtTDDSync
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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

                            text: qsTr("TRANSFERED PACKETS:")
                                  + QGroundControl.commtactLinkManagement.gdtLinkTransferedPackets
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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

                            text: qsTr("ERROR PACKETS:")
                                  + QGroundControl.commtactLinkManagement.gdtLinkErrorPackets
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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

                            text: qsTr("CRC ERROR PACKETS:")
                                  + QGroundControl.commtactLinkManagement.gdtLinkCRCErrorPackets
                            font.family: ScreenTools.demiboldFontFamily
                            color: "White"

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
                        color: "#ffe4c4"
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
                            color: "White"

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
                            color: "White"

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
            }
        }
    }
}
