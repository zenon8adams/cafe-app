import QtQuick 2.11
import QtQuick.Controls 2.4

Item {
    property alias menuName: menuText.text
    signal removeButtonChecked()
    width: 160
    height: 170
    opacity: 1
    focus: true

    onFocusChanged: {
        if(activeFocus)
            console.log("Item Has Focus")
    }

    FocusScope {
        width: 150
        height: 150
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        focus: true
        Rectangle {
            id: background
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#27abc6"
                }

                GradientStop {
                    position: 1
                    color: "#1d2b30"
                }
            }
            border.width: 3
            border.color: "#56bbfb"
            FocusScope {
                anchors.top: parent.top
                anchors.topMargin: -8
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                focus: true
                Rectangle {
                    id: foreground
                    opacity: 1
                    anchors.fill: parent
                    border.color: "#069fbe"
                    gradient: Gradient {
                        GradientStop {
                            position: 0
                            color: "#4bc5f4"
                        }

                        GradientStop {
                            position: 1
                            color: "#084972"
                        }
                    }
                    border.width: 3

                    Text {
                        id: menuText
                        width: 105
                        height: 46
                        color: "#ffffff"
                        anchors.horizontalCenter: parent.horizontalCenter
                        style: Text.Raised
                        font.weight: Font.Black
                        font.capitalization: Font.AllUppercase
                        font.bold: true
                        font.family: "Times New Roman"
                        anchors.verticalCenter: parent.verticalCenter
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 15
                    }

                    CheckBox {
                          id: removeCheckBox
                          text: "Remove"
                          anchors.bottom: parent.bottom
                          anchors.bottomMargin: 8
                          anchors.left: parent.left
                          anchors.leftMargin: 8
                          indicator: Rectangle {
                              implicitWidth: 26
                              implicitHeight: 26
                              x: removeCheckBox.leftPadding
                              y: parent.height / 2 - height / 2
                              radius: 3
                              border.color: "#3068d8"

                              Rectangle {
                                  width: 14
                                  height: 14
                                  color: "#0c80f3"
                                  x: 6
                                  y: 6
                                  radius: 2
                                  visible: removeCheckBox.checked
                              }
                          }

                          contentItem: Text {
                              color: "#ffffff"
                              text: removeCheckBox.text
                              lineHeight: 1.3
                              font.bold: true
                              font.family: "Times New Roman"
                              opacity: enabled ? 1.0 : 0.3
                              verticalAlignment: Text.AlignVCenter
                              leftPadding: removeCheckBox.indicator.width + removeCheckBox.spacing
                          }
                          checkable: true
                          onCheckStateChanged: {
                              if(checked)
                                  removeButtonChecked()
                          }
                    }
                    Keys.onReturnPressed: Keys.enterPressed(event)
                    Keys.onEnterPressed: {
                        removeCheckBox.checked = !removeCheckBox.checked
                        event.accepted = true
                    }
                    onFocusChanged: {
                        if(activeFocus)
                            console.log("Foreground Has Focus")
                    }
                }
            }
            onFocusChanged: {
                if(activeFocus)
                    console.log("background Has Focus")
            }
        }
    }
}
