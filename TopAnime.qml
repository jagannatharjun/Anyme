import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13

Rectangle {
    visible: true

    Material.theme: Material.Dark
    color: Material.background

    Text {
        text: 'Top Anime'
        font.family: myFont.name
        font.pointSize: 24
        color: Material.foreground
        leftPadding: 6
        id: topText
    }

    Row {
        id: categoryList
        y: topText.y + topText.height + 2
        Repeater {
            model: animelist.categoryList()
            Text {
                leftPadding: 6
                font.family: myFont.name
                font.pointSize: 16
                font.underline: animelist.categoryIndex == index
                color: Material.foreground
                text: model.modelData
                rightPadding: 12

                MouseArea {
                    anchors.fill: parent
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        animelist.categoryIndex = index
                    }
                    enabled: !animelist.gettingList
                }
            }
        }
    }

    GridView {
        anchors.top: categoryList.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        onWidthChanged: {
            let normalHeight = 240
            let normalWidth = 136

            let normalColumnCount = Math.floor(width / normalWidth)
            let normalRequiredWidth = normalColumnCount * normalWidth
            let leftWidth = width - normalRequiredWidth - ScrollBar.vertical.width
            if (leftWidth > 0) {
                let tilesCanBeAdded = leftWidth / normalWidth
                if (tilesCanBeAdded >= 1) {
                    normalColumnCount += tilesCanBeAdded
                }
            }

            cellWidth = (width - ScrollBar.vertical.width) / normalColumnCount
            cellHeight = normalHeight * cellWidth / normalWidth
        }

        clip: true

        ScrollBar.vertical: ScrollBar {
            visible: true
        }

        onAtYEndChanged: {
            if (atYEnd) {
                print("at end")
                animelist.nextPage()
            }
        }

        id: animeGrid
        anchors.rightMargin: 0
        boundsBehavior: Flickable.StopAtBounds

        model: animelist.model

        delegate: Rectangle {
            property int ribbionHeight: 56
            width: animeGrid.cellWidth
            height: animeGrid.cellHeight

            //            border.width: 4
            //            border.color: mouseArea.containsMouse ? Material.highlightedButtonColor : 'transparent'
            color: mouseArea.containsMouse ? Material.highlightedButtonColor : 'transparent'

            Item {
                anchors.fill: parent
                anchors.margins: 4

                Image {
                    anchors.left: parent.left
                    anchors.top: parent.top
                    width: parent.width
                    height: parent.height - ribbionHeight
                    source: imageUrl
                    id: img
                    fillMode: Image.PreserveAspectCrop
                }

                Rectangle {
                    anchors.top: img.bottom
                    anchors.left: img.left
                    width: parent.width
                    height: ribbionHeight
                    visible: mouseArea.containsMouse && false
                    color: Material.frameColor
                    id: txtRect
                }

                Text {
                    anchors.margins: 2
                    anchors.fill: txtRect
                    font.family: sansSerifBold.name
                    text: title
                    color: Material.foreground
                    verticalAlignment: Text.AlignTop
                    wrapMode: Text.WordWrap
                    font.bold: true
                    font.pointSize: 10
                    elide: Text.ElideRight
                }
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                id: mouseArea
                onClicked: application.loadAnimeInfo(malId)
            }
        }
    }
}
