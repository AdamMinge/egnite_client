import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import egnite.controls 1.0


AnimatedHideableItem {
    id: root
    anchors.fill: parent

    NavigationBar {
        id: navigation_bar
        width: 105

        anchors { 
            top: parent.top
            bottom: parent.bottom 
            left: parent.left
        }

        models: [
            ListModel {
                ListElement { 
                    name: "Home"
                    icon: "\uf015"
                    handler: function(){ pages_view.currentItem = home_page }
                }
                ListElement { 
                    name: "Messages"
                    icon: "\uf4b6"
                    handler: function(){ pages_view.currentItem = messages_page }
                }
                ListElement { 
                    name: "Notifications"
                    icon: "\uf0f3"
                    handler: function(){ pages_view.currentItem = notifications_page }
                }
                ListElement { 
                    name: "Profile"
                    icon: "\uf007"
                    handler: function(){ pages_view.currentItem = profile_page }
                }
                ListElement { 
                    name: "Setting"
                    icon: "\uf013"
                    handler: function(){ pages_view.currentItem = setting_page }
                }
            }
        ]
    }

    AnimatedPagesView {
        id: pages_view
        currentItem: home_page

        anchors { 
            top: parent.top
            bottom: parent.bottom 
            left: navigation_bar.right
            right: parent.right
        }

        HomePage {
            id: home_page
        }

        MessagesPage {
            id: messages_page
        }

        NotificationsPage {
            id: notifications_page
        }

        ProfilePage {
            id: profile_page
        }

        SettingPage {
            id: setting_page
        }
    }  
}
