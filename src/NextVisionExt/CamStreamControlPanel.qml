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
    id: element
    anchors.fill: parent
    visible: true

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)
    property int columnWidth: (parent.width - (grid.columns * grid.columnSpacing)) / (grid.columns)
    property var _streamModesStr: ["Ch-0 Day/IR : Ch-1 Dis    ", "Ch-0 Day    : Ch-1 IR    ", "Ch-0 Day    : Ch-1 Fusion    ", "Ch-0 Day    : Ch-1 PIP    ", "Ch-0 Day    : Ch-1 SBS    ", "Ch-0 IR     : Ch-1 Fusion    ", "Ch-0 IR     : Ch-1 PIP    ", "Ch-0 IR     : Ch-1 SBS    ", "Ch-0 Fusion : Ch-1 Dis    ", "Ch-0 PIP    : Ch-1 Dis    ", "Ch-0 SBS    : Ch-1 Dis    "]
    property var _pipModesStr: ["Visable Large    ", "IR Large    "]
    property var _sbsModesStr: ["Visable Left    ", "IR Left    "]

    function setStreamModes(modesCombo) {

        //var modesCombo = streamModeCombo.currentIndex;
        switch (modesCombo) {
        case 0:
            joystickManager.cameraManagement.setSysStreamModeCommand(1, 0)
            break
        case 1:
            joystickManager.cameraManagement.setSysStreamModeCommand(1, 2)
            break
        case 2:
            joystickManager.cameraManagement.setSysStreamModeCommand(1, 3)
            break
        case 3:
            joystickManager.cameraManagement.setSysStreamModeCommand(1, 4)
            break
        case 4:
            joystickManager.cameraManagement.setSysStreamModeCommand(1, 5)
            break
        case 5:
            joystickManager.cameraManagement.setSysStreamModeCommand(2, 3)
            break
        case 6:
            joystickManager.cameraManagement.setSysStreamModeCommand(2, 4)
            break
        case 7:
            joystickManager.cameraManagement.setSysStreamModeCommand(2, 5)
            break
        case 8:
            joystickManager.cameraManagement.setSysStreamModeCommand(3, 0)
            break
        case 9:
            joystickManager.cameraManagement.setSysStreamModeCommand(4, 0)
            break
        case 10:
            joystickManager.cameraManagement.setSysStreamModeCommand(5, 0)
            break
        }
    }

    GridLayout {
        id: grid

        columns: 6
        rows: 7
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("Stream control mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("STREAM CTRL")
            font.family: ScreenTools.demiboldFontFamily
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            //Layout.preferredHeight: rowHeight
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCLabel {

            id: streamModeLabel

            text: qsTr("STREAM MODE")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCComboBox {
            id: streamModeCombo
            sizeToContents: true
            centeredLabel: true
            model: _streamModesStr

            Layout.rowSpan: 1
            Layout.columnSpan: 5

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: streamModeSetButton

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                setStreamModes(streamModeCombo.currentIndex)
            }
        }

        QGCLabel {

            id: pipModeLabel

            text: qsTr("PIP MODE")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCComboBox {
            id: pipModeCombo
            sizeToContents: true
            centeredLabel: true
            model: _pipModesStr

            Layout.rowSpan: 1
            Layout.columnSpan: 5

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: pipModeSetButton

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (pipModeCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysPIPModeCommand(
                                pipModeCombo.currentIndex)
            }
        }

        QGCLabel {

            id: sbsModeLabel

            text: qsTr("SBS MODE")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.columnSpan: 6
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCComboBox {
            id: sbsModeCombo
            sizeToContents: true
            centeredLabel: true
            model: _sbsModesStr

            Layout.rowSpan: 1
            Layout.columnSpan: 5

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: sbsModeSetButton

            showBorder: true
            text: qsTr("SET")

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (sbsModeCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysSBSModeCommand(
                                sbsModeCombo.currentIndex)
            }
        }
    }
}
