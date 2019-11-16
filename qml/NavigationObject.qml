import QtQuick 2.13

Rectangle {
    property color background: theme.primary
    property color foreground: theme.primaryForeground
    property bool isSelected: false
    signal clicked

    onIsSelectedChanged: {
        state = isSelected ? 'Selected' : 'Normal'
    }

    Component.onCompleted: state = isSelected ? 'Selected' : 'Normal'

    width: 256
    height: 40

    id: btn

    color: background

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: btn.clicked()
        onContainsMouseChanged: btn.state = containsMouse
                                || isSelected ? 'Selected' : 'Normal'
    }

    states: [
        State {
            name: "Normal"
            PropertyChanges {
                target: btn
                background: theme.primary
                foreground: theme.primaryForeground
            }
        },
        State {
            name: "Selected"
            PropertyChanges {
                target: btn
                background: theme.primaryDark
                foreground: theme.primaryDarkForeground
            }
        }
    ]

    transitions: [
        Transition {
            from: "Normal"
            to: "Selected"
            reversible: true
            ColorAnimation {
                duration: 96
            }
        }
    ]
}
