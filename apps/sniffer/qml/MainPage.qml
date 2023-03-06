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
    }
}
