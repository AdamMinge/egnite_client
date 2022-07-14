import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite as Egnite

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/font/FontAwesome.ttf" }

    property var web_client: Egnite.WebClient{
        baseUrl: "127.0.0.1/"
    }

    property var web_client_authenticator: Egnite.SimpleJWTAuthenticator{
        webClient: web_client

        apiKey: "99744f7e-b4e5-48d9-9d5e-fb4cc46c53a9"
        accessTokenLifetime: 60*60
        refreshTokenLifetime: 60*60*12

        routing {
            tokenCreate: "/token"
            tokenRefresh: "/token/refresh"
            tokenVerify: "/token/verify"
        }
    }

    property var auth_window: AuthWindow{

    }

}
