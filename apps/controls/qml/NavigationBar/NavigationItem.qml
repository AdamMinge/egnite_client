import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt5Compat.GraphicalEffects 6.0

import egnite.controls 1.0


Rectangle {
    id: root
    radius: 10
    state: 'middle'
    color: "transparent"

    Layout.preferredWidth: 50
    Layout.preferredHeight: 50
    Layout.alignment: Qt.AlignLeft
    Layout.bottomMargin: model.bottomMargin

    Behavior on color {
        ColorAnimation {
            duration: 200
        }
    }

    states: [
        State {
            name: 'left'

            PropertyChanges {
                target: root
                Layout.leftMargin: 10
                Layout.preferredWidth: 240
            }

            PropertyChanges {
                target: title
                opacity: 1
            }
        },
        State {
            name: 'middle'

            PropertyChanges {
                target: root
                Layout.leftMargin: Math.ceil((parent.width - 50) / 2)
                Layout.preferredWidth: 50
            }

            PropertyChanges {
                target: title
                opacity: 0
            }
        }
    ]

    transitions: [
        Transition {
            from: 'middle'
            to: 'left'

            NumberAnimation {
                properties: 'Layout.leftMargin, Layout.preferredWidth, opacity'
                duration: 300
                easing.type: Easing.InOutSine
            }
        },
        Transition {
            from: 'left'
            to: 'middle'

            NumberAnimation {
                properties: 'Layout.leftMargin, Layout.preferredWidth, opacity'
                duration: 300
                easing.type: Easing.InOutSine
            }
        }
    ]

    MouseArea {
        id: mouse_area

        hoverEnabled: true
        anchors.fill: parent

        onClicked: model.handler()
    }

    Rectangle {
        width: 5
        height: parent.height
        color: Material.accent 
        visible: model.checked
        
        anchors { 
            left: parent.left
            leftMargin: -root.Layout.leftMargin
        }
    }

    Text {
        id: icon

        text: model.icon
        font.pointSize: 18
        font.family: Fonts.icofont.font.family
        color: mouse_area.containsMouse || model.checked ? Material.foreground : Material.hintTextColor

        anchors { 
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 10
        }
    }

    Text {
        id: title

        text: model.name
        font.pointSize: 14
        color: mouse_area.containsMouse || model.checked ? Material.foreground : Material.hintTextColor

        anchors { 
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 55 
        }
    }
}