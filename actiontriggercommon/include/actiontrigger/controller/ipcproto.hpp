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

#ifndef IPCPROTO_HPP_
#define IPCPROTO_HPP_

#define HEADLESS_SERVER_PORT 34981

namespace actiontrigger
{
    enum IpcCommand
    {
        // Common
        Initialize = 12484,
        // Server only
        ShuttingDown, // Sent when headless is shutting down due to inactivity
        ProcedureStopped,
        ProcedureRunningList,
        // Client only
        Shutdown, // Sent when UI requested immediate shutdown of headless
        ProcedureStart,
        ProcedureStop
    };

    union ipc_header
    {
        struct
        {
            std::size_t size;
            IpcCommand command;
        } val;
        char data[sizeof(int) + sizeof(int)];
    };

    union ipc_integer
    {
        int number;
        char data[sizeof(int)];
    };

} /* namespace actiontrigger */

#endif /* IPCPROTO_HPP_ */
