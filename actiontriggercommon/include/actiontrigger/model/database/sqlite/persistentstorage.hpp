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

#ifndef PersistentStorage_HPP_
#define PersistentStorage_HPP_

#include <string>
#include <map>
#include "actiontrigger/model/database/settingsmodel.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include <QSqlQuery>
#include "actiontrigger/model/database/sqlite/sqlitetools.hpp"
#include <QMutex>

#define INTERNAL_STORAGE_PREPEND "I_"
#define USER_STORAGE_PREPEND "U_"

namespace actiontrigger
{

    class PersistentStorage
    {
    public:
        static void load();
        static PersistentStorage* get();

        // Used by actions and triggers to store user hidden key-values
        std::string getInternalValue(std::string name);
        void setInternalValue(std::string name, std::string value);

        // Used by users to store persistent key-values
        std::string getUserValue(std::string name, std::string defaultValue = "");
        void setUserValue(std::string name, std::string value);

    private:
        static PersistentStorage* instance;
        std::string getValue(std::string name, std::string defaultValue = "");
        void setValue(std::string name, std::string value);
        PersistentStorage();
        virtual ~PersistentStorage();
        static QMutex mutexData;
        std::map<std::string, std::string>* cache;
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* PersistentStorage_HPP_ */
