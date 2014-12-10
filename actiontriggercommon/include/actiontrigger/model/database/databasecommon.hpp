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

#ifndef DATABASECOMMON_HPP_
#define DATABASECOMMON_HPP_

namespace actiontrigger
{

    /*
     * READ_CACHE - Reads never hit database. Writes are flushed right away.
     * IN_MEMORY - No interaction with the database, data is kept in memory only.
     */
    enum DATABASE_MODE
    {
        READ_CACHE, IN_MEMORY
    };

} /* namespace actiontrigger */
#endif /* DATABASECOMMON_HPP_ */
