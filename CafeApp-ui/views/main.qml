import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtGraphicalEffects 1.0
import Qt.labs.settings 1.1
import assets 1.0

Window {

    property int offset : 2
    property int size: 2
    property bool firstTimeLaunch: masterController.firstTimeAppLaunch()
    id: mainWindow
    visible: true
    width: 400
    height: 480
    title: "CafeApp"
    flags: Qt.ToolTip | Qt.FramelessWindowHint | Qt.WA_TranslucentBackground
    Rectangle {
        id: rectangle
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#7f5124"
            }
            GradientStop {
                position: 1
                color: "#a97b4e"
            }
        }
        anchors.fill: parent

        Text {
            width: 109
            height: 51
            color: "#2a2626"
            text: "\uf1f9 Copyright 2019.\nElektra Inc."
            font.family: Styler.faRegular
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 8
            anchors.right: parent.right
            anchors.rightMargin: 8
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.pixelSize: 11
        }

        Item {
                id: rectOutter;
                width: 200
                height: 268
                anchors.centerIn: parent;

                Canvas {
                    opacity: 0.35;
                    antialiasing: true;
                    contextType: "2d";
                    anchors.fill: parent;
                    onPaint: {
                        if (context) {
                            context.reset ();
                            context.globalCompositeOperation = "source-over";
                            context.fillStyle = "grey";
                            context.beginPath ();
                            context.rect (0, 0, width, height);
                            context.fill ();
                            context.globalCompositeOperation = "xor";
                            context.fillStyle = "grey";
                            context.beginPath ();
                            context.rect (rectInner.x, rectInner.y, rectInner.width, rectInner.height);
                            context.fill ();
                        }
                    }
                    onWidthChanged:  { requestPaint (); }
                    onHeightChanged: { requestPaint (); }
                }
                Rectangle {
                    id: sourceOutter;
                    antialiasing: true;
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: "grey"; }
                        GradientStop { position: 0.5; color: "transparent"; }
                        GradientStop { position: 1.0; color: "white"; }
                    }
                    anchors {
                        fill: parent;
                        margins: -offset;
                    }
                }
                Rectangle {
                    id: maskOutter;
                    color: "transparent";
                    antialiasing: true;
                    border {
                        width: offset;
                        color: "black";
                    }
                    anchors.fill: sourceOutter;
                }
                OpacityMask {
                    opacity: 0.65;
                    source: ShaderEffectSource {
                        sourceItem: sourceOutter;
                        hideSource: true;
                    }
                    maskSource: ShaderEffectSource {
                        sourceItem: maskOutter;
                        hideSource: true;
                    }
                    anchors.fill: sourceOutter;
                }
                Item {
                    id: rectInner;
                    anchors {
                        fill: parent;
                        margins: size;
                    }

                    Rectangle {
                        id: sourceInner;
                        antialiasing: true;
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "white"; }
                            GradientStop { position: 0.5; color: "transparent"; }
                            GradientStop { position: 1.0; color: "black"; }
                        }
                        anchors {
                            fill: parent;
                            margins: -offset;
                        }


                    }
                    Rectangle {
                        id: maskInner;
                        color: "transparent";
                        antialiasing: true;
                        border {
                            width: offset;
                            color: "black";
                        }
                        anchors.fill: sourceInner
                        Image {
                            id: name
                            width: 139
                            height: 248
                            anchors.centerIn: parent
                            source: "qrc:/icons/cafeapp-logo.png"
                        }
                    }
                    OpacityMask {
                        opacity: 0.65;
                        source: ShaderEffectSource {
                            sourceItem: sourceInner;
                            hideSource: true;
                        }
                        maskSource: ShaderEffectSource {
                            sourceItem: maskInner;
                            hideSource: true;
                        }
                        anchors.fill: sourceInner;
                    }
                }
            }
    }

    Timer {
        id: appTimer
        interval: 5000
        onTriggered: {
            mainWindow.close()
            if(firstTimeLaunch)
                viewLoader.source = Qt.resolvedUrl("qrc:/components/CafeAppInstance.qml")
            else
            {
                viewLoader.source = Qt.resolvedUrl("qrc:/components/LoginWindow.qml")
                masterController.initAdmin()
            }
        }
    }

    Loader { id: viewLoader }
    Component.onCompleted: {
        appTimer.running = true
        console.log("FirstTimeAppLaunch - " + masterController.firstTimeAppLaunch())
    }
}
