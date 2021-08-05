import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import components 1.0
import assets 1.0
import CafeApp 1.0

Window {
    id: staffView

    visible: true
    width: 800
    height: 600
    flags: /*Qt.ToolTip*/Qt.Sheet | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    color: "#ffffff"
    minimumHeight: 480
    minimumWidth: 800

    Loader { id: viewLoader }

    Image {
        id: backgroundImage
        height: 480
        source: "qrc:/images/slide6.jpg"
        anchors.fill: parent
    }
    function recieptModel()
    {
        return masterController.ui_staffViewController.recieptModel()
    }

    ItemSelection {
        id: food
        width: 270
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 78
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 5
        headerText: "FOOD"
        minControl: 0
        maxControl: 100000
        initialValue: 0
        steps: 10
        repeaterModel: masterController.ui_staffViewController.foodModel()
        onRemoveItemFromModel: {
            staffView.recieptModel().removeRow(staffView.recieptModel().rowOf(data));
            masterController.ui_staffViewController.computeTotal();
        }
        onAddItemToModel: {
            var row = staffView.recieptModel().rowCount()-1;
            staffView.recieptModel().insertRow(row);
            staffView.recieptModel().setData(staffView.recieptModel().index(row, 0), [data, "0"]);
        }
        onSetUnitToValue: {
            staffView.recieptModel().removeRow(staffView.recieptModel().rowOf(data))
            var row = staffView.recieptModel().rowCount()-1;
            staffView.recieptModel().insertRow(row);
            staffView.recieptModel().setData(staffView.recieptModel().index(row, 0), [data, value]);
            var total = masterController.ui_staffViewController.computeTotal();
            staffView.recieptModel().setData(staffView.recieptModel().index(row+1, 0), ["Total", total]);
        }
    }
    ItemSelection {
        id: drinks
        x: 383
        width: 270
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 78
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.rightMargin: 7
        headerText: "DRINKS"
        minControl: 0
        maxControl: 99
        initialValue: 0
        steps: 1
        repeaterModel: masterController.ui_staffViewController.drinkModel()
        onRemoveItemFromModel: {
            staffView.recieptModel().removeRow(staffView.recieptModel().rowOf(data));
        }
        onAddItemToModel: {
            var row = staffView.recieptModel().rowCount()-1;
            staffView.recieptModel().insertRow(row);
            staffView.recieptModel().setData(staffView.recieptModel().index(row, 0), [data, "0"]);
        }
        onSetUnitToValue: {
            staffView.recieptModel().removeRow(staffView.recieptModel().rowOf(data));
            var row = staffView.recieptModel().rowCount()-1;
            staffView.recieptModel().insertRow(row);
            var pos = masterController.ui_staffViewController.drinkModel().rowOf(data);
            var multiplier = parseInt(drinks.drinkModel().data(drinks.drinkModel().index(pos, 0))[1]);
            if(isNaN(multiplier))
                multiplier = 0;
            console.log("value, multiplier - ", value, multiplier);
            staffView.recieptModel().setData(staffView.recieptModel().index(row, 0), [data, value * multiplier]);
            var total = masterController.ui_staffViewController.computeTotal();
            staffView.recieptModel().setData(staffView.recieptModel().index(row+1, 0), ["Total", total]);
        }
        function drinkModel()
        {
           return  masterController.ui_staffViewController.drinkModel()
        }
    }

    TableView {
        id: menuView
        focus: true
        width: 300
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 78
        anchors.top: parent.top
        anchors.topMargin: 22
        anchors.horizontalCenter: parent.horizontalCenter
        flickableDirection: Flickable.VerticalFlick
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
        model: masterController.ui_staffViewController.recieptModel()
        delegate: Rectangle {
            opacity: 0.65
            implicitWidth: menuView.width/2
            implicitHeight: 50
            border.color: "#bdbbba"
            Text {
                text: tabledata
                font.pointSize: (heading == true || footer == true) ? 22 : 12
                font.bold: (heading == true || footer == true)
                anchors.centerIn: parent
            }
        }
        ScrollBar.vertical: ScrollBar{}
    }

    RoundButton {
        id: printButton
        width: 52
        height: width
        hoverEnabled: true
        onHoveredChanged: hovered ? state = "hover" : state = ""
        text: "Print"
        anchors.rightMargin: 56
        anchors.bottomMargin: 15
        anchors.right: parent.right
        font.pointSize: 9
        checked: true
        anchors.bottom: parent.bottom
        background: Rectangle {
            radius: (width/2)
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#3ab9ea";
                }
                GradientStop {
                    position: 1.00;
                    color: "#4179a4";
                }
            }
        }
        states: State {
            name: "hover"
            PropertyChanges {
                target: printButton
                scale: 1.1
            }
        }
        onClicked: masterController.ui_staffViewController.updateDailyEntries()
    }

    Text {
        id: logoutButton
        width: 52
        height: 52
        text: "\uf2f5"
        font.family: Styler.faSolid
        anchors.left: parent.left
        anchors.leftMargin: 8
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        font.pixelSize: 40
        MouseArea {
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent
            hoverEnabled: true
            onEntered: logoutButton.state = "hover"
            onExited: logoutButton.state = ""
            onClicked: {
                staffView.close()
                viewLoader.source = "qrc:/components/LoginWindow.qml"
            }
        }
        states: State {
            name: "hover"
            PropertyChanges {
                target: logoutButton
                color:"#f10000"

            }
        }
    }
    Component.onCompleted: {
        staffView.showMaximized()
    }
}
