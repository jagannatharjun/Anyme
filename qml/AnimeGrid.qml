import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13

GridView {
    visible: true

    //    Text {
    //        text: 'Top Anime'
    //        font.family: myFont.name
    //        font.pointSize: 24
    //        color: Material.foreground
    //        leftPadding: 6
    //        id: topText
    //    }

    //    Row {
    //        id: categoryList
    //        y: topText.y + topText.height + 2
    //        Repeater {
    //            model: animelist.categoryList()
    //            Text {
    //                leftPadding: 6
    //                font.family: myFont.name
    //                font.pointSize: 16
    //                font.underline: animelist.categoryIndex == index
    //                color: animelist.gettingList ? Qt.darker(
    //                                                   Material.foreground) : Material.foreground
    //                text: model.modelData
    //                rightPadding: 12
    //                bottomPadding: 12

    //                MouseArea {
    //                    anchors.fill: parent
    //                    cursorShape: Qt.PointingHandCursor
    //                    onClicked: {
    //                        animelist.categoryIndex = index
    //                    }
    //                    enabled: !animelist.gettingList
    //                }
    //            }
    //        }
    //    }

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

//    onAtYEndChanged: {
//        if (atYEnd) {
//            print("at end")
//            animelist.nextPage()
//        }
//    }

    id: animeGrid
    boundsBehavior: Flickable.StopAtBounds

    delegate: Rectangle {
        readonly property int ribbionHeight: 56
        width: animeGrid.cellWidth
        height: animeGrid.cellHeight
        //            border.width: 4
        //            border.color: mouseArea.containsMouse ? Material.highlightedButtonColor : 'transparent'
        color: mouseArea.containsMouse ? theme.accent : 'transparent'

        Item {
            anchors.fill: parent
            anchors.margins: 8

            Image {
                anchors.left: parent.left
                anchors.top: parent.top
                width: parent.width
                height: parent.height - ribbionHeight
                source: imageUrl
                id: img
                fillMode: Image.PreserveAspectCrop
            }

            Text {
                anchors.margins: 2
                anchors.top: img.bottom
                anchors.left: img.left
                width: parent.width
                height: ribbionHeight

                font.family: fonts.sansSerifBold.name
                text: title
                color: theme.foreground
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