import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import egnite.controls 1.0


AnimatedHideableItem {
    id: root

    signal signUp(string email, string username, string password)
    signal cancel()

    function clearPage() {
        clearInputs()
        setError("")
    }
    function clearInputs() {
        username_field.text = ""
        email_field.text = ""
        password_field.text = ""
        confirm_password_field.text = ""
    }
    function setError(error) {
        error_message.text = error
    }

    Label {
        id: sign_up_label
        x: 0
        y: 0
        topPadding: 20
        width: parent.width
        height: parent.height / 3
        text: qsTr("Sign Up")
        font.pointSize: 30
        horizontalAlignment: Qt.AlignHCenter
    }

    ColumnLayout {
        width: parent.width
        anchors.top: sign_up_label.bottom
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
            id: email_field
            placeholderText: qsTr("Email")
            leftPadding: 30
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: "\uf0e0"
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

        TextField {
            id: confirm_password_field
            placeholderText: qsTr("Confirm Password")
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

        Text {
            id: error_message
            visible: text != ""
            font.pointSize: 10
            font.family: "FontAwesome"
            Layout.alignment: Qt.AlignCenter
            Material.foreground: Material.Red
            color: Material.foreground
        }

        Item {
            height: 20
        }

        Button {
            id: sign_up_button
            text: "Sign Up"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
            enabled: {
                let fields_not_empty = username_field.text && email_field.text && password_field.text && confirm_password_field.text
                let confirm_password_correct = password_field.text == confirm_password_field.text

                return fields_not_empty && confirm_password_correct
            }

            onClicked: root.signUp(
                email_field.text,
                username_field.text,
                password_field.text
            )
        }

        Button {
            id: cancel
            text: "Cancel"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            onClicked: root.cancel()
        }
    }
}
