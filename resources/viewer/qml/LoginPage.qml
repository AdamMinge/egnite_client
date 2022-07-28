import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: login_page

    Rectangle {
        id: icon
        x: 0
        y: 0
        width: parent.width
        height: parent.height / 3
        color: "transparent"

        Text {
            text: qsTr("\uf169")
            anchors.centerIn: parent
            font.pointSize: 112
            font.family: "FontAwesome"
            color: Material.foreground
        }
    }

    ColumnLayout {
        width: parent.width
        anchors.top: icon.bottom
        spacing: 10

        TextField {
            id: username_field
            placeholderText: qsTr("Username")
            leftPadding: 30
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: "\uf007"
                font.pointSize: 14
                font.family: "FontAwesome"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                leftPadding: 5
                color: Material.foreground
            }
        }

        TextField {
            id: password_field
            placeholderText: qsTr("Password")
            leftPadding: 30
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
            echoMode: TextInput.Password

            Text {
                text: "\uf023"
                font.pointSize: 14
                font.family: "FontAwesome"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                leftPadding: 5
                color: Material.foreground
            }
        }

        Item {
            height: 20
        }

        Button {
            id: login_button
            text: "Log In"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
            enabled: {
                var fields_not_empty = username_field.text && password_field.text
                return fields_not_empty
            }

            onClicked: {
                web_client_authenticator.login(username_field.text, password_field.text)
            }
        }

        Button {
            id: signup_button
            text: "Sign Up"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                auth_window.show_sign_up()
            }
        }
    }
}
