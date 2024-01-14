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
        rows: 7
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        onWidthChanged: {
            console.log("Record mode:", grid.width, grid.height)
        }

        QGCLabel {

            id: mainlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("RECORD")
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

            id: lrlabel

            height: ScreenTools.defaultFontPixelHeight

            text: qsTr("LOCAL")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
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
            Layout.columnSpan: 6

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

        QGCLabel {

            id: rrlabel

            text: qsTr("REMOTE")
            color: "White"

            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

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
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setVideoRecordingCommand(
                            1, _file_name)
            }
        }

        QGCButton {

            id: _recordOffModeButton

            showBorder: true
            text: qsTr("REC OFF")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setVideoRecordingCommand(
                            2, _file_name)
            }
        }

        QGCButton {

            id: _snapshotModeButton

            showBorder: true
            text: qsTr("SNAP")

            Layout.rowSpan: 1
            Layout.columnSpan: 6

            Layout.preferredHeight: rowHeight
            Layout.preferredWidth: columnWidth * Layout.columnSpan
            Layout.fillWidth: false
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignLeft

            onReleased: {
                joystickManager.epsilonCameraManagement.setDoSnapshotCommand(
                            1, 1, 1, 0, 0, _file_name)
            }
        }

        //        QGCLabel {

        //            id: aslabel

        //            Layout.columnSpan: 6
        //            Layout.alignment: Qt.AlignCenter

        //            text: qsTr("Auto Snapshots")
        //            font.family: ScreenTools.demiboldFontFamily
        //            font.pointSize: ScreenTools.largeFontPointSize
        //            height: ScreenTools.defaultFontPixelHeight
        //            color: "White"
        //        }

        //        QGCLabel {

        //            id:_intervalSnapshotLabel

        //            Layout.columnSpan: 2
        //            Layout.alignment: Qt.AlignCenter

        //            text:                   qsTr(" Interval[ms]")
        //            font.pointSize:         ScreenTools.isMobile ? point_size : 9
        //            color:                  "white"
        //        }

        //        QGCTextField {

        //            id:                     _snapInervalField

        //            Layout.columnSpan: 2

        //            maximumLength:          5
        //            font.pointSize:         ScreenTools.isMobile ? point_size : 9
        //            text:                   qsTr("1000")
        //        }

        //        QGCLabel {
        //            id:                     _snapStatusLabel

        //            Layout.columnSpan: 2
        //            Layout.alignment: Qt.AlignCenter

        //            text:                   qsTr("Idle")
        //            font.pointSize:         ScreenTools.isMobile ? point_size : 9
        //            color:                  "white"

        //            Connections {
        //                target: _activeVehicle
        //                onSnapShotStatusChanged: {
        //                    if( videoChannelCombo.currentIndex === 0 ){
        //                        _snapStatusLabel.text = (snapShotStatus === 1) ? qsTr("Busy") : qsTr("Idle")
        //                    } else if ( videoChannelCombo.currentIndex === 1 ){
        //                        _snapStatusLabel.text = (snapShotStatus >= 16) ? qsTr("Busy") : qsTr("Idle")
        //                    }
        //                }
        //            }
        //        }

        //        QGCLabel {

        //            id:                     _countLabel

        //            Layout.columnSpan: 2
        //            Layout.alignment: Qt.AlignCenter

        //            text:                   qsTr(" Count")
        //            font.pointSize:         ScreenTools.isMobile ? point_size : 9
        //            color:                  "white"
        //        }

        //        QGCTextField {

        //            id:                     _snapCount

        //            Layout.columnSpan: 2

        //            maximumLength:          4
        //            font.pointSize:         ScreenTools.isMobile ? point_size : 9
        //            text:                   qsTr("10")
        //        }

        //        QGCCheckBox {

        //            id:                     _snapInf

        //            Layout.columnSpan: 2
        //            Layout.alignment: Qt.AlignCenter

        //            text:                   qsTr("Inf")
        //            textColor:              "white"
        //            textFontPointSize:      ScreenTools.isMobile ? point_size : 9
        //        }

        //        QGCButton {

        //            id: _startSnapshotButton

        //            Layout.columnSpan: 3
        //            Layout.fillWidth: true
        //            Layout.fillHeight: true

        //            showBorder: true
        //            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
        //            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
        //            text: qsTr("Start")
        //            leftPadding: 0
        //            rightPadding: 0
        //            onReleased: {
        //                if( videoChannelCombo.currentIndex >= 0 )
        //                    joystickManager.cameraManagement.setSysAutoSnapshotCommand(_snapInerval.text,_snapCount.text,_snapInf.checked, videoChannelCombo.currentIndex);
        //            }
        //        }

        //        QGCButton {

        //            id: _stopSnapshotButton

        //            Layout.columnSpan: 3
        //            Layout.fillWidth: true
        //            Layout.fillHeight: true

        //            showBorder: true
        //            font.pointSize: ScreenTools.isMobile ? point_size : ScreenTools.smallFontPointSize
        //            pointSize: ScreenTools.isMobile ? point_size : ScreenTools.defaultFontPointSize
        //            text: qsTr("Stop")
        //            leftPadding: 0
        //            rightPadding: 0
        //            onReleased: {
        //                if( videoChannelCombo.currentIndex >= 0 )
        //                    joystickManager.cameraManagement.setSysAutoSnapshotCommand(0,0,_snapInf.checked, videoChannelCombo.currentIndex);
        //            }
        //        }
    }
}
