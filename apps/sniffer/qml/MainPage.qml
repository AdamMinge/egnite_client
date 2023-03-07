import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import egnite.controls 1.0


AnimatedHideableItem {
    id: root
    anchors.fill: parent

    NavigationBar {
        width: 105

        anchors { 
            top: parent.top
            bottom: parent.bottom 
            left: parent.left
        }

        models: [
            ListModel {
                ListElement { 
                    name: "A1"
                    icon: "qrc:/egnite/controls/icons/A1.svg"
                    handler: function(){}
                }
            }
        ]
    }
}
