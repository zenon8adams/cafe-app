import QtQuick 2.11
import QtQuick.Controls 2.4
import assets 1.0

Item {
    id: staff
    property alias staffName: centralText.text
    property bool isStaffActive
    signal removeStaff()
    width: 170
    height: width
    Rectangle {
        width: 150
        height: width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#c5b17f"
            }

            GradientStop {
                position: 1
                color: "#a37706"
            }
        }

        Rectangle {
            anchors.rightMargin: -8
            anchors.bottomMargin: -8
            anchors.leftMargin: 8
            anchors.topMargin: 8
            anchors.fill: parent
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#fad985"
                }

                GradientStop {
                    position: 1
                    color: "#e9dfc3"
                }
            }
            Text {
                width: 33
                height: 32
                color: "#ffffff"
                text: "\uf007"
                styleColor: "#fafafa"
                font.family: Styler.faSolid
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 8
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 21
            }
            Text {
                id: centralText
                height: 55
                anchors.right: parent.right
                anchors.rightMargin: 4
                anchors.left: parent.left
                anchors.leftMargin: 4
                font.capitalization: Font.AllUppercase
                anchors.verticalCenter: parent.verticalCenter
                font.bold: true
                wrapMode: Text.WordWrap
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 21
            }

            CheckBox {
                id: checkBox
                width: 85
                height: 29
                text: "Remove"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 2
                anchors.left: parent.left
                anchors.leftMargin: 8
                enabled: isStaffActive
                onClicked:{
                    staff.enabled = false
                    removeStaff()
                }
            }
        }
    }



}

/*##^## Designer {
    D{i:29;anchors_height:48;anchors_width:116;anchors_x:42;anchors_y:54}D{i:30;anchors_x:8;anchors_y:20}
D{i:31;anchors_x:8}D{i:21;anchors_height:200;anchors_width:200;anchors_x:8;anchors_y:8}
}
 ##^##*/
