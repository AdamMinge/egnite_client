import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: authentication
    width: 400
    height: 600

    StackView {
        id: stack_view
        focus: true
        anchors.fill: parent
    }

    Component.onCompleted: {
        showLoginPage()
    }

    function showLoginPage(uname) {
        stack_view.replace("qrc:/sniffer/Auth/LoginPage.qml")
    }

    function showSignUpPage(uname) {
        stack_view.replace("qrc:/sniffer/Auth/SignUpPage.qml")
    }
}
