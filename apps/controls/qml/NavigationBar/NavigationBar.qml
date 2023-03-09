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
                        name: "Menu"
                        icon: "\uefa2"
                        handler: function(){
                            root.state = (root.state == 'close') ? 'open' : 'close'
                        }
                    }

                    default property list<ListModel> models

                    onModelsChanged: {
                        var i = models.length - 1;
                        if (i < 0) return;

                        for (var j = 0; j < models[i].count; j++)
                        {
                            var element = models[i].get(j)
                            navigation_model.append({ 
                                "name": element.name,
                                "icon": element.icon
                            })

                            let handler = element.handler
                            let index = navigation_model.count - 1

                            if(element.checkable)
                            {   
                                handler = function(){
                                    navigation_items.itemAt(index).checked = true
                                    for (var j = 0; j < navigation_items.model.count; j++)
                                    {
                                        if(j === index) continue
                                        navigation_items.itemAt(j).checked = false
                                    }
                                        

                                    element.handler()
                                }
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
