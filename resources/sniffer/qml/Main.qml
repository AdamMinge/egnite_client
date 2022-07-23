import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import Egnite.Web 1.0

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/font/FontAwesome.ttf" }

    property var client: Client{
        baseUrl: "http://localhost/api/v1/"
        serializer: JsonSerializer{}

        onErrorOccured: {

        }

        onSslErrorOccured: {

        }
    }

    property var web_client_authenticator: SimpleJWTAuthenticator{
        client: client

        apiKey: "9RcIrfMn.yq1Ipn8L0mdTaqX7Ta6VFH2mIF5I5lvB"
        accessTokenLifetime: 60*60
        refreshTokenLifetime: 60*60*12

        routing {
            tokenCreate: "/token/"
            tokenRefresh: "/token/refresh/"
            tokenBlacklist: "/token/blacklist/"
        }
    }

    property var auth_window: AuthWindow{

    }

}
