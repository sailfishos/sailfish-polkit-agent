/**
 * Sailfish polkit Agent: Configuration file
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

#ifndef SAILFISH_POLKIT_AGENT_CONFIGURATION_H
#define SAILFISH_POLKIT_AGENT_CONFIGURATION_H


// Object path of the polkit listener object running in the agent process
#define SAILFISH_POLKIT_AGENT_PATH "/org/sailfishos/polkit/AuthenticationAgent"

// D-Bus name of the polkit daemon helper
#define SAILFISH_POLKIT_DAEMON_NAME "org.sailfishos.polkit.daemon"

// D-Bus path of the polkit daemon helper
#define SAILFISH_POLKIT_DAEMON_PATH "/org/sailfishos/polkit/daemon"

// D-Bus interface of the polkit daemon helper
#define SAILFISH_POLKIT_DAEMON_INTF "org.sailfishos.polkit.daemon"

// D-Bus method (of the polkit daemon helper) for sending a response
#define SAILFISH_POLKIT_DAEMON_RESPONSE_METHOD "sendResponse"

#endif /* SAILFISH_POLKIT_AGENT_CONFIGURATION_H */
