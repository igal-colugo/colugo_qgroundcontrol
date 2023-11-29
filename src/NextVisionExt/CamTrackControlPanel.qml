import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.2

import QGroundControl 1.0
import QGroundControl.Palette 1.0
import QGroundControl.Controls 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FactControls 1.0

Item {
    anchors.fill: parent
    visible: true

    GridLayout {

        id: grid

        columns: 5
        rows: 7
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 5
        rowSpacing: 5

        onWidthChanged: {
            console.log("Track mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("TRACK")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 5
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCLabel {

            id: actTracker

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("ACTIVE")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 5
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCRadioButton {

            text: qsTr("0")
            checked: true

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysActiveTrackerCommand(
                           0)
        }

        QGCRadioButton {

            text: qsTr("1")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysActiveTrackerCommand(
                           1)
        }

        QGCRadioButton {

            text: qsTr("2")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysActiveTrackerCommand(
                           2)
        }

        QGCRadioButton {

            text: qsTr("3")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysActiveTrackerCommand(
                           3)
        }

        QGCRadioButton {

            text: qsTr("4")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysActiveTrackerCommand(
                           4)
        }

        QGCLabel {

            id: primTracker

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("PRIMARY")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 5
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCRadioButton {

            text: qsTr("0")
            checked: true

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysPrimaryTrackerCommand(
                           0)
        }

        QGCRadioButton {

            text: qsTr("1")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysPrimaryTrackerCommand(
                           1)
        }

        QGCRadioButton {

            text: qsTr("2")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysPrimaryTrackerCommand(
                           2)
        }

        QGCRadioButton {

            text: qsTr("3")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysPrimaryTrackerCommand(
                           3)
        }

        QGCRadioButton {

            text: qsTr("4")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysPrimaryTrackerCommand(
                           4)
        }

        QGCLabel {

            id: trackerROI

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("ROI")
            font.family: ScreenTools.demiboldFontFamily
            font.pointSize: ScreenTools.largeFontPointSize
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            Layout.columnSpan: 5
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCRadioButton {

            text: qsTr("0")
            checked: true

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysTrackerROICommand(
                           0)
        }

        QGCRadioButton {

            text: qsTr("1")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysTrackerROICommand(
                           1)
        }

        QGCRadioButton {

            text: qsTr("2")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysPrimaryTrackerCommand(
                           2)
        }

        QGCRadioButton {

            text: qsTr("3")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysTrackerROICommand(
                           3)
        }

        QGCRadioButton {

            text: qsTr("4")
            checked: false

            Layout.rowSpan: 1
            Layout.columnSpan: 1

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onClicked: joystickManager.cameraManagement.setSysTrackerROICommand(
                           4)
        }
    }
}
