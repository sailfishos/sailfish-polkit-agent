/**
 * Sailfish polkit Agent: GUI Agent
 * Copyright (C) 2016 Jolla Ltd.
 * Contact: Lucien Xu <sfietkonstantin@free.fr>
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

#ifndef QOBJECTPTR_H
#define QOBJECTPTR_H

#include <QObject>
#include <memory>

namespace
{

class QObjectDeleter
{
public:
    void operator()(QObject *object) const
    {
        object->deleteLater();
    }
};

}

template<class T>
using QObjectPtr = std::unique_ptr<T, QObjectDeleter>;

#endif // QOBJECTPTR_H

