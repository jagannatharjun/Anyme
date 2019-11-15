import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtGraphicalEffects 1.12

Rectangle {
    id: window
    visible: true
    //    width: 600
    //    height: 400
    anchors.fill: parent

    MyFonts {
        id: fonts
    }

    property alias myFont: fonts.bigShoulder
    color: theme.background

    Rectangle {
        color: theme.primary
        id: topBar
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.left
        height: 56

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
                color: theme.primaryForeground
            }

            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                id: menuIconMouseArea
                onClicked: leftBar.state = leftBar.state == 'COLLAPSED' ? 'VISIBLE' : 'COLLAPSED'
                cursorShape: Qt.PointingHandCursor
            }
        }

        Text {
            text: 'Anime'
            x: menuIcon.x + menuIcon.width + 8
            visible: leftBar.state == 'COLLAPSED'
            anchors.verticalCenter: parent.verticalCenter
            font.family: myFont.name
            font.pointSize: 20
            color: theme.primaryForeground
        }
    }

    DropShadow {
        id: topBarShadow
        anchors.fill: topBar
        horizontalOffset: 5
        verticalOffset: 5
        radius: 16
        samples: 32
        color: '#80000000'
        source: topBar
        transparentBorder: true
    }

    AnimeGrid {
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.top: topBar.bottom
        anchors.topMargin: topBarShadow.verticalOffset + topBarShadow.radius
        model: animelist.model
        id: animeGrid

        onAtYEndChanged: {
            // on indexChange count becomes 0 and this method is called be setCategoryIndex
            if (atYEnd && count != 0) {
                animelist.nextPage()
            }
        }

        footer: Item {
            id: animeGridNotif
            visible: animelist.isError || animelist.isLoading
            height: 100
            anchors.left: parent.left
            anchors.right: parent.right

            BusyIndicator {
                anchors.centerIn: parent
                running: animelist.isLoading
            }

            Text {
                anchors.centerIn: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                visible: animelist.isError
                text: animelist.errorString
                font.family: myFont.name
                font.pixelSize: 24
                color: theme.disabledText
            }
        }
    }

    Component.onCompleted: animelist.categoryIndex = 0

    Rectangle {
        id: darkOverlay
        anchors.right: parent.right
        anchors.left: leftBar.right
        width: parent.width - leftBar.width
        height: parent.height
        color: 'black'
        opacity: .7
        visible: leftBar.state == 'VISIBLE'
    }

    MouseArea {
        id: captureMouseEvents
        anchors.fill: parent
        propagateComposedEvents: false
        hoverEnabled: true
        onClicked: leftBar.state = 'COLLAPSED'
        visible: leftBar.state == 'VISIBLE'
    }

    Rectangle {
        width: leftBar.width
        height: window.height
        color: theme.primary
    }

    Flickable {
        id: leftBar
        //    color: theme.primary
        height: parent.height
        contentHeight: leftBarContent.childrenRect.height
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar {
            visible: true
            width: 8
        }
        clip: true

        Item {
            id: leftBarContent
            anchors.fill: parent
            Image {
                id: img
                source: 'qrc:/icon/logo.png'
                width: 81
                height: 104
                x: 11
                y: 17
            }

            Text {
                anchors.verticalCenter: img.verticalCenter
                x: 117
                font.family: fonts.bigShoulder.name
                color: theme.primaryForeground
                text: 'ANIME'
                font.pixelSize: 30
            }

            Rectangle {
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                y: 134
                height: 1
                color: theme.disabledText
                id: logoSeperator
            }

            Text {
                anchors.top: logoSeperator.bottom
                anchors.left: logoSeperator.left
                anchors.topMargin: 4
                anchors.leftMargin: 4
                font.family: fonts.bigShoulder.name
                color: theme.primaryForeground
                text: 'Top'
                font.pixelSize: 26
                id: topTxt
                MouseArea {
                    anchors.fill: parent
                    onClicked: dropDown.toogleState()
                }
            }

            Image {
                anchors.right: logoSeperator.right
                anchors.verticalCenter: topTxt.verticalCenter
                source: 'qrc:/icon/drop-down.png'
                transformOrigin: Item.Center
                id: dropDown

                function toogleState() {
                    state = isCollapsed ? 'DROPPED' : 'COLLAPSED'
                }

                property bool isCollapsed: state == 'COLLAPSED'

                MouseArea {
                    anchors.fill: parent
                    onClicked: dropDown.toogleState()
                }

                Component.onCompleted: state = 'COLLAPSED'

                states: [
                    State {
                        name: "COLLAPSED"
                        PropertyChanges {
                            target: dropDown
                            rotation: 180
                        }
                    },
                    State {
                        name: "DROPPED"
                        PropertyChanges {
                            target: dropDown
                            rotation: 0
                        }
                    }
                ]

                transitions: [
                    Transition {
                        from: "COLLAPSED"
                        to: "DROPPED"
                        reversible: true
                        NumberAnimation {
                            properties: "rotation"
                            duration: 100
                        }
                    }
                ]
            }

            ListView {
                id: catlist
                anchors.top: topTxt.bottom
                width: leftBar.width
                interactive: false
                model: animelist.categoryList()
                clip: true

                delegate: Rectangle {
                    width: leftBar.width
                    height: 34
                    color: mouseArea.containsMouse
                           || animelist.categoryIndex == index ? theme.background : 'transparent'
                    MouseArea {
                        anchors.fill: parent
                        id: mouseArea
                        onClicked: animelist.categoryIndex = index
                        hoverEnabled: true
                    }

                    Text {
                        x: topTxt.x + 12
                        anchors.verticalCenter: parent.verticalCenter
                        font.family: fonts.bigShoulder.name
                        color: mouseArea.containsMouse
                               || animelist.categoryIndex
                               == index ? theme.foreground : theme.primaryForeground
                        text: model.modelData.charAt(0).toUpperCase(
                                  ) + model.modelData.slice(1)
                        font.pixelSize: 20
                        font.underline: animelist.categoryIndex == index
                    }
                }

                state: dropDown.isCollapsed ? 'COLLAPSED' : 'DROPPED'

                height: 34 * catlist.count
                states: [
                    State {
                        name: "COLLAPSED"
                        PropertyChanges {
                            target: catlist
                            height: 0
                        }
                    },
                    State {
                        name: "DROPPED"
                        PropertyChanges {
                            target: catlist
                            height: 34 * catlist.count
                        }
                    }
                ]

                transitions: [
                    Transition {
                        from: "COLLAPSED"
                        to: "DROPPED"
                        reversible: true
                        NumberAnimation {
                            properties: "height"
                            duration: 256
                        }
                    }
                ]
            }
            Rectangle {
                anchors.right: parent.right
                anchors.left: parent.left
                anchors.leftMargin: 12
                anchors.rightMargin: 12
                anchors.top: catlist.bottom
                anchors.topMargin: 16
                height: 1
                color: theme.disabledText
                id: topSeparator
            }

            Text {
                anchors.top: topSeparator.bottom
                anchors.topMargin: 4
                anchors.left: logoSeperator.left
                anchors.leftMargin: 4
                font.family: fonts.bigShoulder.name
                color: theme.primaryForeground
                text: 'Search'
                font.pixelSize: 26
            }
        }

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
                    width: 256
                }
            }
        ]

        transitions: [
            Transition {
                from: "COLLAPSED"
                to: "VISIBLE"
                reversible: true
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

