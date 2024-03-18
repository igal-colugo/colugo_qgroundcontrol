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
    id: linkStatusSettingsMode
    anchors.fill: parent
    visible: true

    GridLayout {
        id: linkStatusMainGrid

        columns: 2
        rows: 1
        anchors.fill: parent
        anchors.margins: 3
        columnSpacing: 2
        rowSpacing: 2

        QGCGroupBox {
            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                        / (parent.rows)
            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing))
                                          / (parent.columns)
            id: gdtLinkStatusGroupBox
            height: cellRowHeight * Layout.rowSpan
            width: cellColumnWidth * Layout.columnSpan
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            title: qsTr("GDT LINK STATUS")
            label: Text {
                color: "#808080"
                text: gdtLinkStatusGroupBox.title
            }

            GridLayout {
                id: gdtlinkStatusGrid

                columns: 1
                rows: 3
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2
            }
        }

        QGCGroupBox {
            property int cellRowHeight: (parent.height - (parent.rows * parent.rowSpacing))
                                        / (parent.rows)
            property int cellColumnWidth: (parent.width - (parent.columns * parent.columnSpacing))
                                          / (parent.columns)
            id: adtLinkStatusGroupBox
            height: cellRowHeight * Layout.rowSpan
            width: cellColumnWidth * Layout.columnSpan
            Layout.rowSpan: 1
            Layout.columnSpan: 1
            Layout.preferredHeight: cellRowHeight * Layout.rowSpan
            Layout.preferredWidth: cellColumnWidth * Layout.columnSpan
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.alignment: Qt.AlignCenter
            title: qsTr("ADT LINK STATUS")
            label: Text {
                color: "#808080"
                text: adtLinkStatusGroupBox.title
            }

            GridLayout {
                id: adtLinkStatusGrid

                columns: 1
                rows: 3
                anchors.fill: parent
                anchors.margins: 3
                columnSpacing: 2
                rowSpacing: 2
            }
        }
    }
}
