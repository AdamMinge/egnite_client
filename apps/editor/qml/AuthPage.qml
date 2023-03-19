import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import egnite.controls 1.0


AnimatedHideableItem {
    id: root
    width: 400
    height: 600
    anchors.centerIn: parent

    AnimatedPagesView {
        id: pages_view
        anchors.fill: parent

        LoginPage {
            id: login_page
            anchors.fill: parent
            property bool duringLogging: false

            states: [
                State { 
                    when: login_page.duringLogging;
                    PropertyChanges { target: login_page; enabled: false }
                }
            ]

            onLogin: function(email, password){
                login_page.duringLogging = true
                egnite_authenticator.login(
                    email,
                    password
                ) 
            }

            onSignUp: {
                pages_view.currentItem = sign_up_page
            }

            Connections {
                target: egnite_authenticator
 
                function onLoginSucceeded() { 
                    login_page.duringLogging = false
                    login_page.setError("")
                }

                function onLoginFailed(detail) { 
                    login_page.duringLogging = false
                    login_page.setError(detail)
                    login_page.clearInputs()
                }
            }
        }

        SignUpPage {
            id: sign_up_page
            anchors.fill: parent
            property bool duringSignUp: false

            function onRegisterSucceeded(){
                sign_up_page.duringSignUp = false
                pages_view.currentItem = login_page
            }
            function onRegisterFailed(error){
                sign_up_page.duringSignUp = false
                sign_up_page.setError(error)
                sign_up_page.clearInputs()
            }

            states: [
                State { 
                    when: sign_up_page.duringSignUp;
                    PropertyChanges { target: sign_up_page; enabled: false }
                }
            ]

            onSignUp: function(email, username, password){
                sign_up_page.duringSignUp = true
                var reply = registration_api.registerUser(
                    {
                        email: email,
                        username: username,
                        password: password,
                    }
                )

                reply.addSucceededHandler(onRegisterSucceeded)
                reply.addFailedHandler(function(http_code, data){
                    var errors = []
                    if(data.email) errors = [...errors, "Email: " + data.email[0]]  
                    if(data.password) errors = [...errors, "Password: " + data.password[0]]       

                   onRegisterFailed(errors.join("\n"))
                })
                reply.addErrorHandler(function(error_str, error_type){
                    onRegisterFailed(error_str)
                })
            }

            onCancel: {
                pages_view.currentItem = login_page
            }
        }
    }

    Connections {
        target: pages_view
        function onCurrentItemChanged() { pages_view.currentItem.clearPage() }
    }
}
