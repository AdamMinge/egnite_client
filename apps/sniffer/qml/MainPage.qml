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
                    icon: "qrc:/sniffer/icon/home.svg"
                    handler: function(){ pages_view.currentItem = home_page }
                }
                ListElement { 
                    name: "Messages"
                    icon: "qrc:/sniffer/icon/messages.svg"
                    handler: function(){ pages_view.currentItem = messages_page }
                }
                ListElement { 
                    name: "Notifications"
                    icon: "qrc:/sniffer/icon/notifications.svg"
                    handler: function(){ pages_view.currentItem = notifications_page }
                }
                ListElement { 
                    name: "Profile"
                    icon: "qrc:/sniffer/icon/profile.svg"
                    handler: function(){ pages_view.currentItem = profile_page }
                }
                ListElement { 
                    name: "Setting"
                    icon: "qrc:/sniffer/icon/setting.svg"
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
