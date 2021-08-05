import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import components 1.0
import CafeApp 1.0

// Deprecated!

Item {
    property int actionIndex
    GridView {
        id: editStaffView
        anchors.fill: parent
        property int idealCellWidth: 200
        property int idealCellHeight: 200
        boundsBehavior: Flickable.StopAtBounds
        keyNavigationEnabled: true
        highlightFollowsCurrentItem: true
        cellHeight: idealCellHeight
        cellWidth: width / Math.floor(width / idealCellWidth)
        keyNavigationWraps: true
        model: masterController.ui_masterViewController.staffs()
        delegate:  Staff {
             height: GridView.view.cellHeight
             width: GridView.view.cellWidth
             staffName: display
             isStaffActive: masterController.ui_masterViewController.isStaffActive(index)
             onRemoveStaff: {
                queryDialog.open()
                 actionIndex = index
             }
             Component.onCompleted: {
                 console.log("Staff still active: " + masterController.ui_masterViewController.isStaffActive(index))
             }
         }
        interactive: true
        clip: true
        flickableDirection: Flickable.HorizontalAndVerticalFlick
        focus: true
    }
    Dialog {

        property alias reason: queryField.text

        id: queryDialog
        width: 309
        height: 132
        dim: true
        modal: true
        x: (parent.width - width)/2
        y: (parent.height - height)/2
        header: Text {
            color: "#999494"
            text: "Query"
            font.weight: Font.Black
            font.bold: true
            font.capitalization: Font.AllUppercase
            font.pointSize: 22
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        background: Rectangle {
            gradient: Gradient {
                GradientStop {
                    position: 0.00;
                    color: "#eeeded";
                }
                GradientStop {
                    position: 1.00;
                    color: "#a6a6a6";
                }
            }

        }

        contentItem: ColumnLayout {
            TextField {
                id: queryField
                placeholderText: "Give reason for sacking staff"
                Layout.fillWidth: true
            }
            RowLayout {
                Layout.alignment: Qt.AlignRight
                GenericFlatButton {
                    id: cancelButton
                    text: "Cancel"
                    onClicked: queryDialog.rejected()
                }
                GenericFlatButton {
                    id: acceptButton
                    text: "Ok"
                    Layout.preferredWidth: cancelButton.width
                    onClicked: queryDialog.accepted()
                }
            }
        }
        onAccepted: {
            masterController.ui_masterViewController.sackStaff(actionIndex, queryField.text)
            queryDialog.close()
        }
        onRejected: queryDialog.close()
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
 ##^##*/
