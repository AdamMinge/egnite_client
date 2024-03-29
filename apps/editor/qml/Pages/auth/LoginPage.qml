import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import egnite.controls 1.0


AnimatedHideableItem {
    id: root

    signal login(string email, string password)
    signal signUp()

    function clearPage() {
        clearInputs()
        setError("")
    }
    function clearInputs() {
        email_field.text = ""
        password_field.text = ""
    }
    function setError(error) {
        error_message.text = error
    }

    Rectangle {
        id: icon
        x: 0
        y: 0
        width: parent.width
        height: parent.height / 3
        color: "transparent"

        Text {
            text: "\ue929"
            anchors.centerIn: parent
            font.pointSize: 112
            font.family: Fonts.icofont.font.family
            color: Material.foreground
        }
    }

    ColumnLayout {
        width: parent.width
        anchors.top: icon.bottom
        spacing: 10

        TextField {
            id: email_field
            placeholderText: qsTr("Email")
            leftPadding: 30
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: "\uec67"
                font.pointSize: 14
                font.family: Fonts.icofont.font.family
                leftPadding: 5
                color: Material.foreground

                anchors { 
                    left: parent.left
                    verticalCenter: parent.verticalCenter 
                    horizontalCenter: parent.horizontalCenter
                }
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
                text: "\uf01a"
                font.pointSize: 14
                font.family: Fonts.icofont.font.family
                leftPadding: 5
                color: Material.foreground

                anchors { 
                    left: parent.left
                    verticalCenter: parent.verticalCenter 
                    horizontalCenter: parent.horizontalCenter
                }
            }
        }

        Text {
            id: error_message
            visible: text != ""
            font.pointSize: 10
            Layout.alignment: Qt.AlignCenter
            Material.foreground: Material.Red
            color: Material.foreground
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
                let fields_not_empty = email_field.text && password_field.text
                return fields_not_empty
            }

            onClicked: root.login(
                email_field.text, 
                password_field.text
            )
        }

        Button {
            id: signup_button
            text: "Sign Up"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            onClicked: root.signUp()
        }
    }
}
