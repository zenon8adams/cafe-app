import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import assets 1.0

ApplicationWindow {
    id: adminWindow
    width: 400
    height: 570
    color: "#27363a"
    visible: true
    flags: Qt.Sheet | Qt.FramelessWindowHint
    title: "Cafe App"

    background: Rectangle {
        id: rectangle1
        gradient: Gradient {
            GradientStop {
                position: 0.00;
                color: "#f6eeeb";
            }
            GradientStop {
                position: 1.00;
                color: "#7a5b40"
            }
        }
        anchors.fill: parent
    }

    Image {
        id: grandImage
        source: "qrc:/images/slide13.jpg"
        anchors.fill: parent
        clip: true
        Rectangle {
            id: plate1
            x: 334
            width: 100
            height: 100
            color: "#7a5434"
            radius: 50
            anchors.right: parent.right
            anchors.rightMargin: -34
            anchors.top: parent.top
            anchors.topMargin: -22
        }

        Rectangle {
            id: plate2
            y: 5
            width: 100
            height: 100
            color: "#7a5434"
            radius: 50
            anchors.left: parent.left
            anchors.leftMargin: -34
            anchors.top: parent.top
            anchors.topMargin: -22
        }

        Rectangle {
            id: plate3
            x: 3
            y: -22
            width: 100
            height: 100
            color: "#d1b388"
            radius: 50
            anchors.bottom: parent.bottom
            anchors.bottomMargin: -22
            anchors.leftMargin: -34
            anchors.left: parent.left
        }

        Rectangle {
            id: plate4
            x: -26
            y: 492
            width: 100
            height: 100
            color: "#d1b388"
            radius: 50
            anchors.right: parent.right
            anchors.rightMargin: -34
            anchors.bottomMargin: -22
            anchors.bottom: parent.bottom
            Text {
                id: quitButton
                width: 40
                height: 44
                color: "#7e2407"
                text: "\uf2f5"
                anchors.verticalCenterOffset: -3
                anchors.horizontalCenterOffset: -10
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: Styler.faSolid
                font.pixelSize: 30
                MouseArea {
                    anchors.fill: parent
                    onClicked: Qt.quit()
                }
            }
        }

        TextField {
            id: activationField
            width: 252
            height: 40
            font.bold: true
            font.pointSize: 11
            font.family: "Times New Roman"
            placeholderText: "Enter Activation key"
            onTextChanged: activationErrorMsg.visible = false
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
            background: Rectangle {
                implicitHeight: activationField.height
                implicitWidth: activationField.width
                radius: 15
            }
            validator: RegExpValidator{
                regExp: /(\d*\w*_*)*/
            }
            Keys.onReturnPressed: {
                Keys.onEnterPressed(event)
                console.log("activationField - " + activationField.text)
                if(masterController.validLicense(activationField.text))
                {
                    control.enabled = true
                    activationField.visible = false
                }else activationErrorMsg.visible = true
            }
        }

        Item {
            id: rectangle
            y: 139
            height: 16
            anchors.left: parent.left
            anchors.right: parent.right
            Canvas {
                anchors.fill: parent
                contextType: "2d"
                onPaint: {
                    context.reset(0, 0);
                    context.moveTo(width/2-4, height/2);
                    context.lineTo(100, height/2);
                    context.moveTo(width/2+4, height/2);
                    context.lineTo(width-100, height/2);
                    context.strokeStyle = "#7a5434";
                    context.lineCap = "round";
                    context.lineWidth = 2;
                    context.stroke();
                }
            }
        }

        Text {
            id: activationErrorMsg
            x: 190
            width: 131
            height: 19
            visible: false
            color: "#fbfbfb"
            text: qsTr("Invalid Activation key!")
            anchors.top: activationField.bottom
            anchors.topMargin: 6
            font.bold: true
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: 11
        }
    }

    RoundButton {
        id: control
        enabled: false
        width: 40
        height: 40
        radius: width/2
        anchors.horizontalCenterOffset: 0
        anchors.bottomMargin: 11
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        background: Rectangle {
            radius: control.radius
            color: "#edece8"
        }
        onClicked: inputDialog.open()
        Dialog {
            id: inputDialog
            width: 200
            height: 290
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            parent: Overlay.overlay
            focus: true
            modal: true
            opacity: 0.75
            title: "Admin Signup"
            standardButtons: Dialog.Ok | Dialog.Cancel
            ColumnLayout {
                spacing: 20
                anchors.fill: parent
                Label {
                    elide: Label.ElideRight
                    text: "Please enter your details:"
                    Layout.fillWidth: true
                }
                TextField {
                    id: userID
                    focus: true
                    placeholderText: "User ID"
                    Layout.fillWidth: true
                    onTextChanged: errorMsg.visible = false
                }
                TextField {
                    id: passwordField
                    placeholderText: "Password"
                    echoMode: TextInput.Password
                    Layout.fillWidth: true
                    onTextChanged: errorMsg.visible = false
                    Keys.onReturnPressed: {
                        Keys.onEnterPressed(event)
                        if(text.length == 0)
                        {
                            errorMsg.visible = true
                            errorMsg.text = "Fields cannot be empty!"
                        }
                    }
                }
                Text {
                    id: errorMsg
                    visible: false
                    Layout.alignment: Qt.AlignHCenter
                    Layout.fillWidth: true
                    color: "#b71414"
                    font.bold: true
                }
            }
            onAccepted: {
                if(userID.text.length == 0 || passwordField.text.length == 0)
                {
                    errorMsg.visible = true
                    errorMsg.text = "Fields cannot be empty!"
                    inputDialog.open()
                }else
                {
                    masterController.ui_adminRegController.setUserID(userID.text)
                    masterController.ui_adminRegController.setPassword(passwordField.text)
                    masterController.registerAdmin()
                    adminWindow.close()
                    viewLoader.source = "qrc:/components/LoginWindow.qml"
                }
            }
            onRejected: errorMsg.visible = false
        }
    }

    Text {
        id: headerText
        x: 61
        y: 90
        width: 278
        height: 72
        color: "#f3dcd2"
        text: "CAFE APP"
        font.bold: true
        font.capitalization: Font.AllUppercase
        style: Text.Normal
        font.family: "Times New Roman"
        lineHeight: 1.5
        elide: Text.ElideMiddle
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 29
    }
    Loader {
        id: viewLoader
    }
}




/*##^##
Designer {
    D{i:11;anchors_height:100;anchors_width:100;anchors_x:"-26";anchors_y:"-22"}D{i:10;anchors_width:100;anchors_x:"-26";anchors_y:"-22"}
D{i:9;anchors_y:"-22"}D{i:16;anchors_height:100;anchors_width:100;anchors_x:"-26";anchors_y:"-22"}
D{i:15;anchors_width:400;anchors_x:0}D{i:17;anchors_y:311}D{i:19;anchors_height:100;anchors_width:100;anchors_x:"-26";anchors_y:"-22"}
D{i:18;anchors_height:100;anchors_width:100;anchors_x:"-26";anchors_y:"-22"}
}
##^##*/
