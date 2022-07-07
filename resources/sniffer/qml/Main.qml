import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite as Egnite

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/font/FontAwesome.ttf" }

    property var rest_client: Egnite.WebClient{
        base_url: "127.0.0.1/"
    }

    property var auth_window: AuthWindow{

    }

}
