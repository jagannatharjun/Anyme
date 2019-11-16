import QtQuick 2.0

Item {
    property string buttonTxt: 'None'
    property color foregroundColor: theme.primaryDark
    property color backgroundColor: theme.primaryDarkForeground

    signal clicked
    signal selected
    signal unselected

    function unselect() {
        state = 'Normal'
    }

    function select() {
        state = 'Selected'
    }

    onStateChanged: {
        if (state == 'Normal')
            unselected()
        else if (state == 'Selected')
            selected();
    }

    id: btn
    height: 44

    anchors.leftMargin: 12
    anchors.rightMargin: 12

    onClicked: {
        select()
    }

    Rectangle {
        anchors.right: parent.right
        anchors.left: parent.left
        height: 0
        color: theme.disabledText
        id: seperator
    }

    Rectangle {
        x: -parent.anchors.leftMargin
        anchors.top: seperator.bottom
        width: parent.width + parent.anchors.leftMargin + parent.anchors.rightMargin
        height: parent.height
        color: backgroundColor
    }

    Text {
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 6
        font.family: fonts.bigShoulder.name
        color: foregroundColor
        text: buttonTxt
        font.pixelSize: 26
        id: txt
    }

    MouseArea {
        anchors.fill: parent
        id: btnMouseArea
        onClicked: btn.clicked()
        hoverEnabled: true
    }

    states: [
        State {
            name: "Selected"
            PropertyChanges {
                target: btn
                backgroundColor: theme.primary
                foregroundColor: theme.primaryForeground
            }
        },
        State {
            name: "Normal"
            PropertyChanges {
                target: btn
                backgroundColor: theme.primaryDark
                foregroundColor: theme.primaryDarkForeground
            }
        }
    ]

    transitions: [
        Transition {
            from: "Normal"
            to: "Selected"
            reversible: true

            ColorAnimation {
                duration: 200
            }
        }
    ]
}
