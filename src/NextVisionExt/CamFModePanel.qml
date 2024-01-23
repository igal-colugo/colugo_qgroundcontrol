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
    id: widgetFMode
    anchors.fill: parent
    visible: true

    property int rowHeight: (parent.height - (grid.rows * grid.rowSpacing)) / (grid.rows)

    GridLayout {

        id: grid

        columns: 6
        rows: 4
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("F mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("F.MODE")
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

        QGCButton {

            id: _syonModeButton

            showBorder: true
            text: qsTr("SY ON")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysSingleYawOnCommand()
            }
        }

        QGCButton {

            id: _syoffModeButton

            showBorder: true
            text: qsTr("SY OFF")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysSingleYawOffCommand()
            }
        }

        QGCButton {

            id: _followonModeButton

            showBorder: true
            text: qsTr("FLW ON")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysFollowOnCommand()
            }
        }

        QGCButton {

            id: _followoffModeButton

            showBorder: true
            text: qsTr("FLW OFF")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysFollowOffCommand()
            }
        }

        QGCButton {

            id: _flyaonModeButton

            showBorder: true
            text: qsTr("FLY.A ON")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysFlyAboveOnCommand()
            }
        }

        QGCButton {

            id: _flyaoffModeButton

            showBorder: true
            text: qsTr("FLY.A OFF")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: -1
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.cameraManagement.setSysFlyAboveOffCommand()
            }
        }
    }
}
