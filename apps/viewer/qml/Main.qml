import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import egnite.rest 1.0
import egnite.auth 1.0

import egnite.schema 1.0


QtObject {

    readonly property FontLoader font_awesome: FontLoader { id: font_awesome; source: "qrc:/viewer/FontAwesome.ttf" }

    property var egnite_client: EgniteClient {
        replyDecorator {
            factories: [
                QmlLoggerReplyFactory {},
                QmlJwtAuthenticatorReplyFactory { authenticator: egnite_authenticator }
            ]
        }
    }

    property var egnite_authenticator: QmlJwtAuthenticator {
        client: egnite_client
        path: "token"

        routing {
            obtain: ""
            refresh: "refresh"
            blacklist: "blacklist"
        }

        onLoginSucceeded: function(){
            
        }

        onLogoutSucceeded: function(){
                    
        }

        onRefreshSucceeded: function(){
                    
        }

        onLoginFailed: function(detail){
                    
        }

        onLogoutFailed: function(detail){
                    
        }

        onRefreshFailed: function(detail){
                    
        }
    }

    property var auth_window: AuthWindow{
        
    }

}
