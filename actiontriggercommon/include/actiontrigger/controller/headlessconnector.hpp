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

#ifndef HEADLESSCONNECTOR_HPP_
#define HEADLESSCONNECTOR_HPP_

#include <QObject>
#include <QDataStream>
#include <QtNetwork/QTcpSocket>
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/ipcproto.hpp"
#include <vector>
#include <queue>
#include <string>
#include <cmath>
#include <QTimer>
#include <unistd.h>

namespace actiontrigger
{
    class HeadlessConnector: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (bool isConnected READ getIsConnected NOTIFY notifyChangeIsConnected FINAL)
    public:
        static const bool HEADLESS_ENABLED;

        static HeadlessConnector* get();

        bool getIsConnected() const;

        Q_INVOKABLE
        void invoke();Q_INVOKABLE
        void start();Q_INVOKABLE
        void kill();Q_INVOKABLE

        void procedureStart(int id);
        void procedureStop(int id);

    public Q_SLOTS:
        // Connection handling
        void socketError(QAbstractSocket::SocketError);
        void socketConnected();
        void socketRead();
        void socketDisconnected();
        void reconnect();

    Q_SIGNALS:
        void notifyChangeIsConnected();

        void procedureStopped(int id);
        void procedureRunning(int id);

    private:
        std::queue<QByteArray*> outQueue;
        HeadlessConnector();
        virtual ~HeadlessConnector();
        QTimer reconnectWatchdog;

        static Logger* LOG;
        static HeadlessConnector* instance;
        bool verified;
        QTcpSocket* socket;

        void connect();
        void write(QByteArray* data);
    };
} /* namespace actiontriggerservice */

#endif /* HEADLESSCONNECTOR_HPP_ */
