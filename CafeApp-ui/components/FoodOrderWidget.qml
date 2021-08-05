import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3

Item {
    property alias refreshmentName: enableButton.text
    property alias minControl: amount.from
    property alias maxControl: amount.to
    property alias initialValue: amount.value
    property alias steps: amount.stepSize
    signal enableButtonChecked(bool state)
    signal unitValueChanged(string newUnit)
    width: 250
    height: 40
    RowLayout {
        width: 240
        anchors{
            fill: parent
            horizontalCenter: parent.horizontalCenter
        }
        spacing: 0

        CheckBox {
            id: enableButton
            enabled: true
            onCheckedChanged: {
                amount.value = "0"
                checked ? amount.enabled = true : amount.enabled = false
                enableButtonChecked(checked)
            }
            contentItem: Text {
                 text: enableButton.text
                 font.pixelSize: 15
                 font.bold: true
                 color: "white"
                 verticalAlignment: Text.AlignVCenter
                 leftPadding: enableButton.indicator.width + enableButton.spacing
            }
        }

        SpinBox {
            id: amount
            enabled: false
            Layout.preferredWidth: parent.width - enableButton.width
            stepSize: steps
            editable: true
            validator: IntValidator {
                locale: amount.locale.name
                bottom: Math.min(amount.from, amount.to)
                top: Math.max(amount.from, amount.to)
            }
            onValueChanged: {
                unitValueChanged(value)
            }
        }
        /*SpinBox {
            id: amount
            Layout.preferredHeight: 30
            Layout.preferredWidth: parent.width - enableButton.width
            stepSize: steps
            editable: true
            validator: IntValidator {
                locale: amount.locale.name
                bottom: Math.min(amount.from, amount.to)
                top: Math.max(amount.from, amount.to)
            }
            onValueChanged: {
                unitValueChanged(value)
            }
            background: Rectangle {
                anchors.fill: parent
                radius: 5
                opacity: 0.6
            }

            up.indicator: Rectangle {
                height: parent.height/2-2
                width: 12
                Canvas {
                    anchors.fill: parent
                    contextType: "2d"
                    onPaint: {
                        context.reset();
                        context.moveTo(width/2, 4);
                        context.lineTo(width-2, height-2);
                        context.lineTo(2, height-2);
                        context.closePath();
                        context.fillStyle = "#b0aead"
                        context.fill();
                    }
                }
                color: "transparent"
                anchors{
                    right: parent.right
                    top: parent.top
                    topMargin: 2
                    rightMargin: 2
                }

            }
            down.indicator: Rectangle {
                id: downIndicator
                height: parent.height/2-2
                width: 12
                Canvas {
                    anchors.fill: parent
                    contextType: "2d"
                    onPaint: {
                        context.reset();
                        context.moveTo(width/2, height-4);
                        context.lineTo(width-2, 2);
                        context.lineTo(2, 2);
                        context.closePath();
                        context.fillStyle = "#b0aead"
                        context.fill();
                    }
                }
                color: "transparent"
                anchors{
                    right: parent.right
                    bottom: parent.bottom
                    rightMargin: 2
                    bottomMargin: 2
                }
            }
        }*/
    }
}
