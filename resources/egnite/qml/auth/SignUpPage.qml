import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Page {
    id: sign_up_page

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
                leftPadding: 10
            }
        }

        TextField {
            id: password_field
            placeholderText: qsTr("Password")
            leftPadding: 30
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: "\uf023"
                font.pointSize: 14
                font.family: "FontAwesome"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 10
            }
        }

        TextField {
            id: confirm_password_field
            placeholderText: qsTr("Confirm Password")
            leftPadding: 30
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            Text {
                text: "\uf023"
                font.pointSize: 14
                font.family: "FontAwesome"
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                leftPadding: 10
            }
        }

        Item {
            height: 20
        }

        Button {
            id: sign_up_button
            text: "Sign Up"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            onClicked: {

            }
        }

        Button {
            id: cancel
            text: "Cancel"
            Layout.preferredWidth: parent.width - 20
            Layout.alignment: Qt.AlignHCenter

            onClicked: {
                show_login()
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
