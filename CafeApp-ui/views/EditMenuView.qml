import QtQuick 2.11
import components 1.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import CafeApp 1.0

Item {
    id: item1

    Canvas {
        width: parent.width
        height: 4
        anchors.verticalCenterOffset: -50
        anchors.horizontalCenterOffset: 0
        anchors.centerIn: parent
        contextType: "2d"
        onPaint: {
            context.reset();
            context.fillStyle = "#b8b6b5";
            context.rect(4, 0, width-10, height);
            context.fill();
        }
        onWidthChanged: requestPaint();
        onHeightChanged: requestPaint();
    }

    GridView {
        id: foodView
        cellHeight: 200
        cellWidth: 200
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 16
        anchors.rightMargin: 16
        anchors.topMargin: 0
        interactive: true
        keyNavigationEnabled: true
        keyNavigationWraps: true
        focus: true
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        width: parent.width
        height: parent.height/3
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        model: masterController.ui_masterViewController.foods()
        delegate: CafeMenu {
            Layout.topMargin: 20
            Layout.leftMargin: 20
            menuName: display
            onRemoveButtonChecked: {
                masterController.ui_masterViewController.removeMenu(MasterViewController.Food, index)
            }
        }

        header: Text {
            text: "FOOD"
            font.pixelSize: 25
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.bold: true
            font.family: "Times New Roman"
            color: "#61605f"
        }

        highlight: Rectangle { color: "#71b3c2"; radius: 5 }
        highlightFollowsCurrentItem: true
        onFocusChanged: {
            if(activeFocus)
                console.log("GridView Has Focus")
        }
        Component.onCompleted: {
            //currentItem.forceActiveFocus()
        }
    }

    GridView {
        id: drinkView
        x: -6
        y: 218
        width: parent.width
        height: parent.height/3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 102
        model: masterController.ui_masterViewController.drinks()
        boundsBehavior: Flickable.StopAtBounds
        keyNavigationEnabled: true
        anchors.right: parent.right
        highlightFollowsCurrentItem: true
        cellHeight: 200
        cellWidth: 200
        keyNavigationWraps: true
        delegate: CafeMenu {
            menuName: display
            Layout.topMargin: 20
            Layout.leftMargin: 20
            onRemoveButtonChecked: masterController.ui_masterViewController.removeMenu(MasterViewController.Drink, index)
        }
        interactive: true
        clip: true
        header: Text {
            color: "#61605f"
            text: "DRINK"
            font.family: "Times New Roman"
            font.bold: true
            font.pixelSize: 25
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
        highlight: Rectangle {
            color: "#71b3c2"
            radius: 5
        }
        anchors.leftMargin: 16
        anchors.left: parent.left
        anchors.rightMargin: 16
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        focus: true
    }
    GenericRoundButton {
        id: addItemButton
        width: 52
        height: 52
        text: "+"
        hoverEnabled: false
        anchors.right: parent.right
        anchors.rightMargin: 49
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 17
        font.pointSize: 24
        font.bold: false
        display: AbstractButton.TextOnly
        background: Rectangle {
            id: editMenuButton
            color: "#17d1e2"
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#a0deec";
                }
                GradientStop {
                    position: 1.00;
                    color: "#72afc0";
                }
            }
            border.width: 0
            radius: (width/2)
        }
        states: [
                State {
                name: "rotate"
                PropertyChanges {
                    target: addItemButton
                    rotation: 90
                }
            }
        ]
        transitions: Transition {
            RotationAnimation { duration: 700; direction: RotationAnimation.Clockwise; }
        }
        onClicked: {
            addItemButton.state = "rotate"
            addItemView.open()
        }
    }
    AddItemPopup {
        id: addItemView
    }

}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
