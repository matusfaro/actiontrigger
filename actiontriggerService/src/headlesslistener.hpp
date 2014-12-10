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

#ifndef HEADLESSLISTENER_HPP_
#define HEADLESSLISTENER_HPP_

#include <QObject>
#include "headlesscontroller.hpp"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/ipcproto.hpp"
#include <bb/Application>
#include <QTimer>
#include <unistd.h>

// Memory check
#include <sys/procfs.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdint.h>

namespace actiontrigger
{

    class HeadlessListener: public QObject
    {
    Q_OBJECT

    public:
        static HeadlessListener* get();

        bool getIsConnected() const;
        void start();

    public Q_SLOTS:
        // Use to get notified if a procedure stops
        void notifyProcedureStopped(int id);
        // Listener handling
        void serverNewConnection();
        // Connection handling
        void socketError(QAbstractSocket::SocketError);
        void socketConnected();
        void socketRead();
        void socketDisconnected();
        // Self check if headless should shutdown
        void watchdogRun();

    private:
        static Logger* LOG;
        static HeadlessListener* instance;

        QTcpServer* server;
        QTcpSocket* socket;
        bool verified;
        HeadlessController controller;
        QTimer watchdog;

        void reconnect();
        void write(QByteArray* data);

        int64_t measureMem();

        HeadlessListener();
        virtual ~HeadlessListener();
    };

} /* namespace actiontrigger */

#endif /* HEADLESSLISTENER_HPP_ */
