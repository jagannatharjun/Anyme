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
            height: animelist.isError || animelist.isLoading ? 100 : 0
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

    Item {
        id: leftBar
        //    color: theme.primary
        height: parent.height
        //contentHeight: leftBarContent.childrenRect.height
        //boundsBehavior: Flickable.StopAtBounds
        //        ScrollBar.vertical: ScrollBar {
        //            visible: true
        //            width: 8
        //        }
        clip: true

        Image {
            x: 16
            y: 20
            source: "qrc:/icon/logo.png"
            id: logoImg
        }

        Text {
            font.family: fonts.bigShoulderRegular.name
            font.pixelSize: 37
            color: theme.primaryForeground
            text: "ANIME"
            x: 120
            anchors.verticalCenter: logoImg.verticalCenter
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

    Item {
        parent: leftBar
        id: leftBarContent
        anchors.fill: parent

        property int sepMargin: 2

        NavigationSeperator {
            y: 140
            id: animeSep
        }

        NavigationButton {
            x: 0
            anchors.top: animeSep.bottom
            anchors.topMargin: parent.sepMargin
            id: homeBtn
        }

        NavigationSeperator {
            anchors.top: homeBtn.bottom
            anchors.topMargin: parent.sepMargin
            id: homeDownSep
        }

        Text {
            font.pixelSize: 18
            font.family: fonts.bigShoulderRegular.name
            color: theme.disabledText
            font.weight: Font.DemiBold
            x: 19.625
            anchors.top: homeDownSep.bottom
            anchors.topMargin: 4
            text: 'Browse'
            id: browseText
        }

        ListView {
            x:0
            anchors.top: browseText.bottom
            anchors.topMargin: 6
            anchors.bottom: searchUpSep.bottom
            contentHeight: catModel.count * 40
            model: ListModel {
                id: catModel
                ListElement {
                    btnText: 'Airing'
                    iconUrl: 'qrc:/icon/NavigationIcons/airing.png'
                }
                ListElement {
                    btnText: 'Upcoming'
                    iconUrl: 'qrc:/icon/NavigationIcons/upcoming.png'
                }
                ListElement {
                    btnText: 'TV'
                    iconUrl: 'qrc:/icon/NavigationIcons/tv.png'
                }
                ListElement {
                    btnText: 'Movie'
                    iconUrl: 'qrc:/icon/NavigationIcons/movie.png'
                }
                ListElement {
                    btnText: 'OVA'
                    iconUrl: 'qrc:/icon/NavigationIcons/ova.png'
                }
                ListElement {
                    btnText: 'Special'
                    iconUrl: 'qrc:/icon/NavigationIcons/special.png'
                }
                ListElement {
                    btnText: 'Popular'
                    iconUrl: 'qrc:/icon/NavigationIcons/popular.png'
                }
                ListElement {
                    btnText: 'Favorite'
                    iconUrl: 'qrc:/icon/NavigationIcons/favorite.png'
                }
            }

            delegate: CategoryButtonDelegate {
                isSelected: animelist.categoryIndex == index
                onClicked: animelist.categoryIndex = index
            }
        }

        NavigationSeperator {
            anchors.bottom: searchBtn.top
            anchors.bottomMargin: parent.sepMargin
            id: searchUpSep
        }

        NavigationButton {
            anchors.bottom: bottomSep.top
            anchors.bottomMargin: parent.sepMargin
            btnText: 'Search'
            id: searchBtn
        }

        NavigationSeperator {
            anchors.bottom: leftBarContent.bottom
            anchors.bottomMargin: parent.sepMargin
            id: bottomSep
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:2;anchors_width:600;anchors_x:0;anchors_y:0}
D{i:8;anchors_height:428;anchors_x:0;anchors_y:52}
}
##^##*/

