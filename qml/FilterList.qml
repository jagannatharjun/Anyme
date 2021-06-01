import QtQuick 2.12
import QtGraphicalEffects 1.12

Item {

    property alias list: filterList
    property alias model: filterList.model
    property alias text: btn.btnText
    property bool multiSelect: false
    property var indexes: multiSelect ? [] : 0

    height: childrenRect.height

    id: f

    NavigationButton {
        icon {
            source: 'qrc:/icon/drop-down.png'
        }
        id: btn
        onClicked: f.state = f.state == 'Collapsed' ? 'Visible' : 'Collapsed'
    }

    Component.onCompleted: f.state = 'Collapsed'

    ListView {
        anchors.top: btn.bottom
        anchors.topMargin: parent.sepMargin
        anchors.left: parent.left
        anchors.right: parent.right
        id: filterList
        clip: true
        delegate: NavigationObject {
            onClicked: {
                if (multiSelect) {
                    if (isSelected) {
                        indexes.splice(indexes.indexOf(index), 1)
                        indexes = indexes
                    } else {
                        indexes.push(index)
                        indexes = indexes
                    }
                } else {
                    indexes = index
                }
            }

            isSelected: multiSelect ? indexes.indexOf(
                                          index) != -1 : index == indexes
            Image {
                id: icon
                source: 'qrc:/icon/check-24px.svg'
                width: 16
                height: 16
                anchors.verticalCenter: parent.verticalCenter
                x: 40
            }

            ColorOverlay {
                anchors.fill: icon
                source: icon
                color: foreground
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                font.family: fonts.bigShoulder.name
                font.pixelSize: 18
                color: foreground
                text: model.modelData.charAt(0).toUpperCase() +  model.modelData.slice(1)
            }
        }
    }

    NavigationSeperator {
        anchors.topMargin: sepMargin
        anchors.top: filterList.bottom
        id: bottomSep
    }

    states: [
        State {
            name: "Visible"
            PropertyChanges {
                target: filterList
                height: 40 * filterList.count
            }
            PropertyChanges {
                target: btn.icon
                rotation: 0
            }
        },
        State {
            name: "Collapsed"
            PropertyChanges {
                target: filterList
                height: 0
            }
            PropertyChanges {
                target: btn.icon
                rotation: 180
            }
        }
    ]

    transitions: [
        Transition {
            from: "Visible"
            to: "Collapsed"
            reversible: true
            NumberAnimation {
                properties: "height,rotation"
                duration: 156
            }
        }
    ]
}
