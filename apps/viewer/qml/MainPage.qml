import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Rectangle {
    id: root
    anchors.fill: parent

    MainMenu {
        id: main_menu
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }
}