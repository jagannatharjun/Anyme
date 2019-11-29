import QtQuick 2.0
import QtGraphicalEffects 1.13
import QtQuick.Controls 2.13

Rectangle {
    anchors.fill: parent

    MyFonts {
        id: fonts
    }

    FontLoader {
        id: bigShoulderRegular
        source: 'qrc:/font/BigShouldersDisplay-Regular.ttf'
    }

    FontLoader {
        id: oswald
        source: 'qrc:/font/Oswald-Medium.ttf'
    }

    FontLoader {
        id: oswaldLight
        source: 'qrc:/font/Oswald-Light.ttf'
    }

    color: theme.background
    property alias myFont: fonts.bigShoulder

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 280
        color: theme.primary
        id: leftBar

        Column {

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            spacing: 6

            Image {
                source: anime.imageUrl
                id: animeImage
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                width: animeImage.width
                anchors.horizontalCenter: parent.horizontalCenter
                text: anime.title
                font.family: myFont.name
                font.pointSize: 18
                color: theme.primaryForeground
                horizontalAlignment: Text.AlignHCenter
                id: animeTitle
                wrapMode: Text.WordWrap
            }
        }
    }

    DropShadow {
        source: leftBar
        anchors.fill: leftBar
        radius: 8
        samples: 17
        transparentBorder: true
    }

    Text {
        anchors.bottom: topLabels.top
        anchors.left: topLabels.left
        text: anime.japaneseTitle
        font.pointSize: 12
        color: theme.foreground
        id: japaneseTitle
    }

    Text {
        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.right: parent.right
        anchors.rightMargin: 16
        text: anime.rating
        font.family: bigShoulderRegular.name
        font.pixelSize: 21
        font.weight: Font.Medium
        color: theme.primary
        id: ratingText
    }

    Rectangle {
        x: ratingText.x - border.width - 2
        y: ratingText.y - border.width - 1
        width: ratingText.width + (border.width * 2) + 4
        height: ratingText.height + (border.width * 2) + 2
        color: 'transparent'
        border.color: ratingText.color
        border.width: 2
    }

    Row {
        x: 306
        y: 44
        spacing: 18
        id: topLabels

        property int selectedIndex: 0
        onSelectedIndexChanged: {
            if (selectedIndex == 0)
                contentView.pop()
            else
                contentView.push(episodesView)
        }

        Repeater {
            model: ['Series', 'Episodes']
            Text {
                text: modelData
                font.pixelSize: 34
                font.family: bigShoulderRegular.name
                font.weight: Font.DemiBold

                color: topLabels.selectedIndex === index ? theme.foreground : theme.disabledText
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        topLabels.selectedIndex = index
                    }
                }
            }
        }
    }

    StackView {
        id: contentView
        anchors.top: topLabels.bottom
        anchors.left: topLabels.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        initialItem: seriesView
        clip: true
    }

    Flickable {
        id: seriesView

        contentHeight: contentContainer.childrenRect.height + 100
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar {
            visible: true
        }

        Item {
            id: contentContainer
            anchors.fill: parent

            Row {
                id: stats
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.top: parent.top
                anchors.topMargin: 36
                anchors.bottomMargin: 36
                spacing: 40
                Repeater {
                    property list<QtObject> paramModel: [
                        QtObject {
                            property string title: anime.score
                            property string caption: 'Score'
                        },
                        QtObject {
                            property string title: '#' + anime.rank
                            property string caption: 'Rank'
                        },
                        QtObject {
                            property string title: '#' + anime.popularity
                            property string caption: 'Popularity'
                        },
                        QtObject {
                            property string title: anime.members
                            property string caption: 'Members'
                        }
                    ]
                    model: paramModel

                    Column {
                        width: titleText.width
                        Text {
                            text: modelData.title
                            id: titleText
                            font.family: bigShoulderRegular.name
                            font.pixelSize: 38
                            font.weight: Font.DemiBold
                            color: theme.foreground
                        }

                        Text {
                            text: modelData.caption
                            anchors.horizontalCenter: titleText.horizontalCenter
                            font.family: bigShoulderRegular.name
                            font.pixelSize: 25
                            color: theme.foreground
                        }
                    }
                }
            }

            Column {
                anchors.top: stats.bottom
                anchors.topMargin: 36
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: 2

                Text {
                    function chose(list, suffix, prefix) {
                        if (!suffix)
                            suffix = ''
                        if (!prefix)
                            prefix = ' | '
                        for (var index = 0; index < list.length; index++) {
                            if (list[index])
                                return prefix + list[index] + suffix
                        }
                        return ''
                    }
                    text: anime.status + chose(
                              [anime.broadcast, anime.premiered, anime.duration]) + chose(
                              [anime.episodes], ' Episodes')
                    font.family: oswald.name
                    font.pixelSize: 18
                    font.weight: Font.DemiBold
                    color: theme.foreground
                    id: infoText
                }

                Text {
                    width: parent.width - 20
                    text: anime.synopsis + (anime.background ? '\n\n' + anime.background : '')
                    font.family: oswaldLight.name
                    font.weight: Font.Light
                    font.pixelSize: 16
                    font.letterSpacing: 0
                    clip: true
                    wrapMode: Text.WordWrap
                    color: theme.foreground
                }
            }
        }
    }

    Component {
        id: episodesView
        Rectangle {
            width: 100
            height: 200
            x: 100
            color: 'red'
        }
    }
}
