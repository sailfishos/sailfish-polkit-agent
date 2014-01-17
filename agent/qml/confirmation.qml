import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow {
    initialPage: Page { /* empty page */ }

    Component.onCompleted: confirmationDialog.open(false, true)

    cover: CoverBackground {
        Label {
            anchors {
                left: parent.left
                right: parent.right
                verticalCenter: parent.verticalCenter
                margins: Theme.paddingSmall
            }

            wrapMode: Text.WordWrap
            text: confirmation.message
            font.pixelSize: Theme.fontSizeSmall
        }

        CoverActionList {
            CoverAction {
                iconSource: 'image://theme/icon-cover-cancel'
                onTriggered: confirmation.setConfirmationResult(false)
            }

            CoverAction {
                iconSource: 'image://theme/icon-cover-answer'
                onTriggered: confirmation.setConfirmationResult(true)
            }
        }
    }

    Dialog {
        id: confirmationDialog

        onAccepted: confirmation.setConfirmationResult(true)
        onRejected: confirmation.setConfirmationResult(false)

        SilicaFlickable {
            anchors.fill: parent

            Column {
                spacing: Theme.paddingMedium

                anchors {
                    left: parent.left
                    right: parent.right
                    margins: Theme.paddingLarge
                }

                DialogHeader {
                    width: parent.width
                    title: 'System Permission'
                    acceptText: 'Allow'
                    cancelText: 'Deny'
                }

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    wrapMode: Text.WordWrap
                    text: confirmation.message
                }

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    wrapMode: Text.WrapAnywhere
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    text: confirmation.action
                }
            }
        }
    }
}
