import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite.rest 1.0
import egnite.auth 1.0

import egnite.api 1.0


Window {
    id: main_window
    minimumWidth: 800
    minimumHeight: 600
    visible: true

    FontLoader { id: font_awesome; source: "qrc:/sniffer/FontAwesome.ttf" }

    EgniteClient {
        id: egnite_client

        replyDecorator {
            factories: [
                QmlLoggerReplyFactory {},
                QmlJwtAuthenticatorReplyFactory { authenticator: egnite_authenticator }
            ]
        }
    }

    QmlJwtAuthenticator {
        id: egnite_authenticator

        client: egnite_client
        path: "token"

        routing {
            obtain: ""
            refresh: "refresh"
            blacklist: "blacklist"
        }
    }

    MainMenu {
        id: main_menu
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

    MainStack {
        id: main_stack
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: main_menu.left
        anchors.right: parent.right
    }

    Authentication {
        id: authentication
        anchors.centerIn: parent
    }
}