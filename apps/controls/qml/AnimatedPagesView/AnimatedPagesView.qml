import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15


Item {
    id: root

    property int currentIndex: 0
    property Item currentItem: null

    default property Item newContent

    onNewContentChanged: {
        newContent.parent = root
        newContent.opacity = 0
        newContent.stateVisible = Qt.binding(bindingsClosure(root.children.length - 1))  
    }

    onCurrentIndexChanged: {
        for (let i = 0; i < root.children.length; i++) {
            if(root.children[i] === root.children[currentIndex]) {
                currentItem = root.children[currentIndex]
            }   
        }
    }

    onCurrentItemChanged: {
        for (let i = 0; i < root.children.length; i++) {
            if(root.children[i] === currentItem) {
                currentIndex = i
            }   
        }
    }

    function bindingsClosure(index) { return function() { return root.currentIndex === index } }
}