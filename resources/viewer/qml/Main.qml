import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite as Egnite

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/font/FontAwesome.ttf" }

    property var web_client: Egnite.WebClient{
        baseUrl: "http://localhost/api/v1/"
        serializer: Egnite.JsonSerializer{}
    }

    property var web_client_authenticator: Egnite.SimpleJWTAuthenticator{
        webClient: web_client

        apiKey: "9RcIrfMn.yq1Ipn8L0mdTaqX7Ta6VFH2mIF5I5lvB"
        accessTokenLifetime: 60*60
        refreshTokenLifetime: 60*60*12

        routing {
            tokenCreate: "/token/"
            tokenRefresh: "/token/refresh/"
            tokenVerify: "/token/verify/"
        }
    }

    property var auth_window: AuthWindow{

    }

}
