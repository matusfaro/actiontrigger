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

#include <src/headlesslistener.hpp>
#include "actiontrigger/model/databasefactory.hpp"

namespace actiontrigger
{
    Logger* HeadlessListener::LOG = LoggerFactory::getInstance("HeadlessListener");
    HeadlessListener* HeadlessListener::instance = NULL;

    HeadlessListener* HeadlessListener::get()
    {
        if (!HeadlessListener::instance) {
            HeadlessListener::instance = new HeadlessListener();
        }
        return HeadlessListener::instance;
    }

    HeadlessListener::HeadlessListener() :
            server(new QTcpServer()), socket(NULL), verified(false)
    {
        QObject::connect(&controller, SIGNAL(procedureStopping(int)), this,
                SLOT(notifyProcedureStopped(int)));
        QObject::connect(server, SIGNAL(newConnection()), this, SLOT(serverNewConnection()));

        watchdog.setInterval(10 * 1000);
        watchdog.setSingleShot(false);
        QObject::connect(&watchdog, SIGNAL(timeout()), this, SLOT(watchdogRun()));
        watchdog.start();
    }

    HeadlessListener::~HeadlessListener()
    {
        QObject::disconnect(&controller, SIGNAL(procedureStopping(int)), this,
                SLOT(notifyProcedureStopped(int)));
        QObject::disconnect(server, SIGNAL(newConnection()), this, SLOT(serverNewConnection()));
    }

    void HeadlessListener::serverNewConnection()
    {
        if (server->hasPendingConnections()) {
            reconnect();
            socket = server->nextPendingConnection();
            socket->setReadBufferSize(1024);
            QObject::connect((QAbstractSocket*) socket, SIGNAL(disconnected()), this,
                    SLOT(socketDisconnected()));
            QObject::connect((QAbstractSocket*) socket, SIGNAL(connected()), this,
                    SLOT(socketConnected()));
            QObject::connect((QAbstractSocket*) socket, SIGNAL(error(QAbstractSocket::SocketError)),
                    this, SLOT(socketError(QAbstractSocket::SocketError)));
            QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
            LOG->debug(
                    SSTR(
                            "serverNewConnection from " << socket->peerAddress().toString().toStdString() << ":" << socket->peerPort()));
        } else {
            LOG->warning("serverNewConnection no connections are pending");
        }
    }

    void HeadlessListener::socketError(QAbstractSocket::SocketError socketError)
    {
        if (!socket) {
            LOG->warning("Socket error on null socket");
            return;
        }
        LOG->debug(SSTR("Socket error: " << socketError));
        socketDisconnected();
    }

    void HeadlessListener::socketConnected()
    {
        if (!socket) {
            LOG->warning("socketConnected on null socket");
            return;
        }
        LOG->debug(SSTR("socketConnected"));
    }

    void HeadlessListener::socketDisconnected()
    {
        LOG->debug(SSTR("socketDisconnected"));
        if (socket) {
            QObject::disconnect((QAbstractSocket*) socket, SIGNAL(disconnected()), this,
                    SLOT(socketDisconnected()));
            QObject::disconnect((QAbstractSocket*) socket, SIGNAL(connected()), this,
                    SLOT(socketConnected()));
            QObject::disconnect((QAbstractSocket*) socket,
                    SIGNAL(error(QAbstractSocket::SocketError)), this,
                    SLOT(socketError(QAbstractSocket::SocketError)));
            QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));
        }
        socket = NULL;
        verified = false;
    }

    void HeadlessListener::watchdogRun()
    {
        LOG->debug(SSTR("watchdog: currentHeap: " << measureMem()));
        if (getIsConnected()) {
            LOG->trace("checkForShutdown: connected to UI");
        } else if (controller.numRunningProcedures() == 0) {
            LOG->debug("checkForShutdown: no running procedures, shutting down.");
            bb::Application::instance()->quit();
        } else {
            LOG->trace(
                    SSTR(
                            "checkForShutdown: " << controller.numRunningProcedures() << " procedures running, staying in background."));
        }
    }

    int64_t HeadlessListener::measureMem()
    {
        int rc, n, i, num;
        int64_t memsize = -1ll;
        procfs_mapinfo *maps = NULL;
        int fd = open("/proc/self/as", O_RDONLY);
        if (fd == -1)
            return -1ll;
        do {
            rc = devctl(fd, DCMD_PROC_PAGEDATA, NULL, 0, &num);
            if (rc)
                goto cleanup;
            maps = (procfs_mapinfo*) realloc(maps, num * sizeof(*maps));
            if (!maps)
                goto cleanup;
            // pre-subtract the allocation we just made, rounded up to page size
            memsize = 0ll - ((num * sizeof(*maps) + __PAGESIZE - 1) & ~(__PAGESIZE - 1));
            rc = devctl(fd, DCMD_PROC_PAGEDATA, maps, num * sizeof(*maps), &n);
            if (rc) {
                memsize = -1ll;
                goto cleanup;
            }
        } while (n > num);
        for (i = 0; i < n; i++) {
            if ((maps[i].flags & (MAP_ANON | MAP_STACK | MAP_ELF | MAP_TYPE))
                    == (MAP_ANON | MAP_PRIVATE)) {
                memsize += maps[i].size;
            }
        }

        cleanup: close(fd);
        free(maps);

        return memsize;
    }

    void HeadlessListener::reconnect()
    {
        QTcpSocket* lastSocket = socket;
        socket = NULL;
        if (lastSocket != NULL) {
            LOG->debug(
                    SSTR(
                            "Disconnecting from " << lastSocket->peerAddress().toString().toStdString() << ":" << lastSocket->peerPort()));

            QObject::disconnect((QAbstractSocket*) socket, SIGNAL(disconnected()), this,
                    SLOT(socketDisconnected()));
            QObject::disconnect((QAbstractSocket*) socket, SIGNAL(connected()), this,
                    SLOT(socketConnected()));
            QObject::disconnect((QAbstractSocket*) socket,
                    SIGNAL(error(QAbstractSocket::SocketError)), this,
                    SLOT(socketError(QAbstractSocket::SocketError)));
            QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(socketRead()));

            lastSocket->disconnectFromHost();
            // Should this be here? Will this be called as part of disconnectFromHost?
            socketDisconnected();
        }
    }

    void HeadlessListener::socketRead()
    {
        LOG->debug(SSTR("socketRead bytes: " << socket->bytesAvailable()));
        if (socket->bytesAvailable() == 0) {
            return;
        }
        if (socket->bytesAvailable() >= sizeof(ipc_header)) {
            ipc_header msg;
            socket->read(msg.data, sizeof(ipc_header));

            LOG->debug(SSTR("RECV: " << msg.val.command));
            do {
                // Wait for more data
                if (socket->bytesAvailable() < msg.val.size) {
                    LOG->debug(
                            SSTR(
                                    "RECV: Waiting for more data for command: " << msg.val.command << " bytes available: " << socket->bytesAvailable() << " expected bytes: " << msg.val.size));
                    usleep(1000000); // TODO Async
                    if (socket->bytesAvailable() < msg.val.size) {
                        LOG->error(
                                SSTR(
                                        "RECV: Timed out receiving more data for command: " << msg.val.command << " bytes available: " << socket->bytesAvailable() << " expected bytes: " << msg.val.size));
                        reconnect();
                        return;
                    }
                }
                // Handle data
                switch (msg.val.command) {
                    case Initialize:
                        if (msg.val.size != 0) {
                            LOG->error("RECV: Size is set for Initialize command");
                            reconnect();
                            return;
                        }
                        LOG->debug("Connection verified");
                        verified = true;
                        {
                            ipc_header envelopeHeader;
                            envelopeHeader.val.command = Initialize;
                            envelopeHeader.val.size = 0;
                            QByteArray* data = new QByteArray(envelopeHeader.data,
                                    sizeof(envelopeHeader.data));

                            LOG->debug(
                                    SSTR(
                                            "SEND: cmd=" << envelopeHeader.val.command << " size=" << envelopeHeader.val.size));
                            write(data);
                            delete data;
                        }
                        LOG->error("Sending list of running procedures");
                        {
                            QByteArray* data = new QByteArray();
                            ipc_header envelopeHeader;
                            envelopeHeader.val.command = ProcedureRunningList;
                            envelopeHeader.val.size = 0;

                            for (std::map<int, Procedure*>::iterator it =
                                    controller.getProcedures()->begin();
                                    it != controller.getProcedures()->end(); ++it) {
                                if ((*it).second->isRunning()) {
                                    LOG->debug(
                                            SSTR(
                                                    "SEND: ProcedureRunningList: id: " << (*it).first));
                                    envelopeHeader.val.size += sizeof(ipc_integer);
                                    ipc_integer envelopeId;
                                    envelopeId.number = (*it).first;
                                    data->push_front(
                                            QByteArray::fromRawData(envelopeId.data,
                                                    sizeof(ipc_integer)));
                                }
                            }

                            if (envelopeHeader.val.size > 0) {
                                data->push_front(
                                        QByteArray::fromRawData(envelopeHeader.data,
                                                sizeof(ipc_header)));

                                LOG->debug(
                                        SSTR(
                                                "SEND: cmd=" << envelopeHeader.val.command << " size=" << envelopeHeader.val.size));
                                write(data);
                            }
                            delete data;
                        }
                        return;

                    case Shutdown:
                        if (msg.val.size != 0) {
                            LOG->error("RECV: Size is set for ShuttingDown command");
                            reconnect();
                            return;
                        }
                        LOG->error("Received ShuttingDown command");
                        bb::Application::instance()->quit();
                        return;

                    case ProcedureStart:
                        if (msg.val.size < sizeof(ipc_integer)) {
                            LOG->error(
                                    SSTR(
                                            "RECV: Size is less than size of ipc_integer for ProcedureStart command: " << msg.val.size));
                            reconnect();
                            return;
                        } else {
                            // Handle data
                            ipc_integer envelopeId;
                            socket->read(envelopeId.data, sizeof(envelopeId));
                            LOG->debug(SSTR("RECV: ProcedureStart: " << envelopeId.number));
                            controller.startProcedure(envelopeId.number);
                            msg.val.size -= sizeof(ipc_integer);
                        }
                        break;

                    case ProcedureStop:
                        if (msg.val.size < sizeof(int)) {
                            LOG->error(
                                    SSTR(
                                            "RECV: Size is less than size of int for ProcedureStop command: " << msg.val.size));
                            reconnect();
                            return;
                        } else {
                            // Handle data
                            ipc_integer envelopeId;
                            socket->read(envelopeId.data, sizeof(envelopeId));
                            LOG->debug(SSTR("RECV: ProcedureStop: " << envelopeId.number));
                            controller.stopProcedure(envelopeId.number);
                            msg.val.size -= sizeof(ipc_integer);
                        }
                        break;

                    case ShuttingDown:
                    case ProcedureStopped:
                    case ProcedureRunningList:
                        LOG->debug(SSTR("Server only command: " << msg.val.command));
                        reconnect();
                        return;

                    default:
                        LOG->debug(SSTR("Unknown command: " << msg.val.command));
                        reconnect();
                        return;
                }
            } while (msg.val.size > 0);
        }
    }

    bool HeadlessListener::getIsConnected() const
    {
        return socket && socket->isOpen() && verified;
    }

    void HeadlessListener::start()
    {
        server->listen(QHostAddress::LocalHost, HEADLESS_SERVER_PORT);
    }

    void HeadlessListener::notifyProcedureStopped(int id)
    {
        if (getIsConnected()) {
            LOG->debug(SSTR("Procedure " << id << " stopped, notifying UI."));

            QByteArray* data = new QByteArray();

            ipc_header envelopeHeader;
            envelopeHeader.val.command = ProcedureStopped;
            envelopeHeader.val.size = sizeof(ipc_integer);
            data->push_back(QByteArray::fromRawData(envelopeHeader.data, sizeof(ipc_header)));

            ipc_integer envelopeId;
            envelopeId.number = id;
            data->push_back(QByteArray::fromRawData(envelopeId.data, sizeof(ipc_integer)));

            LOG->debug(
                    SSTR(
                            "SEND: cmd=" << envelopeHeader.val.command << " size=" << envelopeHeader.val.size));
            write(data);
            delete data;
        }
    }

    void HeadlessListener::write(QByteArray* data)
    {
        LOG->debug(SSTR("WRITE bytes: " << data->size()));
        if (!socket || !socket->isOpen()) {
            LOG->error(SSTR("Failed writing data to socket, not connected"));
            return;
        }
        if (socket->isOpen()) {
            if (socket->write(*data) == -1) {
                LOG->error(SSTR("Failed writing data to socket, dropping..."));
                reconnect();
            }
            socket->flush();
            socket->waitForBytesWritten();
        } else {
            LOG->error(SSTR("Write called on closed socket, dropping..."));
        }
    }

} /* namespace actiontrigger */
