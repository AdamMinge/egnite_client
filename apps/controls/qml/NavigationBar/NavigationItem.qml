import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15


Rectangle {
    id: root
    height: 40
    width: parent ? parent.width : null
    color: "transparent"

    property string name 
    property string icon 

    Text {
        anchors.centerIn: parent
        font.pointSize: 14
        text: name
        color: mouse_area.containsMouse ? Material.accent : Material.foreground
    }
    
    MouseArea {
        id: mouse_area
        anchors.fill: parent
        hoverEnabled: true
    }
}