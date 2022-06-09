import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

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
}
