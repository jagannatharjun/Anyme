import QtQuick 2.0
import QtQuick.Controls 2.13

Rectangle {
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

    anchors.fill: parent
    color: theme.background
    property alias myFont: fonts.bigShoulder

    Rectangle {
        anchors.fill: parent
        z: 10
        color: theme.background
        visible: !animeReq.isCompleted

        BusyIndicator {
            anchors.centerIn: parent
            running: animeReq.isLoading
        }

        Text {
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            visible: animeReq.isError
            text: animeReq.errorString
            font.family: myFont.name
            font.pixelSize: 24
            color: theme.disabledText
        }
    }

    Connections {
        target: animeReq
        onCompleted: {
                pageLoader.setSource(animeDetailsQmlSource)

        }
    }

    Loader {
        anchors.fill: parent
        id : pageLoader
    }
}
