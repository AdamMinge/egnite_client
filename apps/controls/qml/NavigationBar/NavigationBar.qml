import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.15
import QtQml.Models 2.1


Rectangle {
    id: root
    color: Material.background

    ColumnLayout {
        anchors.fill: parent

        Repeater {
            model: navigation_items
        }
    }

    ObjectModel {
        id: navigation_items
        
        NavigationItem { name: "Stockholm 1"; icon: "" }
        NavigationItem { name: "Stockholm 2"; icon: "" }
        NavigationItem { name: "Stockholm 3"; icon: "" }
        NavigationItem { name: "Stockholm 4"; icon: "" }
        NavigationItem { name: "Stockholm 5"; icon: "" }

        Item { Layout.fillWidth: true; Layout.fillHeight: true }

        NavigationItem { name: "Stockholm 6"; icon: "" }
        NavigationItem { name: "Stockholm 7"; icon: "" }
        NavigationItem { name: "Stockholm 8"; icon: "" }
    }
}
