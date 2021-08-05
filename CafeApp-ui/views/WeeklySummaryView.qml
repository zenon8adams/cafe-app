import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Item {

    Text {
        id: date
        width: 235
        height: 34
        color: "#1f2f38"
        text: masterController.ui_masterViewController.ui_weekDate
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 8
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 14
    }
    TableView {
        height: 423
        anchors.top: date.bottom
        anchors.topMargin: 5
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.left: parent.left

        focus: true
        clip: true
        interactive: true
        boundsBehavior: Flickable.StopAtBounds
        model: masterController.ui_masterViewController.weeklySummary()
        delegate: Rectangle {
            id: internalRep
          opacity: 0.65
          implicitWidth: 150
          implicitHeight: 50
          border.color: "#bdbbba"
          Text {
              text: tabledata
              font.pointSize: (heading === true) ? 13 : 12
              font.bold: (heading === true || footer == true)
              anchors.centerIn: parent
          }
          MouseArea {
              anchors.fill: parent
              hoverEnabled: true
              onEntered: internalRep.state = "hover"
              onExited: internalRep.state = ""
          }
          ToolTip {
              id: widgetDiscription
              text: masterController.ui_masterViewController.ui_toolTipDescription.at(tabledata)
              contentItem: Text {
                text: widgetDiscription.text
                font: widgetDiscription.font
                color: "#939593"
              }
          }
          states: State {
              name: "hover"
              PropertyChanges {
                  target: widgetDiscription
                  visible: {
                      var textAt = masterController.ui_masterViewController.ui_toolTipDescription.at(tabledata)
                      heading == true && textAt.length > 0
                  }
              }
          }
          Connections{
              target: masterController.ui_masterViewController
              onUpdateStaffMetadata: {
                  widgetDiscription.text = masterController.ui_masterViewController.ui_toolTipDescription.at(tabledata)
              }
          }
        }
        ScrollBar.vertical: ScrollBar{}
    }
}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:1;anchors_width:640;anchors_x:0}D{i:3;anchors_x:8}
}
 ##^##*/
