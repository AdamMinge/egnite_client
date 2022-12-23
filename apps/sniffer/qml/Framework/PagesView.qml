import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


Item {
    id: root

    property int currentIndex: 0
    property Item currentItem: null

    default property Item newContent

    onNewContentChanged: {
        newContent.parent = root
        newContent.visible = Qt.binding(bindingsClosure(root.children.length - 1))
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