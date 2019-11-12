import QtQuick 2.0
import QtQuick.Window 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.12

Rectangle {
    id: window
    visible: true
    //    width: 600
    //    height: 400
    anchors.fill: parent


    FontLoader {
        id: varelaFont
        source: 'qrc:/font/VarelaRound-Regular.otf'
    }

    FontLoader {
        id: bigShoulder
        source: 'qrc:/font/BigShouldersText-Medium.ttf'
    }

    FontLoader {
        id: sansSerif
        source: 'qrc:/font/Montserrat-Regular.otf'
    }

    FontLoader {
        id: sansSerifBold
        source: "qrc:/font/MontserratAlternates-SemiBold.otf"
    }

    property alias myFont: bigShoulder

    readonly property int leftBarWidth: 256

    Material.theme: Material.Dark
    color: Material.background

    Pane {
        Material.elevation: 12
        id: topBar
        anchors.top: parent.top

        anchors.right: parent.right
        anchors.left: parent.left
        height: Material.dialogButtonBoxHeight

        background: Rectangle {
            color: Material.toolBarColor

            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: leftBar.width
                color: Qt.darker(parent.color)

                Text {
                    font.family: myFont.name
                    font.pointSize: 12
                    x: menuIcon.x + menuIcon.width + 4
                    y: menuIcon.y
                    text: 'MENU'
                    color: Material.foreground
                    visible: leftBar.state == 'VISIBLE'
                }
            }

            Image {
                id: menuIcon
                width: 24
                height: 24
                x: 10
                anchors.verticalCenter: parent.verticalCenter
                sourceSize.height: 24
                sourceSize.width: 24
                source: "qrc:/icon/menu-24px.svg"
                fillMode: Image.PreserveAspectFit

                ColorOverlay {
                    source: menuIcon
                    anchors.fill: parent
                    color: menuIconMouseArea.containsMouse ? Material.accent : Material.foreground
                }

                MouseArea {
                    hoverEnabled: true
                    anchors.fill: parent
                    id: menuIconMouseArea
                    onClicked: leftBar.state = leftBar.state == 'COLLAPSED' ? 'VISIBLE' : 'COLLAPSED'
                }
            }

            Text {
                text: 'Anime'
                x: leftBar.state == 'COLLAPSED' ? menuIcon.x + menuIcon.width + 8: leftBar.x + leftBar.width + 8
                anchors.verticalCenter: parent.verticalCenter
                font.family: myFont.name
                font.pointSize: 20
                color: Material.foreground
            }
        }
    }

    DropShadow {
        id: dropShadow
        anchors.fill: topBar
        horizontalOffset: 3
        verticalOffset: 3
        radius: 16
        samples: 32
        color: '#80000000'
        source: topBar
        transparentBorder: true
    }

    TopAnime {
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: topBar.bottom
        anchors.topMargin: 0
    }

    Rectangle {
        id: leftBar
        width: leftBarWidth
        color: "#ffffff"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: topBar.bottom
        anchors.topMargin: 0

        Component.onCompleted: state = 'COLLAPSED'

        states: [
            State {
                name: "COLLAPSED"
                PropertyChanges {
                    target: leftBar
                    width: 0
                }
            },
            State {
                name: "VISIBLE"
                PropertyChanges {
                    target: leftBar
                    width: leftBarWidth
                }
            }
        ]

        transitions: [
            Transition {
                from: "COLLAPSED"
                to: "VISIBLE"
                NumberAnimation {
                    properties: "width"
                    duration: 100
                }
            },
            Transition {
                from: "VISIBLE"
                to: "COLLAPSED"
                NumberAnimation {
                    properties: "width"
                    duration: 100
                }
            }
        ]
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2;anchors_width:600;anchors_x:0;anchors_y:0}
D{i:8;anchors_height:428;anchors_x:0;anchors_y:52}
}
##^##*/

