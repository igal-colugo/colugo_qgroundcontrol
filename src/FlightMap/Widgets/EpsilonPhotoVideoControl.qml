

/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/
import QtQuick 2.4
import QtPositioning 5.2
import QtQuick.Layouts 1.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0

import QGroundControl 1.0
import QGroundControl.ScreenTools 1.0
import QGroundControl.Controls 1.0
import QGroundControl.Palette 1.0
import QGroundControl.Vehicle 1.0
import QGroundControl.Controllers 1.0
import QGroundControl.FactSystem 1.0
import QGroundControl.FactControls 1.0

Rectangle {
    height: 300
    width: parent.width
    color: "#80000000"
    radius: _margins
    anchors.margins: 3

    property real _margins: ScreenTools.defaultFontPixelHeight / 2
    property real _butMargins: ScreenTools.defaultFontPixelHeight / 4

    /* nextvision panel resources */
    property int _panel_index: 0
    property int _panel_index_min: 0
    property int _panel_index_max: 4
    property real point_size: point_sizes[0]
    property var point_sizes: [ScreenTools.smallFontPointSize, ScreenTools.mediumFontPointSize, ScreenTools.largeFontPointSize]

    MouseArea {
        anchors.fill: parent
        enabled: true
        onClicked: {

        }
    }

    function updatePanelIndex(dir) {
        if (_panel_index + dir >= _panel_index_max)
            _panel_index = 0
        else if (_panel_index + dir < _panel_index_min)
            _panel_index = _panel_index_max - 1
        else
            _panel_index += dir
    }

    QGCPalette {
        id: qgcPal
        colorGroupEnabled: enabled
    }

    property var camQuickPages: ["qrc:/epsqml/EpsilonCamQuickPanel.qml"]
    property var pages: [camQuickPages[0], "qrc:/epsqml/EpsilonCamModePanel.qml", "qrc:/epsqml/EpsilonCamIRPanel.qml", "qrc:/epsqml/EpsilonCamRecPanel.qml", "qrc:/epsqml/EpsilonCamPosPanel.qml", "qrc:/epsqml/EpsilonCamFModePanel.qml", "qrc:/epsqml/EpsilonCamObjDetPanel.qml", "qrc:/epsqml/EpsilonCamMiscPanel.qml", "qrc:/epsqml/EpsilonCamStreamControlPanel.qml", "qrc:/epsqml/EpsilonCamVMDControlPanel.qml", "qrc:/epsqml/EpsilonCamTrackControlPanel.qml"]

    Loader {
        id: _presentedCamPanel
        anchors.fill: parent
        source: pages[_panel_index]
        width: parent.width
    }

    QGCColoredImage {
        id: buttonRigth
        anchors.margins: _margins
        anchors.top: parent.top
        anchors.right: parent.right
        source: "/res/buttonRight.svg"
        height: ScreenTools.isMobile ? ScreenTools.defaultFontPixelHeight
                                       * 2.0 : ScreenTools.defaultFontPixelHeight
        width: height * 1.2
        sourceSize.height: height * 1.2
        color: "white"
        fillMode: Image.PreserveAspectFit
        visible: true

        QGCMouseArea {
            fillItem: parent
            onClicked: updatePanelIndex(1)
        }
    }

    QGCColoredImage {
        id: buttonLeft
        anchors.margins: _margins
        anchors.top: parent.top
        anchors.left: parent.left
        source: "/res/buttonLeft.svg"
        height: ScreenTools.isMobile ? ScreenTools.defaultFontPixelHeight
                                       * 2.0 : ScreenTools.defaultFontPixelHeight
        width: height * 1.2
        sourceSize.height: height * 1.2
        color: "white"
        fillMode: Image.PreserveAspectFit
        visible: true

        QGCMouseArea {
            fillItem: parent
            onClicked: updatePanelIndex(-1)
        }
    }
}
