import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite.rest 1.0
import egnite.auth 1.0

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/viewer/FontAwesome.ttf" }

    property var rest_client: QmlClient {
        baseUrl: "http://localhost/api"
        version: "1"

        replyDecorator {
            factories: [
                QmlDebugReplyFactory{}
            ]
        }
    }

    property var rest_authenticator: QmlJwtAuthenticator {
        client: rest_client
        path: "token"

        routing {
            obtain: ""
            refresh: "refresh"
            blacklist: "blacklist"
        }
    }

    property var auth_window: AuthWindow{

    }

}
