import QtQuick 2.13
import QtQuick.Window 2.13
import QtQuick.Controls 2.13

GridView {
    property bool isError: false
    property bool isLoading: false
    property string errorString: ''

    function tileClicked() {}

    anchors.right: parent.right
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    anchors.top: topBar.bottom
    anchors.topMargin: topBarShadow.verticalOffset + topBarShadow.radius

    visible: true

    onWidthChanged: {
        let normalHeight = 280
        let normalWidth = 156

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
        id: scrollBar
        stepSize: .1
    }

    Keys.onUpPressed: scrollBar.decrease()
    Keys.onDownPressed: scrollBar.increase()
    focus: true

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
                font.family: fonts.bigShoulder.name
                text: title
                color: theme.foreground
                verticalAlignment: Text.AlignTop
                wrapMode: Text.WordWrap
                font.bold: true
                font.pixelSize: 16
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
            visible: !isLoading && count > 0
            text: isError ? errorString : 'Scroll Down Or Click Me for more'
            font.family: myFont.name
            font.pixelSize: 24
            color: theme.disabledText
            wrapMode: Text.Wrap
            MouseArea {
                anchors.fill: parent
                onClicked: tileClicked()
            }
        }
    }

    onAtYEndChanged: {
        // on indexChange count becomes 0 and this method is called by setCategoryIndex
        if (atYEnd && count > 0) {
            loadMore()
        }
    }
}
