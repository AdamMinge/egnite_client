import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.1

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
        }

        TextField {
            id: password_field
            placeholderText: qsTr("Password")
            leftPadding: 30
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter
        }

        Item {
            height: 20
        }

        Button {
            id: login_button
            text: "Log In"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            onClicked: {

            }
        }

        Button {
            id: signup_button
            text: "Sign Up"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                show_sign_up()
            }
        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
