/**
 * Sailfish polkit Agent: GUI Agent
 * Copyright (C) 2014 Jolla Ltd.
 * Contact: Thomas Perl <thomas.perl@jolla.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB. If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 **/

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
        }
    }

    Dialog {
        id: confirmationDialog

        onAccepted: confirmation.setConfirmationResult(true)
        onRejected: confirmation.setConfirmationResult(false)

        SilicaFlickable {
            anchors.fill: parent
            contentHeight: questionColumn.height

            VerticalScrollDecorator {}

            Column {
                id: questionColumn
                spacing: Theme.paddingSmall

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
                    text: [
                        confirmation.action,
                        'Identity: ' + confirmation.identity
                    ].join('\n')
                }

                SectionHeader {
                    text: 'Subject'
                }

                ProcessInfo {
                    model: [
                        ['PID', confirmation.subject.pid],
                        ['User', confirmation.subject.user],
                        ['Group', confirmation.subject.group],
                        ['Application', confirmation.subject.exec],
                        ['Command', confirmation.subject.cmdline.join(' ')]
                    ]
                }

                SectionHeader {
                    text: 'Caller'
                }

                ProcessInfo {
                    model: [
                        ['PID', confirmation.caller.pid],
                        ['User', confirmation.caller.user],
                        ['Group', confirmation.caller.group],
                        ['Application', confirmation.caller.exec],
                        ['Command', confirmation.caller.cmdline.join(' ')]
                    ]
                }
            }
        }
    }
}
