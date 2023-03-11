import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15


Item {
    id: root
    state: 'close'

    property alias models: navigation_model.models

    states: [
        State {
            name: 'open'
            PropertyChanges { 
                target: root
                width: 240 
            }

            PropertyChanges { 
                target: timer
                index: 0 
            }
        },
        State {
            name: 'close'
            PropertyChanges { 
                target: root
                width: 105 
            }

            PropertyChanges { 
                target: timer
                index: 0 
            }
        }
    ]

    transitions: [
        Transition {
            from: 'close'
            to: 'open'

            NumberAnimation {
                properties: 'width'
                duration: 300
                easing.type: Easing.OutCubic
            }

            ScriptAction {
                script: {
                    timer.start();
                }
            }
        },
        Transition {
            from: 'open'
            to: 'close'

            SequentialAnimation {

                ScriptAction {
                    script: {
                        timer.start();
                    }
                }

                PauseAnimation {
                    duration: 600
                }

                NumberAnimation {
                    properties: 'width'
                    duration: 300
                    easing.type: Easing.OutCubic
                }
            }
        }
    ]

    Timer {
        id: timer
        interval: 10
        property int index: 0

        onTriggered: {
            if (root.state == 'open')
                navigation_items.itemAt(index).state = 'left';
            else
                navigation_items.itemAt(index).state = 'middle';

            if (++index != navigation_items.model.count)
                timer.start();
        }
    }

    Rectangle {
        color: Material.rippleColor
        anchors.fill: parent

        ColumnLayout {
            width: parent.width

            anchors { 
                top: parent.top
                topMargin: 30 
            }

            Repeater {
                id: navigation_items

                model: ListModel {
                    id: navigation_model
                    
                    ListElement { 
                        icon: "\uefa2"
                        bottomMargin: 20
                        handler: function(){
                            root.state = (root.state == 'close') ? 'open' : 'close'
                        }
                    }

                    property list<ListModel> models
                    property var checked_index: null

                    onModelsChanged: {
                        let i = models.length - 1;
                        if (i < 0) return;

                        for (let j = 0; j < models[i].count; j++)
                        {
                            let element = models[i].get(j)
                            navigation_model.append({ 
                                "name": element.name,
                                "icon": element.icon,
                            })

                            let handler = element.handler
                            let index = navigation_model.count - 1
                            
                            if(element.checkable)
                            {   
                                handler = function(){
                                    if(checked_index !== null) 
                                        navigation_model.setProperty(checked_index, "checked", false)

                                    checked_index = index

                                    if(checked_index !== null) 
                                        navigation_model.setProperty(checked_index, "checked", true)

                                    element.handler()
                                }

                                if(checked_index === null)
                                    handler()
                            }

                            navigation_model.setProperty(index, "handler", handler)
                        }
                    }
                }

                delegate: NavigationItem {}
            }
        }
    }
}
