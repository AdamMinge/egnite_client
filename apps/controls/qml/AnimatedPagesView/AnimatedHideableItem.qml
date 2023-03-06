import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

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

