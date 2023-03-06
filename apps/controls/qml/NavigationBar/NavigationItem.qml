import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Rectangle {
    id: root
    radius: 10
    color: mouse_area.containsMouse ? Material.background.darker() : "transparent"
    state: 'middle'

    Layout.preferredWidth: 50
    Layout.preferredHeight: 50
    Layout.alignment: Qt.AlignLeft
    Layout.topMargin: model.index === 1 ? 20 : 0

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
                Layout.preferredWidth: model.index !== 0 ? 240 : 50
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

    Image {
        id: icon

        source: model.icon
        sourceSize: Qt.size(30, 30)

        anchors { 
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 10
        }
    }

    Text {
        id: title

        text: model.index === 0 ? '' : model.name
        color: Material.foreground
        font.pointSize: 14

        anchors { 
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 55 
        }
    }
}