/****************************************************************************
 *
 * (c) 2009-2020 QGROUNDCONTROL PROJECT <http://www.qgroundcontrol.org>
 *
 * QGroundControl is licensed according to the terms in the file
 * COPYING.md in the root of the source code directory.
 *
 ****************************************************************************/

import QtQuick                      2.11
import QtQuick.Controls             2.4
import QtQuick.Dialogs              1.3
import QtQuick.Layouts              1.11

import QGroundControl               1.0
import QGroundControl.Palette       1.0
import QGroundControl.Controls      1.0
import QGroundControl.ScreenTools   1.0
import QGroundControl.Controllers   1.0
import QGroundControl.FactSystem    1.0
import QGroundControl.FactControls  1.0

Item {
    width:                  availableWidth
    height:                 (globals.activeVehicle.supportsJSButton ? buttonCol.height : flowColumn.height) + (ScreenTools.defaultFontPixelHeight * 2)
    Connections {
        target: _activeCamJoystick
        onRawButtonPressedChanged: {
            if (buttonActionRepeater.itemAt(index)) {
                buttonActionRepeater.itemAt(index).pressed = pressed
            }
            if (jsButtonActionRepeater.itemAt(index)) {
                jsButtonActionRepeater.itemAt(index).pressed = pressed
            }
        }
    }
    ColumnLayout {
        id:         flowColumn
        y:          ScreenTools.defaultFontPixelHeight / 2
        width:      parent.width
        spacing:    ScreenTools.defaultFontPixelHeight / 2
        Flow {
            id:                     buttonFlow
            Layout.preferredWidth:  parent.width
            spacing:                ScreenTools.defaultFontPixelWidth
            visible:                !globals.activeVehicle.supportsJSButton
            Repeater {
                id:             buttonActionRepeater
                model:          _activeCamJoystick ? Math.min(_activeCamJoystick.totalButtonCount, _maxButtons) : []
                Row {
                    spacing:    ScreenTools.defaultFontPixelWidth
                    property bool pressed
                    property var  currentAssignableAction: _activeCamJoystick ? _activeCamJoystick.assignableCamActions.get(buttonActionCombo.currentIndex) : null
                    Rectangle {
                        anchors.verticalCenter:     parent.verticalCenter
                        width:                      ScreenTools.defaultFontPixelHeight * 1.5
                        height:                     width
                        border.width:               1
                        border.color:               qgcPal.text
                        color:                      pressed ? qgcPal.buttonHighlight : qgcPal.button
                        QGCLabel {
                            anchors.fill:           parent
                            color:                  pressed ? qgcPal.buttonHighlightText : qgcPal.buttonText
                            horizontalAlignment:    Text.AlignHCenter
                            verticalAlignment:      Text.AlignVCenter
                            text:                   modelData
                        }
                    }
                    QGCComboBox {
                        id:                         buttonActionCombo
                        width:                      ScreenTools.defaultFontPixelWidth * 26
                        model:                      _activeCamJoystick ? _activeCamJoystick.assignableCamActionTitles : []

                        function _findCurrentButtonAction() {
                            if(_activeCamJoystick) {
                                var i = find(_activeCamJoystick.buttonCamActions[modelData])
                                if(i < 0) i = 0
                                currentIndex = i
                            }
                        }

                        Component.onCompleted:  _findCurrentButtonAction()
                        onModelChanged:         _findCurrentButtonAction()
                        onActivated:            _activeCamJoystick.setButtonCamAction(modelData, textAt(index))
                    }

                    Item {
                        width:                      ScreenTools.defaultFontPixelWidth * 2
                        height:                     1
                    }
                }
            }
        }
    }
    Column {
        id:         buttonCol
        width:      parent.width
        visible:    globals.activeVehicle.supportsJSButton
        spacing:    ScreenTools.defaultFontPixelHeight / 3
        Row {
            spacing: ScreenTools.defaultFontPixelWidth
            QGCLabel {
                horizontalAlignment:    Text.AlignHCenter
                width:                  ScreenTools.defaultFontPixelHeight * 1.5
                text:                   qsTr("#")
            }
            QGCLabel {
                width:                  ScreenTools.defaultFontPixelWidth * 15
                text:                   qsTr("Function: ")
            }
            QGCLabel {
                width:                  ScreenTools.defaultFontPixelWidth * 15
                text:                   qsTr("Shift Function: ")
            }
        }
        Repeater {
            id:     jsButtonActionRepeater
            model:  _activeCamJoystick ? Math.min(_activeCamJoystick.totalButtonCount, _maxButtons) : 0

            Row {
                spacing: ScreenTools.defaultFontPixelWidth
                visible: globals.activeVehicle.supportsJSButton

                property bool pressed

                Rectangle {
                    anchors.verticalCenter:     parent.verticalCenter
                    width:                      ScreenTools.defaultFontPixelHeight * 1.5
                    height:                     width
                    border.width:               1
                    border.color:               qgcPal.text
                    color:                      pressed ? qgcPal.buttonHighlight : qgcPal.button


                    QGCLabel {
                        anchors.fill:           parent
                        color:                  pressed ? qgcPal.buttonHighlightText : qgcPal.buttonText
                        horizontalAlignment:    Text.AlignHCenter
                        verticalAlignment:      Text.AlignVCenter
                        text:                   modelData
                    }
                }

                FactComboBox {
                    id:         mainJSButtonActionCombo
                    width:      ScreenTools.defaultFontPixelWidth * 15
                    fact:       controller.parameterExists(-1, "BTN"+index+"_FUNCTION") ? controller.getParameterFact(-1, "BTN" + index + "_FUNCTION") : null;
                    indexModel: false
                }

                FactComboBox {
                    id:         shiftJSButtonActionCombo
                    width:      ScreenTools.defaultFontPixelWidth * 15
                    fact:       controller.parameterExists(-1, "BTN"+index+"_SFUNCTION") ? controller.getParameterFact(-1, "BTN" + index + "_SFUNCTION") : null;
                    indexModel: false
                }
            }
        }
    }
}


