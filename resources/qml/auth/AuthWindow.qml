import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Window {
    id: auth_window
    visible: true
    visibility: Window.Windowed
    width: 420
    height: 680

    StackView {
        id: stack_view
        focus: true
        anchors.fill: parent
    }

    Component.onCompleted: {
        show_login()
    }

    function show_login(){
        stack_view.replace("LoginPage.qml")
    }

    function show_sign_up(){
        stack_view.replace("SignUpPage.qml")
    }

    function show_reset_password(){
        stack_view.replace("ResetPasswordPage.qml")
    }
}
