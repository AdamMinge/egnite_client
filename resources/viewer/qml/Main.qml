import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite as Egnite

QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/font/FontAwesome.ttf" }

    property var rest_client: Egnite.WebClient{
        baseUrl: "127.0.0.1/"

        authenticator: Egnite.SimpleJWTAuthenticator {
            accessTokenLifetime: 60*60
            refreshTokenLifetime: 60*60*12
        }

        authorizator: Egnite.ApiKeyAuthorizator {
            apiKey: "99744f7e-b4e5-48d9-9d5e-fb4cc46c53a9"
        }
    }

    property var auth_window: AuthWindow{

    }

}
