import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import assets 1.0
import CafeApp 1.0

Window {

    id: loginView
    visible: true
    width: 500
    height: 400
    //flags: Qt.Sheet | Qt.WindowTitleHint | Qt.WindowMinimizeButtonHint | Qt.WindowCloseButtonHint
    minimumHeight: 400
    minimumWidth: 500

    Image {
        id: backgroundImage
        anchors.fill: parent
        source: Styler.imageName
    }
    GaussianBlur {
        id: backgroundImageBlurEffect
        anchors.fill: backgroundImage
        source: backgroundImage
        radius: 20
        samples: 2 * radius + 1
    }

    Loader { id: viewLoader }

    GroupBox {
        id: frame
        width: 270
        height: 160
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 2
        background: Rectangle {
            id: groupBackground
            opacity: 0.5
            radius: 5
        }

        GridLayout {
            id: formHolder
            columns: 3
            anchors.fill: parent
            TextField {
                id: idField
                Layout.fillWidth: true
                focus: true
                placeholderText: "Enter User ID"
                Layout.columnSpan: 3
//                onFocusChanged:
//                    if(focus)
//                        errorText.visible = false
                Keys.onReturnPressed: {
                    Keys.enterPressed(event);
                    loginButton.clicked();
                }

                onTextChanged: errorText.visible = false
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
                onTextChanged: errorText.visible = false
                Keys.onReturnPressed: {
                    Keys.enterPressed(event);
                    loginButton.clicked()
                }
            }
            Item {
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Label {
                    id: errorText
                    visible: false
                    anchors.centerIn: parent
                    color: "#e00505"
                    text: masterController.errorHandler().ui_errorMsg
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 12
                }
            }
            Button {
                id: loginButton
                focus: true
                Layout.preferredHeight: loginButton.implicitHeight
                Layout.preferredWidth: loginButton.implicitWidth + 10
                text: qsTr("Login")
                Layout.alignment: Qt.AlignRight | Qt.AlignBottom
                Layout.fillWidth: false
                Layout.columnSpan: 1
                hoverEnabled: true
                onHoveredChanged: hovered ? state = "hover" : state = ""
                background: Rectangle {
                    id: background
                    color: "#d9d7d6"
                    border.width: 0
                }
                Keys.onPressed: {
                    if(event.key === Qt.Key_Return || event.key === Qt.Key_Enter)
                        clicked()
                }

                onClicked: {
                    masterController.ui_logInFormController.setUserID(idField.text)
                    masterController.ui_logInFormController.setPassword(passwordField.text)
                    if(users.text == "\uf007")
                        masterController.ui_logInFormController.setUserType(LogInFormController.Staff)
                    else  masterController.ui_logInFormController.setUserType(LogInFormController.Admin)
                    if(masterController.ui_validLoginDetails)
                    {
                        if(users.text == "\uf007"){
                            loginView.close()
                            viewLoader.source = Qt.resolvedUrl("qrc:/views/StaffView.qml")
                        }else {
                            loginView.close()
                            viewLoader.source = Qt.resolvedUrl("qrc:/views/AdminView.qml")
                        }
                    }else {
                        errorText.visible = true
                    }
                }

                states: State {
                    name: "hover"
                    PropertyChanges {
                        target: background
                        color: "transparent"
                        border.width: 1
                        border.color: "#d9d7d6"
                    }
                }
                Connections {
                    target: loginButton
                    onClicked: masterController.ui_logInFormController.readyToValidate()
                }
            }
        }
    }
    Text {
        id: users
        width: 50
        height: 50
        color: "#89c1e1"
        text: "\uf007"
        anchors.top: parent.top
        anchors.topMargin: 19
        anchors.right: parent.right
        anchors.rightMargin: 17
        font.family: Styler.faSolid
        font.pixelSize: 25
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        MouseArea {
            anchors.fill: parent
            onClicked: {
                idField.clear()
                passwordField.clear()
                errorText.visible = false
                if(users.text == "\uf007"){
                    users.text = "\uf4fe"
                }else {
                    users.text = "\uf007"
                }
            }
            hoverEnabled: true
            onEntered: users.state = "hover"
            onExited: users.state = ""
        }
        states: State {
            name: "hover"
            PropertyChanges {
                target: users
                color: "#0000ff"
            }
        }
    }

    GenericRoundButton {
        id: signupButton
        x: 377
        width: 50
        height: 50
        text: "Sign Up"
        anchors.right: users.left
        anchors.rightMargin: 6
        anchors.verticalCenterOffset: 0
        font.pointSize: 6
        font.family: "Times New Roman"
        anchors.verticalCenter: users.verticalCenter
        anchors.top: parent.top
        anchors.topMargin: 19
        spacing: 8
        onClicked: {
            loginView.close()
            viewLoader.source = "qrc:/components/SignUpWindow.qml"
        }
    }
}
