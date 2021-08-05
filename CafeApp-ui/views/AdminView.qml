import QtQuick 2.9
import QtQuick.Window 2.3
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import components 1.0
import assets 1.0

Window {
    id: adminView
    visible: true
    minimumHeight: 480
    minimumWidth: 640
    Loader { id: viewLoader }

    Rectangle {
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#f4f4f4"
            }

            GradientStop {
                position: 1
                color: "#d5d3d3"
            }
        }
        anchors.fill: parent

        Rectangle {
            id: sidepane
            width: 132
            border.width: 3
            border.color: "lightgrey"
            gradient: Gradient {
                GradientStop {
                    position: 0
                    color: "#fbfbfb"
                }

                GradientStop {
                    position: 1
                    color: "#f9f8f8"
                }
            }
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            anchors.top: parent.top
            anchors.topMargin: 0

            Text {
                id: dashboard
                height: 32
                color: "#5a87c7"
                text: qsTr("DASHBOARD")
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                font.bold: true
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 14
            }
            TextField {
                id: dateSearchField
                height: 24
                placeholderText: "dd-MM-yyyy"
                anchors.top: dashboard.bottom
                anchors.topMargin: 21
                anchors.right: parent.right
                anchors.rightMargin: 19
                anchors.left: parent.left
                anchors.leftMargin: 8
                horizontalAlignment: Text.AlignHCenter
                background: Rectangle {
                    border.color: "#c8c4c4"
                    border.width: 0
                }
                validator: RegExpValidator {
                    regExp: /^(0[1-9]|3[01]|[12][0-9])-(0[1-9]|1[0-2])-\d{2,4}$/
                }
                Keys.onReturnPressed: {
                    Keys.enterPressed(event)
                    searchButton.clicked()
                }
                RoundButton {
                    id: searchButton
                    width: 24
                    text: "\uf002"
                    contentItem: Text {
                       text: searchButton.text
                       font.pixelSize: 10
                       font.family: Styler.faSolid

                    }
                    anchors.right: parent.right
                    anchors.rightMargin: -11
                    anchors.bottom: parent.bottom
                    anchors.top: parent.top
                    background: Rectangle {
                        radius: searchButton.width/2
                    }
                    onClicked: {
                        contextView.replace("qrc:/views/DateSummaryView.qml", { "givenDate": dateSearchField.text }, StackView.Immediate)
                    }
                }
            }
            ColumnLayout {
                id: timelyreport
                y: 133
                height: 88
                anchors.verticalCenterOffset: -77
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                spacing: 0

                GenericFlatButton {
                    id: dailySummaryButton
                    text: "Daily Summary"
                    Layout.fillWidth: true
                    onClicked: contextView.replace("qrc:/views/DailySummaryView.qml")
                }
                GenericFlatButton {
                    id: weeklySummaryButton
                    text: "Weekly Summary"
                    Layout.fillWidth: true
                    onClicked: contextView.replace("qrc:/views/WeeklySummaryView.qml")
                }
                GenericFlatButton {
                    id: annualSummaryButton
                    text: "Annual Summary"
                    Layout.fillWidth: true
                    onClicked: contextView.replace("qrc:/views/AnnualSummaryView.qml")
                }
            }

            ColumnLayout {
                id: addnremove
                y: 334
                height: 62
                anchors.bottom: signoutButton.top
                anchors.bottomMargin: 26
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 8
                spacing: 0

                GenericFlatButton {
                    text: "Sack Staff"
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                    Layout.fillWidth: true
                    onClicked: contextView.replace("qrc:/views/EditStaffView.qml")
                }

                GenericFlatButton {
                    text: "Add/Remove Menu"
                    Layout.fillWidth: true
                    onClicked: contextView.replace("qrc:/views/EditMenuView.qml")
                }
            }
            GenericRoundButton {
                id: signoutButton
                x: 60
                y: 412
                width: 60
                height: 60
                text: "Sign Out"
                font.weight: Font.Bold
                font.bold: true
                font.pointSize: 6
                font.family: "Courier"
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 8
                anchors.right: parent.right
                anchors.rightMargin: 8
                onClicked: {
                    adminView.close();
                    viewLoader.source = "qrc:/components/LoginWindow.qml"
                }
            }


        }
        StackView {
            id: contextView
            initialItem: Qt.resolvedUrl("qrc:/views/DailySummaryView.qml")
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.left: sidepane.right
            focus: true
            onCurrentItemChanged: currentItem.forceActiveFocus()
            replaceEnter: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 0
                    to: 1
                    duration: 500
                }
            }
            replaceExit: Transition {
                PropertyAnimation {
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 500
                }
            }


        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:10;anchors_width:116;anchors_x:8;anchors_y:8}
}
##^##*/
