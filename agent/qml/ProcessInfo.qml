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

Column {
    property alias model: repeater.model

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
