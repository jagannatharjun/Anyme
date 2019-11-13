import QtQuick 2.0
import QtGraphicalEffects 1.13
import QtQuick.Controls 2.13
import QtQuick.Controls.Material 2.13
import QtQuick.Layouts 1.13

Rectangle {
    anchors.fill: parent
    color: Material.background

    Material.theme: Material.Dark

    MyFonts {
        id: fonts
    }

    property alias myFont: fonts.bigShoulder

    Rectangle {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width: 280
        color: Material.accent
        id: leftBar

        Column {

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter


            Image {
                source: anime.imageUrl
                id : animeImage
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Text {
                width: animeImage.width
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.topMargin: 4
                text: anime.title
                font.family: myFont.name
                font.pointSize: 18
                color: Material.foreground
                horizontalAlignment: Text.AlignHCenter
                id: animeTitle
                wrapMode: Text.WordWrap
            }
        }
    }

    DropShadow {
        source: leftBar
        anchors.fill: leftBar
        radius: 16
        samples: 33
        transparentBorder: true
    }

    Row {
        anchors.left: leftBar.right
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 24
        spacing: 18
        id: topLabels
        property int selectedIndex: 0
        Repeater {
            model: ['Series', 'Episodes']
            Text {
                text: modelData
                font.pointSize: 24
                color: topLabels.selectedIndex
                       === index ? Material.foreground : Material.backgroundDimColor
                font.family: myFont.name
                MouseArea {
                    anchors.fill: parent
                    onClicked: topLabels.selectedIndex = index
                }
            }
        }
    }

    StackLayout {}
}
