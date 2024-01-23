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

    property var _videoChannelsStr: ["Channel-0", "Channel-1"]
    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle

    // The following properties relate to a simple camera
    property var _flyViewSettings: QGroundControl.settingsManager.flyViewSettings
    property bool _simpleCameraAvailable: _activeVehicle
                                          && _flyViewSettings.showSimpleCameraControl.rawValue
    property bool _onlySimpleCameraAvailable: !_anyVideoStreamAvailable
                                              && _simpleCameraAvailable
    property bool _simpleCameraIsShootingInCurrentMode: _onlySimpleCameraAvailable
                                                        && !_simplePhotoCaptureIsIdle

    // The following properties relate to a simple video stream
    property bool _videoStreamAvailable: _videoStreamManager.hasVideo
    property var _videoStreamSettings: QGroundControl.settingsManager.videoSettings
    property var _videoStreamManager: QGroundControl.videoManager
    property bool _videoStreamAllowsPhotoWhileRecording: true
    property bool _videoStreamIsStreaming: _videoStreamManager.streaming
    property bool _simplePhotoCaptureIsIdle: true
    property bool _videoStreamRecording: _videoStreamManager.recording
    property bool _videoStreamCanShoot: _videoStreamIsStreaming
    property bool _videoStreamIsShootingInCurrentMode: _videoStreamInPhotoMode ? !_simplePhotoCaptureIsIdle : _videoStreamRecording
    property bool _videoStreamInPhotoMode: false

    property bool _anyVideoStreamAvailable: _videoStreamManager.hasVideo
    property string _cameraName: ""
    property bool _showModeIndicator: _videoStreamManager.hasVideo
    property bool _modeIndicatorPhotoMode: _videoStreamInPhotoMode
                                           || _onlySimpleCameraAvailable
    property bool _allowsPhotoWhileRecording: _videoStreamAllowsPhotoWhileRecording
    property bool _switchToPhotoModeAllowed: !_modeIndicatorPhotoMode
    property bool _switchToVideoModeAllowed: _modeIndicatorPhotoMode
    property bool _videoIsRecording: _videoStreamRecording
    property bool _canShootInCurrentMode: _videoStreamCanShoot
                                          || _simpleCameraAvailable
    property bool _isShootingInCurrentMode: _videoStreamIsShootingInCurrentMode
                                            || _simpleCameraIsShootingInCurrentMode

    property string _file_name: "COLUGO"

    function toggleShooting() {
        console.log("toggleShooting", _anyVideoStreamAvailable)
        if (_anyVideoStreamAvailable) {
            if (_videoStreamManager.recording) {
                _videoStreamManager.stopRecording()
            } else {
                _videoStreamManager.startRecording()
            }
        }
    }

    GridLayout {
        id: grid

        columns: 6
        rows: 8
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("Record mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            text: qsTr("REC")
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

            id: lrlabel

            text: qsTr("LOCAL")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.columnSpan: 3
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        Rectangle {

            id: localrec

            color: Qt.rgba(0, 0, 0, 0)
            radius: width * 0.5
            border.color: qgcPal.buttonText
            border.width: 3

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: rowHeight
            Layout.fillWidth: false
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Rectangle {
                anchors.centerIn: parent
                width: parent.width * (_isShootingInCurrentMode ? 0.5 : 0.75)
                height: width
                radius: _isShootingInCurrentMode ? 0 : width * 0.5
                color: _canShootInCurrentMode ? qgcPal.colorRed : qgcPal.colorGrey
            }
            MouseArea {
                anchors.fill: parent
                enabled: _canShootInCurrentMode
                onClicked: toggleShooting()
            }
        }

        QGCComboBox {

            id: videoChannelCombo
            sizeToContents: true
            centeredLabel: true
            model: _videoChannelsStr

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: false
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _recordOnModeButton

            showBorder: true
            text: qsTr("REC ON")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (videoChannelCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysRecOnCommand(
                                videoChannelCombo.currentIndex)
            }
        }

        QGCButton {

            id: _recordOffModeButton

            showBorder: true
            text: qsTr("REC OFF")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (videoChannelCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysRecOffCommand(
                                videoChannelCombo.currentIndex)
            }
        }

        QGCButton {

            id: _snapshotModeButton

            showBorder: true
            text: qsTr("SNAP")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * 2
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (videoChannelCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysSnapshotCommand(
                                videoChannelCombo.currentIndex)
            }
        }

        QGCLabel {

            text: qsTr("INT[ms]")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCTextField {

            id: _snapInerval
            maximumLength: 5
            text: qsTr("1000")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCLabel {

            id: _snapStatus
            text: qsTr("IDLE")
            color: "white"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true

            Connections {
                target: _activeVehicle
                onSnapShotStatusChanged: {
                    if (videoChannelCombo.currentIndex == 0) {
                        _snapStatus.text = snapShotStatus == 1 ? qsTr("Busy") : qsTr(
                                                                     "Idle")
                    } else if (videoChannelCombo.currentIndex == 1) {
                        _snapStatus.text = snapShotStatus >= 16 ? qsTr("Busy") : qsTr(
                                                                      "Idle")
                    }
                }
            }
        }

        QGCLabel {

            text: qsTr("COUNT")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.preferredHeight: rowHeight
            Layout.columnSpan: 2
            Layout.fillHeight: false
            Layout.fillWidth: true
        }

        QGCTextField {

            id: _snapCount
            maximumLength: 5
            text: qsTr("10")

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCCheckBox {

            id: _snapInf
            text: qsTr("INF")
            textColor: "white"

            Layout.rowSpan: 1
            Layout.columnSpan: 2

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft
        }

        QGCButton {

            id: _startModeButton

            showBorder: true
            text: qsTr("START")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (videoChannelCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysAutoSnapshotCommand(
                                _snapInerval.text, _snapCount.text,
                                _snapInf.checked,
                                videoChannelCombo.currentIndex)
            }
        }

        QGCButton {

            id: _stopModeButton

            showBorder: true
            text: qsTr("STOP")

            Layout.rowSpan: 1
            Layout.columnSpan: 3

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: -1
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                if (videoChannelCombo.currentIndex >= 0)
                    joystickManager.cameraManagement.setSysAutoSnapshotCommand(
                                0, 0, _snapInf.checked,
                                videoChannelCombo.currentIndex)
            }
        }
    }
}
