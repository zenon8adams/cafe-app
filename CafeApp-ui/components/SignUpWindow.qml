import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

Window {
    id: signupWindow
    visible: true
    width: 640
    height: 480

    Image {
        id: backgroundImage
        anchors.fill: parent
        source: "qrc:/images/slide3.jpg"
    }
    GaussianBlur {
        id: backgroundImageBlurEffect
        anchors.fill: backgroundImage
        source: backgroundImage
        radius: 20
        samples: 2 * radius + 1
    }
    Loader { id: viewLoader }
    Button {
        id: signinButton
        x: 582
        width: 40
        height: 40
        text: "Sign In"
        anchors.top: parent.top
        anchors.topMargin: 8
        anchors.right: parent.right
        anchors.rightMargin: 18
        font.pixelSize: 8
        font.bold: false
        hoverEnabled: true
        onHoveredChanged: hovered ? signinButtonBackground.state = "hover" : signinButtonBackground.state = ""
        background: Rectangle {
            id: signinButtonBackground
            color: "lightgrey"
            border.width: 0
            radius: 20
            states: State {
                name: "hover"
                PropertyChanges {
                    target: signinButtonBackground
                    color: "transparent"
                    border.color: "lightgrey"
                    border.width: 1
                }
            }
        }
        onClicked: {
            signupWindow.close()
            viewLoader.source = "qrc:/components/LoginWindow.qml"
        }
    }

    GroupBox {
        width: 267
        height: 315
        anchors.verticalCenterOffset: 0
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        background: Rectangle {
            color: "white"
            opacity: 0.2
            border.width: 1
            border.color: "lightgrey"
        }

        GridLayout {
            anchors.bottomMargin: 0
            anchors.fill: parent
            columns: 3
            TextField {
                id: lastNameField
                Layout.fillWidth: true
                Layout.columnSpan: 3
                placeholderText: "Last Name"
            }
            TextField {
                id: firstNameField
                Layout.fillWidth: true
                Layout.columnSpan: 3
                placeholderText: "First Name"
            }
            TextField {
                id: idField
                Layout.fillWidth: true
                Layout.columnSpan: 3
                placeholderText: "Enter User ID"
                validator: RegExpValidator{
                    regExp: /^_+[\da-zA-Z]*|[a-zA-Z]+[_\d]*$/
                }
            }
            TextField {
                id: passwordField
                Layout.fillWidth: true
                Layout.columnSpan: 3
                placeholderText: "Enter Password"
                echoMode: TextInput.Password
            }
            TextField {
                id: confirmPasswordField
                Layout.fillWidth: true
                Layout.columnSpan: 3
                placeholderText: "Confirm Password"
                echoMode: TextInput.Password
                Layout.preferredWidth: 160
                Connections {
                    target: confirmPasswordField
                    onEditingFinished: {
                        if(passwordField.text != confirmPasswordField.text)
                        {
                            errorText.visible = true
                            errorText.text = "Password not match!"
                        }
                    }
                    onTextChanged: errorText.visible = false
                }
            }
            TextField {
                id: adminAcceptancePasswordField
                Layout.fillWidth: true
                Layout.columnSpan: 3
                placeholderText: "Admin Password Confirmation"
                echoMode: TextInput.Password
                Layout.preferredWidth: 160
                Keys.onReturnPressed: {
                    Keys.enterPressed(event);
                    signupButton.clicked();
                }
            }
            Item {
                Layout.columnSpan: 2
                Layout.fillWidth: true
                Label {
                    id: errorText
                    anchors.fill: parent
                    color: "#cd1414"
                    text: masterController.errorHandler().ui_errorMsg
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: 12
                    visible: false
                }
            }
            Button {
                id: signupButton
                height: 30
                text: "Sign Up"
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                hoverEnabled: true
                focus: true
                onHoveredChanged: hovered ? signupButtonBackground.state = "hover" : signupButtonBackground.state = ""
                background: Rectangle {
                    id: signupButtonBackground
                    color: "lightgrey"
                    border.width: 0
                    radius: 10
                    states: State {
                        name: "hover"
                        PropertyChanges {
                            target: signupButtonBackground
                            color: "transparent"
                            border.color: "lightgrey"
                            border.width: 1
                        }
                    }
                }
                enabled: !errorText.visible
                onClicked: {
                    masterController.ui_signUpFormController.setFirstName(firstNameField.text)
                    masterController.ui_signUpFormController.setLastName(lastNameField.text)
                    masterController.ui_signUpFormController.setUserID(idField.text)
                    masterController.ui_signUpFormController.setPassword(passwordField.text)
                    masterController.ui_signUpFormController.setAdminPassword(adminAcceptancePasswordField.text)
                    if(masterController.ui_validSignupDetails){
                        signupWindow.close()
                        viewLoader.source = "qrc:/views/StaffView.qml"
                    }else {
                        errorText.visible = true
                    }
                }
            }
        }
    }
}
