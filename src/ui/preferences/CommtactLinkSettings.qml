

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
    anchors.margins: ScreenTools.defaultFontPixelWidth

    property var _currentSelection: null
    property int _firstColumnWidth: ScreenTools.defaultFontPixelWidth * 12
    property int _secondColumnWidth: ScreenTools.defaultFontPixelWidth * 30
    property int _rowSpacing: ScreenTools.defaultFontPixelHeight / 2
    property int _colSpacing: ScreenTools.defaultFontPixelWidth / 2

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

    //-- Header
    Rectangle {
        id: _header
        width: parent.width
        height: (parent.height - buttonRow.height) / 2
        color: qgcPal.window
        anchors.top: parent.top

        QGCFlickable {
            clip: true
            anchors.fill: parent
            anchors.margins: ScreenTools.defaultFontPixelWidth
            contentHeight: settingsColumn_0.height
            contentWidth: settingsColumn_0.width
            flickableDirection: Flickable.VerticalFlick

            Column {
                id: settingsColumn_0
                width: _header.width
                spacing: ScreenTools.defaultFontPixelHeight * 0.5
                anchors.margins: ScreenTools.defaultFontPixelWidth
                //-----------------------------------------------------------------
                //-- GDT settings
                Item {
                    width: _header.width * 0.8
                    height: gdtLabel.height
                    anchors.margins: ScreenTools.defaultFontPixelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                    QGCLabel {
                        id: gdtLabel
                        text: qsTr("GDT settings")
                        font.family: ScreenTools.demiboldFontFamily
                    }
                }
                Rectangle {
                    height: gdtColumn.height + (ScreenTools.defaultFontPixelHeight * 2)
                    width: _header.width * 0.8
                    color: qgcPal.windowShade
                    anchors.margins: ScreenTools.defaultFontPixelWidth
                    anchors.horizontalCenter: parent.horizontalCenter

                    Column {
                        id: gdtColumn
                        spacing: ScreenTools.defaultFontPixelWidth
                        anchors.centerIn: parent
                        Row {
                            spacing: ScreenTools.defaultFontPixelWidth
                            QGCLabel {
                                width: 200
                                anchors.baseline: gdtFreq.baseline
                                text: qsTr("Frequency:")
                            }
                            QGCTextField {
                                id: gdtFreq
                                text: QGroundControl.mavlinkSystemID.toString()
                                width: 100
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                anchors.verticalCenter: parent.verticalCenter
                                onEditingFinished: {
                                    saveItems()
                                }
                            }
                            QGCButton {
                                text: qsTr("SET")
                                width: 100
                                enabled: true
                                onClicked: QGroundControl.commtactLinkManagement.setCameraModeCommand(
                                               0)
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }

                        //                        QGCCheckBox {
                        //                            text: qsTr("Emit heartbeat")
                        //                            checked: QGroundControl.multiVehicleManager.gcsHeartBeatEnabled
                        //                            onClicked: {
                        //                                QGroundControl.multiVehicleManager.gcsHeartBeatEnabled = checked
                        //                            }
                        //                        }

                        //                        QGCCheckBox {
                        //                            text: qsTr("Only accept MAVs with same protocol version")
                        //                            checked: QGroundControl.isVersionCheckEnabled
                        //                            onClicked: {
                        //                                QGroundControl.isVersionCheckEnabled = checked
                        //                            }
                        //                        }

                        //                        FactCheckBox {
                        //                            id: mavlinkForwardingChecked
                        //                            text: qsTr("Enable MAVLink forwarding")
                        //                            fact: QGroundControl.settingsManager.appSettings.forwardMavlink
                        //                            visible: QGroundControl.settingsManager.appSettings.forwardMavlink.visible
                        //                        }
                    }
                }

                //-- ADT settings
                Item {
                    width: _header.width * 0.8
                    height: adtLabel.height
                    anchors.margins: ScreenTools.defaultFontPixelWidth
                    anchors.horizontalCenter: parent.horizontalCenter
                    QGCLabel {
                        id: adtLabel
                        text: qsTr("ADT settings")
                        font.family: ScreenTools.demiboldFontFamily
                    }
                }
                Rectangle {
                    height: adtColumn.height + (ScreenTools.defaultFontPixelHeight * 2)
                    width: _header.width * 0.8
                    color: qgcPal.windowShade
                    anchors.margins: ScreenTools.defaultFontPixelWidth
                    anchors.horizontalCenter: parent.horizontalCenter

                    Column {
                        id: adtColumn
                        spacing: ScreenTools.defaultFontPixelWidth
                        anchors.centerIn: parent
                        Row {
                            spacing: ScreenTools.defaultFontPixelWidth
                            QGCLabel {
                                width: 200
                                anchors.baseline: adtFreq.baseline
                                text: qsTr("Frequency:")
                            }
                            QGCTextField {
                                id: adtFreq
                                text: QGroundControl.mavlinkSystemID.toString()
                                width: 100
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                anchors.verticalCenter: parent.verticalCenter
                                onEditingFinished: {
                                    saveItems()
                                }
                            }
                        }

                        //                        QGCCheckBox {
                        //                            text: qsTr("Emit heartbeat")
                        //                            checked: QGroundControl.multiVehicleManager.gcsHeartBeatEnabled
                        //                            onClicked: {
                        //                                QGroundControl.multiVehicleManager.gcsHeartBeatEnabled = checked
                        //                            }
                        //                        }

                        //                        QGCCheckBox {
                        //                            text: qsTr("Only accept MAVs with same protocol version")
                        //                            checked: QGroundControl.isVersionCheckEnabled
                        //                            onClicked: {
                        //                                QGroundControl.isVersionCheckEnabled = checked
                        //                            }
                        //                        }

                        //                        FactCheckBox {
                        //                            id: mavlinkForwardingChecked
                        //                            text: qsTr("Enable MAVLink forwarding")
                        //                            fact: QGroundControl.settingsManager.appSettings.forwardMavlink
                        //                            visible: QGroundControl.settingsManager.appSettings.forwardMavlink.visible
                        //                        }
                    }
                }
            }
        }
    }

    //-- Links
    Rectangle {
        id: _links
        width: parent.width
        height: (parent.height - buttonRow.height) / 2
        color: qgcPal.window
        anchors.top: _header.bottom
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
