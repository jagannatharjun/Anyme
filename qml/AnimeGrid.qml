import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13

GridView {
    property bool isError: false
    property bool isLoading: false
    property string errorString: ''
    onErrorStringChanged: print(errorString)

    anchors.right: parent.right
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    anchors.top: topBar.bottom
    anchors.topMargin: topBarShadow.verticalOffset + topBarShadow.radius

    visible: true

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

    id: animeGrid
    boundsBehavior: Flickable.StopAtBounds

    delegate: Rectangle {
        readonly property int ribbionHeight: 56
        width: animeGrid.cellWidth
        height: animeGrid.cellHeight
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

                BusyIndicator {
                    anchors.centerIn: parent
                    running: img.status == Image.Loading
                }
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

    footer: Item {
        id: animeGridNotif
        height: 124
        anchors.left: parent.left
        anchors.right: parent.right

        BusyIndicator {
            anchors.centerIn: parent
            running: isLoading
        }

        Text {
            height: 124
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            visible: isError
            text: errorString
            font.family: myFont.name
            font.pixelSize: 24
            color: theme.disabledText
            wrapMode: Text.Wrap
        }
    }
}
