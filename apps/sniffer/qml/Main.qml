import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite.rest 1.0

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/sniffer/FontAwesome.ttf" }

    property var rest_client: QmlClient {
        baseUrl: "http://localhost/api"
        version: "1"

        api: [

        ]

    }

    property var auth_window: AuthWindow{

    }

}
