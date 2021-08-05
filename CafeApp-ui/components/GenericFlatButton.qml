import QtQuick 2.9
import QtQuick.Controls 2.4

Button {
    //property alias buttonText: internalPh.text
    font.family: "Times New Roman"
    focusPolicy: Qt.TabFocus
    hoverEnabled: true
    /*contentItem: Text {
        id: internalPh
        color: "#ffffff"
    }*/

    onHoveredChanged: hovered ? signupButtonBackground.state = "hover" : signupButtonBackground.state = ""
    background: Rectangle {
        id: signupButtonBackground
        color: "lightgrey"
        border.width: 0
        gradient: Gradient {
            GradientStop {
                position: 0.12;
                color: "#d3d3d3";
            }
            GradientStop {
                position: 1.00;
                color: "#ffffff";
            }
        }
        states: State {
            name: "hover"
            PropertyChanges {
                target: signupButtonBackground
                color: "transparent"
                border.color: "lightgrey"
                border.width: 1
            }
        }
    }
}
