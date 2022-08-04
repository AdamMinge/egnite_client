import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Egnite.Rest 1.0

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/font/FontAwesome.ttf" }

    property var web_client: Client{
        baseUrl: "http://localhost/api/v1/"
        serializer: JsonSerializer{}
        headers: Headers{
            apiKey: "9RcIrfMn.yq1Ipn8L0mdTaqX7Ta6VFH2mIF5I5lvB"
            contentType: "application/json"
            accept: "application/json"
        }

        onErrorOccured: function(error) {

        }

        onSslErrorOccured: function(errors) {

        }
    }

    property var web_client_authenticator: SimpleJWTAuthenticator{
        client: web_client

        accessTokenLifetime: 60*60
        refreshTokenLifetime: 60*60*12

        routing {
            tokenCreate: "/token/"
            tokenRefresh: "/token/refresh/"
            tokenBlacklist: "/token/blacklist/"
        }

        onLoginSuccess: {

        }

        onLoginError: function(error) {

        }

        onLogoutSuccess: {

        }

        onLogoutError: function(error) {

        }
    }

    property var auth_window: AuthWindow{

    }

}
