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

#include <actiontrigger/controller/headlessconnector.hpp>
#include "actiontrigger/controller/services/invocation/invocation.hpp"
#include "actiontrigger/controller/ipcproto.hpp"

namespace actiontrigger
{
    Logger* HeadlessConnector::LOG = LoggerFactory::getInstance("HeadlessConnector");
    ////// HEADLESS DISABLED
    const bool HeadlessConnector::HEADLESS_ENABLED = false;
    HeadlessConnector* HeadlessConnector::instance = NULL;

    HeadlessConnector* HeadlessConnector::get()
    {
        if (!HeadlessConnector::instance) {
            HeadlessConnector::instance = new HeadlessConnector();
        }
        return HeadlessConnector::instance;
    }

    HeadlessConnector::HeadlessConnector() :
            QObject(), verified(false), socket(NULL)
    {
        invoke();
        reconnectWatchdog.setInterval(1000);
        reconnectWatchdog.setSingleShot(false);
        QObject::connect(&reconnectWatchdog, SIGNAL(timeout()), this, SLOT(reconnect()));
    }

    HeadlessConnector::~HeadlessConnector()
    {
    }

    bool HeadlessConnector::getIsConnected() const
    {
        return socket && socket->isOpen() && verified;
    }

    void HeadlessConnector::start()
    {
        LOG->debug("Start called");
        if (!getIsConnected()) {
            reconnect();
        }
    }

    void HeadlessConnector::kill()
    {
        while (!outQueue.empty()) {
            outQueue.pop();
        }
        reconnect();
        if (socket) {
            socket->waitForConnected(2000);
        }
        if (socket->isOpen()) {
            ipc_header envelopeHeader;
            envelopeHeader.val.command = Shutdown;
            envelopeHeader.val.size = 0;
            QByteArray* data = new QByteArray(envelopeHeader.data, sizeof(envelopeHeader.data));

            LOG->debug(
                    SSTR(
                            "SEND: cmd=" << envelopeHeader.val.command << " size=" << envelopeHeader.val.size));
            write(data);
            delete data;

            verified = false;
        }
        socketDisconnected();
    }

    void HeadlessConnector::procedureStart(int id)
    {
        QByteArray* data = new QByteArray();

        ipc_header envelopeHeader;
        envelopeHeader.val.command = ProcedureStart;
        envelopeHeader.val.size = sizeof(ipc_integer);
        data->push_back(QByteArray::fromRawData(envelopeHeader.data, sizeof(ipc_header)));

        ipc_integer envelopeId;
        envelopeId.number = id;
        data->push_back(QByteArray::fromRawData(envelopeId.data, sizeof(ipc_integer)));

        LOG->debug(
                SSTR(
                        "SEND: cmd=" << envelopeHeader.val.command << " size=" << envelopeHeader.val.size));
        if (getIsConnected()) {
            write(data);
            delete data;
        } else {
            outQueue.push(data);
            reconnect();
        }
    }

    void HeadlessConnector::procedureStop(int id)
    {
        QByteArray* data = new QByteArray();

        ipc_header envelopeHeader;
        envelopeHeader.val.command = ProcedureStop;
        envelopeHeader.val.size = sizeof(ipc_integer);
        data->push_back(QByteArray::fromRawData(envelopeHeader.data, sizeof(ipc_header)));

        ipc_integer envelopeId;
        envelopeId.number = id;
        data->push_back(QByteArray::fromRawData(envelopeId.data, sizeof(ipc_integer)));

        LOG->debug(
                SSTR(
                        "SEND: cmd=" << envelopeHeader.val.command << " size=" << envelopeHeader.val.size));
        if (getIsConnected()) {
            write(data);
            delete data;
        } else {
            outQueue.push(data);
            reconnect();
        }
    }

    void HeadlessConnector::connect()
    {
        if (!socket) {
            socket = new QTcpSocket();
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
                            "Connecting to " << QHostAddress::LocalHost << ":" << HEADLESS_SERVER_PORT));
            socket->connectToHost(QHostAddress::LocalHost, HEADLESS_SERVER_PORT);
        }
    }

    void HeadlessConnector::reconnect()
    {
        if (!socket) {
            connect();
        } else {
            LOG->debug(SSTR("reconnect from state: " << socket->state()));
            socket->disconnectFromHost();
            // Should this be here? Will this be called as part of disconnectFromHost?
            socketDisconnected();
        }
    }

    void HeadlessConnector::socketError(QAbstractSocket::SocketError socketError)
    {
        if (!socket) {
            LOG->warning("Socket error on null socket");
            return;
        }
        LOG->debug(SSTR("Socket error: " << socketError));
        switch (socketError) {
            default:
            case QTcpSocket::HostNotFoundError:
            case QTcpSocket::ProxyAuthenticationRequiredError:
            case QTcpSocket::SslHandshakeFailedError:
            case QTcpSocket::ProxyConnectionRefusedError:
            case QTcpSocket::ProxyConnectionClosedError:
            case QTcpSocket::ProxyConnectionTimeoutError:
            case QTcpSocket::ProxyNotFoundError:
            case QTcpSocket::ProxyProtocolError:
                LOG->warning("???");
            case QTcpSocket::SocketResourceError:
            case QTcpSocket::DatagramTooLargeError:
                // Drop next packet in case this is the problem
                if (!outQueue.empty()) {
                    outQueue.pop();
                }
            case QTcpSocket::UnknownSocketError:
            case QTcpSocket::SocketAccessError:
            case QTcpSocket::SocketAddressNotAvailableError:
            case QTcpSocket::UnsupportedSocketOperationError:
            case QTcpSocket::UnfinishedSocketOperationError:
            case QTcpSocket::AddressInUseError:
            case QTcpSocket::RemoteHostClosedError:
            case QTcpSocket::ConnectionRefusedError:
            case QTcpSocket::SocketTimeoutError:
            case QTcpSocket::NetworkError:
                invoke();
                socketDisconnected();
                break;
        }
    }

    void HeadlessConnector::socketConnected()
    {
        if (!socket) {
            LOG->warning("socketConnected on null socket");
            return;
        }
        LOG->debug(SSTR("socketConnected"));
        reconnectWatchdog.stop();
        if (!verified) {
            ipc_header envelopeHeader;
            envelopeHeader.val.command = Initialize;
            envelopeHeader.val.size = 0;
            QByteArray* data = new QByteArray(envelopeHeader.data, sizeof(envelopeHeader.data));

            LOG->debug(
                    SSTR(
                            "SEND: cmd=" << envelopeHeader.val.command << " size=" << envelopeHeader.val.size));
            write(data);
            delete data;
        }
    }

    void HeadlessConnector::socketDisconnected()
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
        emit notifyChangeIsConnected();
        if (!reconnectWatchdog.isActive())
            reconnectWatchdog.start();
    }

    void HeadlessConnector::socketRead()
    {
        if (!socket) {
            LOG->warning("socketRead on null socket");
            return;
        }
        LOG->debug(SSTR("socketRead bytes: " << socket->bytesAvailable()));
        if (socket->bytesAvailable() == 0) {
            return;
        }
        if (socket->bytesAvailable() >= sizeof(ipc_header)) {
            ipc_header msg;
            socket->read(msg.data, sizeof(ipc_header));

            LOG->debug(SSTR("RECV: COMMAND: " << msg.val.command));
            do {
                LOG->debug(SSTR("RECV: BYTES LEFT: " << msg.val.size));
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
                        emit notifyChangeIsConnected();

                        // Flush out queue
                        while (!outQueue.empty()) {
                            write(outQueue.front());
                            outQueue.pop();
                        }
                        return;

                    case ShuttingDown:
                        if (msg.val.size != 0) {
                            LOG->error("RECV: Size is set for ShuttingDown command");
                            reconnect();
                            return;
                        }
                        LOG->error("Received headless is ShuttingDown");
                        reconnect();
                        return;

                    case ProcedureStopped:
                        if (msg.val.size < sizeof(int)) {
                            LOG->error(
                                    SSTR(
                                            "RECV: Size is less than size of int for ProcedureStopped command: " << msg.val.size));
                            reconnect();
                            return;
                        } else {
                            // Handle data
                            ipc_integer envelopeId;
                            socket->read(envelopeId.data, sizeof(envelopeId));
                            LOG->debug(SSTR("RECV: ProcedureStopped: " << envelopeId.number));
                            emit procedureStopped(envelopeId.number);
                            msg.val.size -= sizeof(ipc_integer);
                        }
                        break;

                    case ProcedureRunningList:
                        if (msg.val.size < sizeof(int)) {
                            LOG->error(
                                    SSTR(
                                            "RECV: Size is less than size of int for ProcedureRunningList command: " << msg.val.size));
                            reconnect();
                            return;
                        } else {
                            // Handle data
                            ipc_integer envelopeId;
                            socket->read(envelopeId.data, sizeof(envelopeId));
                            LOG->debug(SSTR("RECV: ProcedureRunning: " << envelopeId.number));
                            emit procedureRunning(envelopeId.number);
                            msg.val.size -= sizeof(ipc_integer);
                        }
                        break;

                    case Shutdown:
                    case ProcedureStart:
                    case ProcedureStop:
                        LOG->debug(SSTR("Client only command: " << msg.val.command));
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

    void HeadlessConnector::invoke()
    {
        LOG->debug("Invoking headless");
        Invocation invocation;
        if (!invocation.invoke("com.actiontriggerService", "bb.action.START", "actiontrigger/START",
                "", (std::string) "", 30000)) {
            LOG->error("Failed to invoke headless");
        }
    }

    void HeadlessConnector::write(QByteArray* data)
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

} /* namespace actiontriggerservice */
