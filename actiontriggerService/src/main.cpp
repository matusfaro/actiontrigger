/*
 * ActionTrigger
 * Copyright (C) 2013  Matus Faro
 *
 * This file is part of ActionTrigger.
 *
 * ActionTrigger is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ActionTrigger is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ActionTrigger.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <bb/Application>

#include <QLocale>
#include <QTranslator>
#include "actiontriggerservice.hpp"

// Declare types
#include <QMetaType>
#include <QtNetwork/qabstractsocket.h>
Q_DECLARE_METATYPE(QAbstractSocket::SocketState)
Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
#include <string>
Q_DECLARE_METATYPE (std::string)

using namespace bb;

int main(int argc, char **argv)
{
    qRegisterMetaType<QAbstractSocket::SocketState>();
    qRegisterMetaType<QAbstractSocket::SocketError>();

    Application app(argc, argv);

    actiontrigger::ActionTriggerService* service = new actiontrigger::ActionTriggerService();
    service->start();

    // Enter the application main event loop.
    return Application::exec();
}
