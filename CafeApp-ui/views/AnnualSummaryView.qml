import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Item {

    Text {
        id: date
        width: 235
        height: 34
        color: "#1f2f38"
        text: masterController.ui_masterViewController.ui_annualDate
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
        model: masterController.ui_masterViewController.annualSummary()
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

        }
        ScrollBar.vertical: ScrollBar{}
    }
}
