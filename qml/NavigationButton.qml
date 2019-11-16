import QtQuick 2.13
import QtGraphicalEffects 1.13

NavigationObject {

    property string btnText: 'Home'

    Image {
        source: 'qrc:/icon/arrow_forward-24px.png'
        anchors.verticalCenter: parent.verticalCenter
        fillMode: Image.PreserveAspectFit
        width: 16
        height: 16
        x: 225
        id: icon
    }

    ColorOverlay {
        source: icon
        color: parent.foreground
        anchors.fill: icon
    }

    Text {
        font.pixelSize: 26
        font.family: fonts.bigShoulder.name
        color: parent.foreground
        x: 19.625
        anchors.verticalCenter: parent.verticalCenter
        text: btnText
    }

}
