import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite.rest 1.0
import egnite.auth 1.0

import egnite.api 1.0


Window {
    id: root
    minimumWidth: 800
    minimumHeight: 600
    visible: true

    FontLoader { id: font_awesome; source: "qrc:/viewer/FontAwesome.ttf" }

    EgniteClient {
        id: egnite_client

        replyDecorator {
            factories: [
                QmlLoggerReplyFactory { logDetail: QmlLoggerReplyFactory.LogOnlyResult },
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

    PagesView {
        id: pages_view
        anchors.fill: parent

        AuthPage {
            id: auth_page
            anchors.centerIn: parent
        }

        MainPage {
            id: main_page
            anchors.fill: parent
        }
    }  
}