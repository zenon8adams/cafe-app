import QtQuick 2.11
import QtQuick.Controls 2.4

Button {
    font.family: "Times New Roman"
    focusPolicy: Qt.TabFocus
    hoverEnabled: true
    onHoveredChanged:  hovered ? signupButtonBackground.state = "hover" : signupButtonBackground.state = ""
    background: Rectangle {
        id: signupButtonBackground
        color: "lightgrey"
        gradient: Gradient {
            id: buttonGradient
            GradientStop {
                position: 0.00;
                color: "#d3d3d3";
            }
            GradientStop {
                position: 0.83;
                color: "#ffffff";
            }
        }
        border.width: 0
        radius: (width/2)
        states: State {
            name: "hover"
            PropertyChanges {
                target: signupButtonBackground
                color: "transparent"
                border.color: "lightgrey"
            }
        }
    }
}
