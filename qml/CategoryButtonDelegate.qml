import QtQuick 2.13
import QtGraphicalEffects 1.13

NavigationObject {
    id: btn

    Image {
        id: icon
        source: iconUrl
        width: 16
        height: 16
        anchors.verticalCenter: parent.verticalCenter
        x: 48
    }

    ColorOverlay {
        anchors.fill: icon
        source: icon
        color: foreground
    }

    Text {
        x: 86
        anchors.verticalCenter: parent.verticalCenter
        font.family: fonts.bigShoulder.name
        font.pixelSize: 22
        color: foreground
        text: btnText
    }
}
