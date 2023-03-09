import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import egnite.rest 1.0
import egnite.auth 1.0

import egnite.api 1.0
import egnite.controls 1.0


Window {
    id: root
    minimumWidth: 800
    minimumHeight: 700
    visible: true

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
        path: "auth"

        routing {
            obtain: "login"
            refresh: "login/refresh"
            blacklist: "login/blacklist"
        }

        property bool isLogin: egnite_authenticator.accessToken != ""
        property bool isLogout: !isLogin
    }

    RegistrationApi {
        id: registration_api
        client: egnite_client
    }

    AnimatedPagesView {
        id: pages_view
        anchors.fill: parent
        currentItem: egnite_authenticator.isLogout ? auth_page : main_page

        AuthPage {
            id: auth_page
        }

        MainPage {
            id: main_page
        }
    }  
}