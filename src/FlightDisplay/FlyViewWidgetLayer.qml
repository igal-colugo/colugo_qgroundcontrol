

/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/
import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.12

import QtLocation 5.3
import QtPositioning 5.3
import QtQuick.Window 2.2
import QtQml.Models 2.1

import QGroundControl 1.0
import QGroundControl.Controls 1.0
import QGroundControl.Airspace 1.0
import QGroundControl.Airmap 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.Controls 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FlightDisplay 1.0
import QGroundControl.FlightMap 1.0
import QGroundControl.Palette 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Vehicle 1.0

// This is the ui overlay layer for the widgets/tools for Fly View
Item {
    id: _root

    property var parentToolInsets
    property var totalToolInsets: _totalToolInsets
    property var mapControl

    property var _activeVehicle: QGroundControl.multiVehicleManager.activeVehicle
    property var _planMasterController: globals.planMasterControllerFlyView
    property var _missionController: _planMasterController.missionController
    property var _geoFenceController: _planMasterController.geoFenceController
    property var _rallyPointController: _planMasterController.rallyPointController
    property var _guidedController: globals.guidedControllerFlyView
    property real _margins: ScreenTools.defaultFontPixelWidth / 2
    property real _toolsMargin: ScreenTools.defaultFontPixelWidth * 0.75
    property rect _centerViewport: Qt.rect(0, 0, width, height)
    property real _rightPanelWidth: ScreenTools.defaultFontPixelWidth * 30

    property int _standardPhotoVideoEnabled: QGroundControl.settingsManager.appSettings.enableStandardPhotoVideo.value
    property int _nextVisionEnabled: QGroundControl.settingsManager.appSettings.enableNextVision.value
    property int _asioEnabled: QGroundControl.settingsManager.appSettings.enableAsio.value
    property int _epsilonEnabled: QGroundControl.settingsManager.appSettings.enableEpsilon.value

    property int _loaderStandardHeight: 110
    property int _loaderNextVisionHeight: 180
    property int _loaderEpsilonHeight: 180
    property int _loaderAsioHeight: 100

    QGCToolInsets {
        id: _totalToolInsets
        leftEdgeTopInset: toolStrip.leftInset
        leftEdgeCenterInset: toolStrip.leftInset
        leftEdgeBottomInset: parentToolInsets.leftEdgeBottomInset
        rightEdgeTopInset: parentToolInsets.rightEdgeTopInset
        rightEdgeCenterInset: parentToolInsets.rightEdgeCenterInset
        rightEdgeBottomInset: parentToolInsets.rightEdgeBottomInset
        topEdgeLeftInset: parentToolInsets.topEdgeLeftInset
        topEdgeCenterInset: parentToolInsets.topEdgeCenterInset
        topEdgeRightInset: parentToolInsets.topEdgeRightInset
        bottomEdgeLeftInset: parentToolInsets.bottomEdgeLeftInset
        bottomEdgeCenterInset: mapScale.centerInset
        bottomEdgeRightInset: 0
    }

    FlyViewMissionCompleteDialog {
        missionController: _missionController
        geoFenceController: _geoFenceController
        rallyPointController: _rallyPointController
    }

    Row {
        id: multiVehiclePanelSelector
        anchors.margins: _toolsMargin
        anchors.top: parent.top
        anchors.right: parent.right
        width: _rightPanelWidth
        spacing: ScreenTools.defaultFontPixelWidth
        visible: QGroundControl.multiVehicleManager.vehicles.count > 1
                 && QGroundControl.corePlugin.options.flyView.showMultiVehicleList

        property bool showSingleVehiclePanel: !visible
                                              || singleVehicleRadio.checked

        QGCMapPalette {
            id: mapPal
            lightColors: true
        }

        QGCRadioButton {
            id: singleVehicleRadio
            text: qsTr("Single")
            checked: true
            textColor: mapPal.text
        }

        QGCRadioButton {
            text: qsTr("Multi-Vehicle")
            textColor: mapPal.text
        }
    }

    MultiVehicleList {
        anchors.margins: _toolsMargin
        anchors.top: multiVehiclePanelSelector.bottom
        anchors.right: parent.right
        width: _rightPanelWidth
        height: parent.height - y - _toolsMargin
        visible: !multiVehiclePanelSelector.showSingleVehiclePanel
    }

    FlyViewInstrumentPanel {
        id: instrumentPanel
        anchors.margins: _toolsMargin
        anchors.top: multiVehiclePanelSelector.visible ? multiVehiclePanelSelector.bottom : parent.top
        anchors.right: parent.right
        width: _rightPanelWidth
        spacing: _toolsMargin
        visible: QGroundControl.corePlugin.options.flyView.showInstrumentPanel
                 && multiVehiclePanelSelector.showSingleVehiclePanel
        availableHeight: parent.height - y - _toolsMargin

        property real rightInset: visible ? parent.width - x : 0
    }

    Loader {
        id: _standard_loader
        anchors.margins: _toolsMargin
        anchors.right: parent.right
        width: _rightPanelWidth
        height: _loaderStandardHeight
        anchors.top: undefined
        anchors.verticalCenter: _root.verticalCenter

        sourceComponent: {
            (_standardPhotoVideoEnabled > 0) ? _standard : null
        }
    }

    Loader {
        id: _next_vision_loader
        anchors.margins: _toolsMargin
        anchors.right: parent.right
        width: _rightPanelWidth
        height: _loaderNextVisionHeight
        anchors.top: {
            (_standardPhotoVideoEnabled > 0) ? _standard_loader.bottom : undefined
        }
        anchors.verticalCenter: {
            (_standardPhotoVideoEnabled > 0) ? undefined : _root.verticalCenter
        }

        sourceComponent: {
            (_nextVisionEnabled > 0) ? _next_vision : null
        }
    }

    Loader {
        id: _epsilon_loader
        anchors.margins: _toolsMargin
        anchors.right: parent.right
        width: _rightPanelWidth
        height: _loaderEpsilonHeight
        anchors.top: {
            if (_standardPhotoVideoEnabled > 0 && _nextVisionEnabled > 0) {
                _next_vision_loader.bottom
            } else if (_standardPhotoVideoEnabled > 0
                       && _nextVisionEnabled <= 0) {
                _standard_loader.bottom
            } else {
                undefined
            }
        }
        anchors.verticalCenter: {
            if (_standardPhotoVideoEnabled > 0 && _nextVisionEnabled > 0) {
                undefined
            } else if (_standardPhotoVideoEnabled > 0
                       && _nextVisionEnabled <= 0) {
                undefined
            } else {
                _root.verticalCenter
            }
        }

        sourceComponent: {
            (_epsilonEnabled > 0) ? _epsilon : null
        }
    }

    Loader {
        id: _obox_loader
        anchors.margins: _toolsMargin
        anchors.right: parent.right
        width: _rightPanelWidth
        height: _loaderAsioHeight
        anchors.top: {
            if ((_standardPhotoVideoEnabled > 0 && _nextVisionEnabled > 0
                 && _epsilonEnabled > 0) || (_standardPhotoVideoEnabled > 0
                                             && _nextVisionEnabled <= 0
                                             && _epsilonEnabled > 0)) {
                _epsilon_loader.bottom
            } else if ((_standardPhotoVideoEnabled > 0 && _nextVisionEnabled > 0
                        && _epsilonEnabled <= 0)
                       || (_standardPhotoVideoEnabled <= 0
                           && _nextVisionEnabled > 0 && _epsilonEnabled > 0)) {
                _next_vision_loader.bottom
            } else if (_standardPhotoVideoEnabled > 0 && _nextVisionEnabled <= 0
                       && _epsilonEnabled <= 0) {
                _standard_loader.bottom
            } else {
                undefined
            }
        }
        anchors.verticalCenter: {
            if ((_standardPhotoVideoEnabled > 0 && _nextVisionEnabled > 0
                 && _epsilonEnabled > 0) || (_standardPhotoVideoEnabled > 0
                                             && _nextVisionEnabled <= 0
                                             && _epsilonEnabled > 0)) {
                undefined
            } else if ((_standardPhotoVideoEnabled > 0 && _nextVisionEnabled > 0
                        && _epsilonEnabled <= 0)
                       || (_standardPhotoVideoEnabled <= 0
                           && _nextVisionEnabled > 0 && _epsilonEnabled > 0)) {
                undefined
            } else if (_standardPhotoVideoEnabled > 0 && _nextVisionEnabled <= 0
                       && _epsilonEnabled <= 0) {
                undefined
            } else {
                _root.verticalCenter
            }
        }

        sourceComponent: {
            (_asioEnabled === 1) ? _obox : null
        }
    }

    Component {
        id: _standard
        PhotoVideoControl {
            id: _standard_photo_video_control
        }
    }

    Component {
        id: _next_vision
        NextVisionPhotoVideoControl {
            id: _next_vision_photo_video_control
        }
    }

    Component {
        id: _obox
        OboxControl {
            id: _obox_control
        }
    }

    Component {
        id: _epsilon
        EpsilonPhotoVideoControl {
            id: _epsilon_photo_video_control
        }
    }

    TelemetryValuesBar {
        id: telemetryPanel
        x: recalcXPosition()
        anchors.margins: _toolsMargin

        // States for custom layout support
        states: [
            State {
                name: "bottom"
                when: telemetryPanel.bottomMode

                AnchorChanges {
                    target: telemetryPanel
                    anchors.top: undefined
                    anchors.bottom: parent.bottom
                    anchors.right: undefined
                    anchors.verticalCenter: undefined
                }

                PropertyChanges {
                    target: telemetryPanel
                    x: recalcXPosition()
                }
            },

            State {
                name: "right-video"
                when: !telemetryPanel.bottomMode && photoVideoControl.visible

                AnchorChanges {
                    target: telemetryPanel
                    anchors.top: photoVideoControl.bottom
                    anchors.bottom: undefined
                    anchors.right: parent.right
                    anchors.verticalCenter: undefined
                }
            },

            State {
                name: "right-novideo"
                when: !telemetryPanel.bottomMode && !photoVideoControl.visible

                AnchorChanges {
                    target: telemetryPanel
                    anchors.top: undefined
                    anchors.bottom: undefined
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        ]

        function recalcXPosition() {
            // First try centered
            var halfRootWidth = _root.width / 2
            var halfPanelWidth = telemetryPanel.width / 2
            var leftX = (halfRootWidth - halfPanelWidth) - _toolsMargin
            var rightX = (halfRootWidth + halfPanelWidth) + _toolsMargin
            if (leftX >= parentToolInsets.leftEdgeBottomInset
                    || rightX <= parentToolInsets.rightEdgeBottomInset) {
                // It will fit in the horizontalCenter
                return halfRootWidth - halfPanelWidth
            } else {
                // Anchor to left edge
                return parentToolInsets.leftEdgeBottomInset + _toolsMargin
            }
        }
    }

    //-- Virtual Joystick
    Loader {
        id: virtualJoystickMultiTouch
        z: QGroundControl.zOrderTopMost + 1
        width: parent.width - (_pipOverlay.width / 2)
        height: Math.min(parent.height * 0.25,
                         ScreenTools.defaultFontPixelWidth * 16)
        visible: _virtualJoystickEnabled
                 && !QGroundControl.videoManager.fullScreen
                 && !(_activeVehicle ? _activeVehicle.usingHighLatencyLink : false)
        anchors.bottom: parent.bottom
        anchors.bottomMargin: parentToolInsets.leftEdgeBottomInset
                              + ScreenTools.defaultFontPixelHeight * 2
        anchors.horizontalCenter: parent.horizontalCenter
        source: "qrc:/qml/VirtualJoystick.qml"
        active: _virtualJoystickEnabled
                && !(_activeVehicle ? _activeVehicle.usingHighLatencyLink : false)

        property bool autoCenterThrottle: QGroundControl.settingsManager.appSettings.virtualJoystickAutoCenterThrottle.rawValue

        property bool _virtualJoystickEnabled: QGroundControl.settingsManager.appSettings.virtualJoystick.rawValue
    }

    FlyViewToolStrip {
        id: toolStrip
        anchors.leftMargin: _toolsMargin + parentToolInsets.leftEdgeCenterInset
        anchors.topMargin: _toolsMargin + parentToolInsets.topEdgeLeftInset
        anchors.left: parent.left
        anchors.top: parent.top
        z: QGroundControl.zOrderWidgets
        maxHeight: parent.height - y - parentToolInsets.bottomEdgeLeftInset - _toolsMargin
        visible: !QGroundControl.videoManager.fullScreen

        onDisplayPreFlightChecklist: mainWindow.showPopupDialogFromComponent(
                                         preFlightChecklistPopup)

        property real leftInset: x + width
    }

    FlyViewAirspaceIndicator {
        anchors.top: parent.top
        anchors.topMargin: ScreenTools.defaultFontPixelHeight * 0.25
        anchors.horizontalCenter: parent.horizontalCenter
        z: QGroundControl.zOrderWidgets
        show: mapControl.pipState.state !== mapControl.pipState.pipState
    }

    VehicleWarnings {
        anchors.centerIn: parent
        z: QGroundControl.zOrderTopMost
    }

    MapScale {
        id: mapScale
        anchors.margins: _toolsMargin
        anchors.left: toolStrip.right
        anchors.top: parent.top
        mapControl: _mapControl
        buttonsOnLeft: false
        visible: !ScreenTools.isTinyScreen
                 && QGroundControl.corePlugin.options.flyView.showMapScale
                 && mapControl.pipState.state === mapControl.pipState.fullState

        property real centerInset: visible ? parent.height - y : 0
    }

    Component {
        id: preFlightChecklistPopup
        FlyViewPreFlightChecklistPopup {}
    }
}
