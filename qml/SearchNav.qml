import QtQuick 2.13
import QtQuick.Controls 2.13

import QtQuick.Dialogs 1.1

Item {
    property int sepMargin: 6

    Image {
        source: 'qrc:/icon/arrow_forward-24px.png'
        x: 14
        y: 18
        fillMode: Image.PreserveAspectFit
        width: 16
        height: 16
        rotation: 180
        MouseArea {
            anchors.fill: parent
            onClicked: leftBarView.pop()
        }
        id: arrowIcon
    }

    Flickable {
        width: parent.width
        anchors.top: arrowIcon.bottom
        anchors.topMargin: 20
        anchors.bottom: startSearchBtn.top
        anchors.margins: sepMargin
        onHeightChanged: print(height)
        contentHeight: container.childrenRect.height
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        ScrollBar.vertical: ScrollBar {
            width: 8
            visible: true
        }

        Item {
            anchors.fill: parent
            id: container
            NavigationSeperator {
                id: upSep
            }

            Image {
                anchors.top: upSep.bottom
                anchors.margins: sepMargin
                source: "qrc:/icon/NavigationIcons/search.png"
                id: searchIcon
                x: 14
            }

            TextEdit {
                y: searchIcon.y
                anchors.left: searchIcon.right
                anchors.right: parent.right
                anchors.margins: sepMargin + 4
                focus: true
                z: 2
                font.family: fonts.bigShoulderRegular.name
                font.pixelSize: 20
                color: theme.primaryForeground
                id: searchText
                selectByMouse: true
                selectedTextColor: theme.accentForeground
                selectionColor: theme.accent
                wrapMode: TextEdit.Wrap

                onTextChanged: animesearch.keyWord = text

                Text {
                    text: 'Search...'
                    color: theme.disabledText
                    visible: !searchText.text
                    font: searchText.font
                }
            }

            NavigationSeperator {
                anchors.top: searchText.bottom
                anchors.margins: sepMargin
                id: downSep
            }

            FilterList {
                anchors.top: downSep.bottom
                width: parent.width
                anchors.margins: sepMargin
                model: animesearch.status()
                text: 'Status'
                multiSelect: true
                id: statusFilter
            }

            FilterList {
                anchors.top: statusFilter.bottom
                width: parent.width
                anchors.margins: sepMargin
                model: animesearch.types()
                text: 'Types'
                multiSelect: true
                id: typeFitler
            }

            FilterList {
                anchors.top: typeFitler.bottom
                width: parent.width
                anchors.margins: sepMargin
                model: animesearch.rating()
                text: 'Rating'
                multiSelect: true
                id: ratingFilter
            }

            FilterList {
                anchors.top: ratingFilter.bottom
                width: parent.width
                anchors.margins: sepMargin
                model: animesearch.genre()
                text: 'Genre'
                multiSelect: true
                id: genreFilter
            }

            FilterList {
                anchors.top: genreFilter.bottom
                width: parent.width
                anchors.margins: sepMargin
                model: animesearch.sort()
                text: 'Sort'
                multiSelect: false
                id: sortFilter
            }

            FilterList {
                anchors.top: sortFilter.bottom
                width: parent.width
                anchors.margins: sepMargin
                model: animesearch.orderBy()
                text: 'Order By'
                multiSelect: false
                id: orderByFilter
            }
        }
    }

    SearchNavBtn {
        btnText: 'Search'
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.margins: parent.sepMargin
        onClicked: {
            if (!animesearch.isValidKeyword()) {
                errorDialog.open()
                return;
            }

            animesearch.typeIndices = typeFitler.indexes
            animesearch.statusIndices = statusFilter.indexes
            animesearch.ratingIndices = ratingFilter.indexes
            animesearch.genreIndices = genreFilter.indexes
            animesearch.orderByIndex = orderByFilter.indexes
            animesearch.sortIndex = sortFilter.indexes
            animesearch.beginSearch()
        }
        z: 4
        id: startSearchBtn
    }

    MessageDialog {
        id: errorDialog
        title: "Error"
        text: "Invalid Keyword for Seach. A Keyword should contain more then 2 words or empty to do filter based searching"
    }
}
