import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: root
    property bool stateVisible: true
    visible: opacity > 0.0

    Item {
        states: [
            State { 
                when: stateVisible;
                PropertyChanges { target: root; opacity: 1.0 }
            },
            State { 
                when: !stateVisible;
                PropertyChanges { target: root; opacity: 0.0 }
            }
        ]

        transitions: [
            Transition {
                NumberAnimation { property: "opacity"; duration: 500}
            }
        ]
    }
}

