import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import egnite.controls 1.0


AnimatedHideableItem {
    id: root
    anchors.fill: parent

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

        NotificationsPage {
            id: notifications_page
        }

        ProfilePage {
            id: profile_page
        }

        SettingsPage {
            id: settings_page
        }
    }  

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
                    icon: "\uef47"
                    handler: function(){ pages_view.currentItem = home_page }
                    checkable: true
                }
                ListElement { 
                    name: "Notifications"
                    icon: "\ueea3"
                    handler: function(){ pages_view.currentItem = notifications_page }
                    checkable: true
                }
                ListElement { 
                    name: "Profile"
                    icon: "\ued05"
                    handler: function(){ pages_view.currentItem = profile_page }
                    checkable: true
                }
                ListElement { 
                    name: "Settings"
                    icon: "\uef3a"
                    handler: function(){ pages_view.currentItem = settings_page }
                    checkable: true
                }
            }
        ]
    }
}
