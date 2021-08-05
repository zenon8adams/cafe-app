import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import CafeApp 1.0

Item {
    property string headerText
    property int minControl
    property int maxControl
    property int initialValue
    property int steps
    property MenuListModel repeaterModel
    signal removeItemFromModel(string data)
    signal addItemToModel(string data)
    signal setUnitToValue(string data, string value)
    width: 272
    height: 550

    id: itemSelection

    ListView {
        id: itemView
        y: 91
        height: 459
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        spacing: 10
        model: repeaterModel
        clip: true
        delegate: FoodOrderWidget {
            id: foodOrder
            refreshmentName: itemView.dataAt(index, 0)
            minControl: itemSelection.minControl
            maxControl: itemSelection.maxControl
            steps: itemSelection.steps
            Connections {
                target: foodOrder
                onEnableButtonChecked: {
                    if(state === false)
                        removeItemFromModel(itemView.dataAt(index, 0))
                    else addItemToModel(itemView.dataAt(index, 0))
                }
                onUnitValueChanged: {
                    setUnitToValue(itemView.dataAt(index, 0), newUnit)
                }
            }
        }
        function dataAt(index, pos)
        {
            return repeaterModel.data(repeaterModel.index(index, 0))[pos]
        }
    }

    Text {
        x: 83
        y: 19
        width: 106
        height: 44
        text: itemSelection.headerText
        style: Text.Sunken
        font.weight: Font.Bold
        font.bold: true
        color: "#ffffff"
        anchors.horizontalCenterOffset: 0
        anchors.horizontalCenter: itemView.horizontalCenter
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pixelSize: 30
    }

}


/*##^## Designer {
    D{i:1;anchors_width:272;anchors_x:0}
}
 ##^##*/
