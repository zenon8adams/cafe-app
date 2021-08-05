import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4
import CafeApp 1.0

Popup {
    id: addItemPopup

    visible: true
    width: 300
    height: 200
    opacity: 1
    anchors.centerIn: parent
    modal: true

    Rectangle {
        id: rectangle
        x: 0
        y: 0
        width: 300
        height: 200
        radius: 0
        border.width: 0
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#fbfbfb"
            }

            GradientStop {
                position: 1
                color: "#686565"
            }
        }

        RowLayout {
            x: 8
            y: 7
            width: 284
            height: 40
            spacing: 40
            RadioButton {
                text: "Food"
                onCheckedChanged: {
                    if(checked)
                        masterController.ui_masterViewController.ui_menuFormController.setMenuType(MenuFormController.Food)
                }
            }
            RadioButton {
                x: 203
                text: "Drink"
                onCheckedChanged: {
                    if(checked)
                        masterController.ui_masterViewController.ui_menuFormController.setMenuType(MenuFormController.Drink)
                }
            }
        }

        RowLayout {
            x: 9
            y: 52
            width: 283
            height: 40
            Layout.fillWidth: false
            Text {
                text: "Name: "
            }
            TextField {
                placeholderText: "Item Name..."
                onTextEdited: {
                    masterController.ui_masterViewController.ui_menuFormController.setMenuName(text)
                }
            }
        }

        RowLayout {
            x: 7
            y: 101
            width: 354
            height: 40
            Layout.fillWidth: false
            Text {
                text: "Price:"
            }
            TextField {
                id: priceTag
                x: 40
                Layout.fillHeight: false
                placeholderText: "Item Price..."
                validator: RegExpValidator {
                    regExp: /\d*\.*/
                }
                onTextEdited: {
                    masterController.ui_masterViewController.ui_menuFormController.setMenuPrice(text)
                }
                onTextChanged: {
                    errorString.visible = length == 0 || Number(text) === 0;
                }
            }
        }

        ColumnLayout {
            id: columnLayout
            x: 81
            y: 147
            width: 139
            height: 53

            Text {
                id: errorString
                x: 20
                y: 32
                text: "Price cannot be zero";
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                color: '#f00';
                visible: false;
            }

            GenericFlatButton {
                y: 2
                width: 60
                height: 27
                text: "Add"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                Layout.minimumWidth: 70
                onClicked: {
                    if(masterController.ui_masterViewController.ui_menuFormController.ui_menuType === MenuFormController.Drink
                            && Number(masterController.ui_masterViewController.ui_menuFormController.ui_menuPrice) === 0)
                    {
                    }else {
                        masterController.ui_masterViewController.updateMenu()
                        addItemPopup.close();
                    }


                }
            }

        }


    }


}

/*##^##
Designer {
    D{i:15;anchors_width:106;anchors_x:175}D{i:1;anchors_height:200;anchors_width:300;anchors_x:0;anchors_y:0}
}
##^##*/
