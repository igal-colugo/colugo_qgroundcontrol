

/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/
import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2

import QGroundControl 1.0
import QGroundControl.Controls 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Palette 1.0

Rectangle {
    id: _linkRoot
    color: qgcPal.window
    anchors.fill: parent
    anchors.margins: 2

    property var _currentSelection: null
    property int _firstColumnWidth: ScreenTools.defaultFontPixelWidth * 12
    property int _secondColumnWidth: ScreenTools.defaultFontPixelWidth * 30
    property int _rowSpacing: ScreenTools.defaultFontPixelHeight / 2
    property int _colSpacing: ScreenTools.defaultFontPixelWidth / 2

    property int gdtRowHeight: (parent.height / 3
                                - (gdtGrid.rows * gdtGrid.rowSpacing)) / (gdtGrid.rows)
    property int gdtColumnWidth: (parent.width - (gdtGrid.columns * gdtGrid.columnSpacing))
                                 / (gdtGrid.columns)

    property int adtRowHeight: (parent.height / 3
                                - (adtGrid.rows * adtGrid.rowSpacing)) / (adtGrid.rows)
    property int adtColumnWidth: (parent.width - (adtGrid.columns * adtGrid.columnSpacing))
                                 / (adtGrid.columns)

    QGCPalette {
        id: qgcPal
        colorGroupEnabled: enabled
    }

    function openCommSettings(originalLinkConfig) {
        settingsLoader.originalLinkConfig = originalLinkConfig
        if (originalLinkConfig) {
            // Editing existing link config
            settingsLoader.editingConfig
                    = QGroundControl.commtactLinkManager.startConfigurationEditing(
                        originalLinkConfig)
        } else {
            // Create new link configuration
            settingsLoader.editingConfig = QGroundControl.commtactLinkManager.createConfiguration(
                        CommtactLinkConfiguration.TypeUdp, "")
        }
        settingsLoader.sourceComponent = commSettings
    }

    Component.onDestruction: {
        if (settingsLoader.sourceComponent) {
            settingsLoader.sourceComponent = null
            QGroundControl.commtactLinkManager.cancelConfigurationEditing(
                        settingsLoader.editingConfig)
        }
    }

    //---------------- GDT settings -------------------
    Rectangle {
        id: _gdtSettings
        width: parent.width
        height: (parent.height - buttonRow.height) / 3
        color: qgcPal.window
        anchors.top: parent.top
        border.width: 2
        border.color: "White"

        QGCFlickable {
            clip: true
            anchors.fill: parent
            anchors.margins: ScreenTools.defaultFontPixelWidth
            flickableDirection: Flickable.VerticalFlick

            GridLayout {
                id: gdtGrid

                columns: 14
                rows: 7
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2

                onWidthChanged: {
                    console.log("Commtact gdt:", gdtGrid.width, gdtGrid.height)
                }

                QGCLabel {

                    id: gdtMainlabel

                    height: ScreenTools.defaultFontPixelHeight

                    text: qsTr("GDT SETTINGS")
                    font.family: ScreenTools.demiboldFontFamily
                    color: "White"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.columnSpan: 14
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                }
                QGCLabel {

                    height: ScreenTools.defaultFontPixelHeight

                    text: {
                        if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                === 0) {
                            qsTr("TX OFF")
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 1) {
                            qsTr("TX HIGH")
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 2) {
                            qsTr("TX LOW")
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 3) {
                            qsTr("IBIT")
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 124) {
                            qsTr("24dBm")
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 127) {
                            qsTr("27dBm")
                        } else {
                            qsTr("NAN")
                        }
                    }
                    font.family: ScreenTools.demiboldFontFamily
                    color: {
                        if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                === 0) {
                            "Red"
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 1) {
                            "Green"
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 2) {
                            "Yellow"
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 3) {
                            "Green"
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 124) {
                            "Green"
                        } else if (QGroundControl.commtactLinkManagement.transmitterOperationalMode
                                   === 127) {
                            "Green"
                        } else {
                            "White"
                        }
                    }

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft
                    Layout.columnSpan: 2
                    Layout.fillHeight: false
                    Layout.fillWidth: true
                }

                QGCButton {

                    id: _gdtTxLowButton

                    showBorder: true
                    text: qsTr("TX LOW")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                    2)
                    }
                }
                QGCButton {

                    id: _gdtTxHighButton

                    showBorder: true
                    text: qsTr("TX HIGH")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                    1)
                    }
                }
                QGCButton {

                    id: _gdtTxOffButton

                    showBorder: true
                    text: qsTr("TX OFF")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                    0)
                    }
                }
                QGCButton {

                    id: _gdtIbitButton

                    showBorder: true
                    text: qsTr("IBIT")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                    3)
                    }
                }
                QGCButton {

                    id: _gdt24dBmButton

                    showBorder: true
                    text: qsTr("24dBm")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                    124)
                    }
                }
                QGCButton {

                    id: _gdt27DbmButton

                    showBorder: true
                    text: qsTr("27dBm")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 2

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onClicked: {
                        QGroundControl.commtactLinkManagement.setGDTOperationalModeCommand(
                                    127)
                    }
                }

                QGCLabel {

                    id: _gdtSetCameraScreenInformationLabel

                    height: ScreenTools.defaultFontPixelHeight

                    text: qsTr("INFO")
                    color: "White"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft
                }
                QGCTextField {

                    id: _gdtSetCameraScreenInformationField

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    maximumLength: 4
                    font.pointSize: ScreenTools.isMobile ? point_size : 9
                    text: qsTr("2047")
                }
                QGCButton {

                    id: _gdtSetCameraScreenInformationButton

                    showBorder: true
                    text: qsTr("SET")

                    Layout.rowSpan: 1
                    Layout.columnSpan: 4

                    Layout.preferredHeight: gdtRowHeight
                    Layout.preferredWidth: gdtColumnWidth * Layout.columnSpan
                    Layout.fillWidth: false
                    Layout.fillHeight: false
                    Layout.alignment: Qt.AlignLeft

                    onReleased: {
                        if (parseInt(_gdtSetCameraScreenInformationField.text) >= 0
                                && parseInt(
                                    _gdtSetCameraScreenInformationField.text) <= 2047) {
                            joystickManager.epsilonCameraManagement.setCameraScreenInformationCommand(
                                        parseInt(
                                            _gdtSetCameraScreenInformationField.text))
                        }
                    }
                }
            }
        }
    }
    //---------------- ADT settings -------------------
    Rectangle {
        id: _adtSettings
        width: parent.width
        height: (parent.height - buttonRow.height) / 3
        color: qgcPal.window
        anchors.top: _gdtSettings.bottom
        border.width: 2
        border.color: "White"

        QGCFlickable {
            clip: true
            anchors.fill: parent
            anchors.margins: ScreenTools.defaultFontPixelWidth
            flickableDirection: Flickable.VerticalFlick

            GridLayout {
                id: adtGrid

                columns: 6
                rows: 7
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2

                onWidthChanged: {
                    console.log("Commtact adt:", adtGrid.width, adtGrid.height)
                }

                QGCLabel {

                    id: adtMainlabel

                    height: ScreenTools.defaultFontPixelHeight

                    text: qsTr("GDT SETTINGS")
                    font.family: ScreenTools.demiboldFontFamily
                    color: "White"

                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Layout.alignment: Qt.AlignLeft | Qt.AlignTop
                    Layout.columnSpan: 6
                    Layout.fillHeight: false
                    Layout.fillWidth: true
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

    //-- Links
    Rectangle {
        id: _links
        width: parent.width
        height: (parent.height - buttonRow.height) / 3
        color: qgcPal.window
        anchors.top: _adtSettings.bottom
        border.width: 2
        border.color: "White"
        //-------------------------------------------------------------------------
        QGCFlickable {
            clip: true
            anchors.fill: parent
            anchors.margins: ScreenTools.defaultFontPixelWidth
            contentHeight: settingsColumn.height
            contentWidth: settingsColumn.width
            flickableDirection: Flickable.VerticalFlick

            Column {
                id: settingsColumn
                width: _links.width
                spacing: ScreenTools.defaultFontPixelHeight * 0.5
                anchors.margins: ScreenTools.defaultFontPixelWidth

                Item {
                    width: _links.width * 0.8
                    height: linksLabel.height
                    anchors.margins: ScreenTools.defaultFontPixelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                    QGCLabel {
                        id: linksLabel
                        text: qsTr("Links")
                        font.family: ScreenTools.demiboldFontFamily
                    }
                }

                Repeater {
                    model: QGroundControl.commtactLinkManager.linkConfigurations
                    delegate: QGCButton {
                        anchors.horizontalCenter: settingsColumn.horizontalCenter
                        width: _linkRoot.width * 0.5
                        text: object.name
                        autoExclusive: true
                        visible: !object.dynamic
                        onClicked: {
                            checked = true
                            _currentSelection = object
                            QGroundControl.commtactLinkManager.selectConfiguration(
                                        _currentSelection)
                        }
                    }
                }
            }
        }
    }

    Row {
        id: buttonRow
        spacing: ScreenTools.defaultFontPixelWidth
        anchors.bottom: parent.bottom
        anchors.margins: ScreenTools.defaultFontPixelWidth
        anchors.horizontalCenter: parent.horizontalCenter
        QGCButton {
            width: ScreenTools.defaultFontPixelWidth * 10
            text: qsTr("Delete")
            enabled: _currentSelection && !_currentSelection.dynamic
            onClicked: deleteDialog.visible = true

            MessageDialog {
                id: deleteDialog
                visible: false
                icon: StandardIcon.Warning
                standardButtons: StandardButton.Yes | StandardButton.No
                title: qsTr("Remove Link Configuration")
                text: _currentSelection ? qsTr("Remove %1. Is this really what you want?").arg(
                                              _currentSelection.name) : ""

                onYes: {
                    QGroundControl.commtactLinkManager.removeConfiguration(
                                _currentSelection)
                    _currentSelection = null
                    deleteDialog.visible = false
                }
                onNo: deleteDialog.visible = false
            }
        }
        QGCButton {
            text: qsTr("Edit")
            enabled: _currentSelection && !_currentSelection.link
            onClicked: _linkRoot.openCommSettings(_currentSelection)
        }
        QGCButton {
            text: qsTr("Add")
            onClicked: _linkRoot.openCommSettings(null)
        }
        QGCButton {
            text: qsTr("Connect")
            enabled: _currentSelection && !_currentSelection.link
            onClicked: QGroundControl.commtactLinkManager.createConnectedLink(
                           _currentSelection)
        }
        QGCButton {
            text: qsTr("Disconnect")
            enabled: _currentSelection && _currentSelection.link
            onClicked: _currentSelection.link.disconnect()
        }
    }

    Loader {
        id: settingsLoader
        anchors.fill: parent
        visible: sourceComponent ? true : false

        property var originalLinkConfig: null
        property var editingConfig: null
    }

    //---------------------------------------------
    // Comm Settings
    Component {
        id: commSettings
        Rectangle {
            id: settingsRect
            color: qgcPal.window
            anchors.fill: parent
            property real _panelWidth: width * 0.8

            QGCFlickable {
                id: settingsFlick
                clip: true
                anchors.fill: parent
                anchors.margins: ScreenTools.defaultFontPixelWidth
                contentHeight: mainLayout.height
                contentWidth: mainLayout.width

                ColumnLayout {
                    id: mainLayout
                    spacing: _rowSpacing

                    QGCGroupBox {
                        title: originalLinkConfig ? qsTr("Edit Link Configuration Settings") : qsTr(
                                                        "Create New Link Configuration")

                        ColumnLayout {
                            spacing: _rowSpacing

                            GridLayout {
                                columns: 2
                                columnSpacing: _colSpacing
                                rowSpacing: _rowSpacing

                                QGCLabel {
                                    text: qsTr("Name")
                                }
                                QGCTextField {
                                    id: nameField
                                    Layout.preferredWidth: _secondColumnWidth
                                    Layout.fillWidth: true
                                    text: editingConfig.name
                                    placeholderText: qsTr("Enter name")
                                }

                                QGCLabel {
                                    text: qsTr("Type")
                                }
                                QGCComboBox {
                                    Layout.preferredWidth: _secondColumnWidth
                                    Layout.fillWidth: true
                                    enabled: originalLinkConfig == null
                                    model: QGroundControl.commtactLinkManager.linkTypeStrings
                                    currentIndex: editingConfig.linkType

                                    onActivated: {
                                        if (index !== editingConfig.linkType) {
                                            // Save current name
                                            var name = nameField.text
                                            // Create new link configuration
                                            editingConfig = QGroundControl.commtactLinkManager.createConfiguration(
                                                        index, name)
                                        }
                                    }
                                }
                            }

                            Loader {
                                id: linksettingsLoader
                                source: subEditConfig.settingsURL

                                property var subEditConfig: editingConfig
                            }
                        }
                    }

                    RowLayout {
                        Layout.alignment: Qt.AlignHCenter
                        spacing: _colSpacing

                        QGCButton {
                            width: ScreenTools.defaultFontPixelWidth * 10
                            text: qsTr("OK")
                            enabled: nameField.text !== ""

                            onClicked: {
                                // Save editing
                                linksettingsLoader.item.saveSettings()
                                editingConfig.name = nameField.text
                                settingsLoader.sourceComponent = null
                                if (originalLinkConfig) {
                                    QGroundControl.commtactLinkManager.endConfigurationEditing(
                                                originalLinkConfig,
                                                editingConfig)
                                } else {
                                    // If it was edited, it's no longer "dynamic"
                                    editingConfig.dynamic = false
                                    QGroundControl.commtactLinkManager.endCreateConfiguration(
                                                editingConfig)
                                }
                            }
                        }

                        QGCButton {
                            width: ScreenTools.defaultFontPixelWidth * 10
                            text: qsTr("Cancel")
                            onClicked: {
                                settingsLoader.sourceComponent = null
                                QGroundControl.commtactLinkManager.cancelConfigurationEditing(
                                            settingsLoader.editingConfig)
                            }
                        }
                    }
                }
            }
        }
    }
}
