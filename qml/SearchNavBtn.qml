import QtQuick 2.0

NavigationObject {
    property string btnText: 'value'

    width: 80
    Rectangle {
        anchors.fill: parent
        color: parent.background
        border.color: theme.background
        border.width: !mouseArea.containsMouse ? 2 : 0
    }

    Text {
        font.pixelSize: 20
        font.family: fonts.bigShoulder.name
        color: parent.foreground
        anchors.centerIn: parent
        text: btnText
    }
}
