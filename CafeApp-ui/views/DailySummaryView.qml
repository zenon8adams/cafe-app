import QtQuick 2.12
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import components 1.0

Item {
    TabBar {
          id: bar
          width: parent.width

          Repeater {
              id: continuity
              model: masterController.ui_masterViewController.staffsOnDuty()
              delegate: TabButton {
                  text: model.display
                  font.bold: true
                  background: Rectangle {
                      color: "#d3d6d3"
                  }
              }
          }
      }

      StackLayout {
          id: summaryViewLayout
          anchors.top: date.bottom
          anchors.topMargin: 6
          anchors.rightMargin: 8
          anchors.bottomMargin: 8
          anchors.leftMargin: 8
          anchors.bottom: parent.bottom
          anchors.right: parent.right
          anchors.left: parent.left

          currentIndex: bar.currentIndex
          Repeater {
              model: masterController.ui_masterViewController.dailySummary()
              delegate: TableView {
                  focus: true
                  clip: true
                  Layout.preferredWidth: parent.width
                  Layout.topMargin: 20
                  interactive: true
                  boundsBehavior: Flickable.StopAtBounds
                  model: modelData
                  delegate: Rectangle {
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
                  }
                  ScrollBar.vertical: ScrollBar{}
              }
          }
      }

      Rectangle {
          id: date
          width: 220
          height: 47
          opacity: 0.34
          gradient: Gradient {
              GradientStop {
                  position: 0
                  color: "#ffffff"
              }

              GradientStop {
                  position: 1
                  color: "#8b8787"
              }
          }
          anchors.top: bar.bottom
          anchors.topMargin: 15
          anchors.left: parent.left
          anchors.leftMargin: 8
          Text {
              anchors.centerIn: parent
              text: masterController.ui_masterViewController.ui_date
              font.bold: true
              font.pixelSize: 15
          }
      }


}

/*##^## Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:5;anchors_height:404;anchors_y:68}D{i:11;anchors_x:8;anchors_y:15}
}
 ##^##*/
