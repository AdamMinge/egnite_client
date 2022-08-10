import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/font/FontAwesome.ttf" }

    property var auth_window: AuthWindow{

    }

}
