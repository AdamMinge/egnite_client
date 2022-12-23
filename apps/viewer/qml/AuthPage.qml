import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material


Rectangle {
    id: root
    width: 400
    height: 600
    
    PagesView {
        id: pages_view
        anchors.fill: parent

        LoginPage {
            id: login_page
            anchors.fill: parent

            onLogin: function(username, password){
                egnite_authenticator.login(
                    username,
                    password
                )
            }

            onSignUp: {
                pages_view.currentItem = sign_up_page
            }
        }

        SignUpPage {
            id: sign_up_page
            anchors.fill: parent

            onSignUp: function(email, username, password){
                
            }

            onCancel: {
                pages_view.currentItem = login_page
            }
        }
    }
}
