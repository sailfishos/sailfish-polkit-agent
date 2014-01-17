import QtQuick 2.0
import Sailfish.Silica 1.0

Column {
    property alias model: repeater.model

    anchors {
        left: parent.left
        right: parent.right
    }

    Repeater {
        id: repeater

        delegate: Item {
            height: keyLabel.height

            anchors {
                left: parent.left
                right: parent.right
            }

            Label {
                id: keyLabel

                anchors {
                    left: parent.left
                }

                text: modelData[0] + ': '
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.primaryColor
            }

            Label {
                id: valueLabel

                anchors {
                    left: keyLabel.right
                    right: parent.right
                }

                wrapMode: Text.Wrap
                text: modelData[1]
                font.pixelSize: Theme.fontSizeExtraSmall
                color: Theme.highlightColor
            }
        }
    }
}
