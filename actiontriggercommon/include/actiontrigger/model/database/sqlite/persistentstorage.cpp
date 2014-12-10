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

#include "actiontrigger/model/database/sqlite/persistentstorage.hpp"

namespace actiontrigger
{

    Logger* PersistentStorage::LOG = LoggerFactory::getInstance("PersistentStorage");
    QMutex PersistentStorage::mutexData;
    PersistentStorage* PersistentStorage::instance = NULL;

    void PersistentStorage::load()
    {
        if (!instance) {
            instance = new PersistentStorage();
        }
    }

    PersistentStorage* PersistentStorage::get()
    {
        if (!instance) {
            load();
        }
        return instance;
    }

    PersistentStorage::PersistentStorage()
    {
        LOG->trace("PersistentStorage");
        QSqlQuery query;
        query.prepare("SELECT NAME, VALUE FROM VARIABLES");
        SqliteTools::queryExec(&query);
        this->cache = new std::map<std::string, std::string>();
        while (query.next()) {
            this->cache->insert(
                    std::pair<std::string, std::string>(
                            query.value(0).toString().toUtf8().constData(), // Name
                            query.value(1).toString().toUtf8().constData())); // Value
        }
    }

    PersistentStorage::~PersistentStorage()
    {
    }

    std::string PersistentStorage::getInternalValue(std::string name)
    {
        return this->getValue(INTERNAL_STORAGE_PREPEND + name);
    }

    void PersistentStorage::setInternalValue(std::string name, std::string value)
    {
        this->setValue(INTERNAL_STORAGE_PREPEND + name, value);
    }

    std::string PersistentStorage::getUserValue(std::string name, std::string defaultValue)
    {
        return this->getValue(USER_STORAGE_PREPEND + name, defaultValue);
    }

    void PersistentStorage::setUserValue(std::string name, std::string value)
    {
        this->setValue(USER_STORAGE_PREPEND + name, value);
    }

    std::string PersistentStorage::getValue(std::string name, std::string defaultValue)
    {
        std::string value;
        mutexData.lock();
        if (this->cache->find(name) != this->cache->end()) {
            value = this->cache->find(name)->second;
            LOG->trace("Read property \"" + name + "\" with value \"" + value + "\"");
        } else {
            value = defaultValue;
        }
        mutexData.unlock();
        return value;
    }

    void PersistentStorage::setValue(std::string name, std::string value)
    {
        LOG->trace("Setting property \"" + name + "\" with value \"" + value + "\"");
        QSqlQuery query;
        mutexData.lock();
        if (this->cache->find(name) != this->cache->end()) {
            this->cache->find(name)->second = value;
            query.prepare("UPDATE VARIABLES SET VALUE = :value WHERE name = :name");
        } else {
            this->cache->insert(std::pair<std::string, std::string>(name, value));
            query.prepare("INSERT INTO VARIABLES (NAME, VALUE) VALUES (:name, :value)");
        }
        query.bindValue(":name", QString::fromUtf8(name.c_str()));
        query.bindValue(":value", QString::fromUtf8(value.c_str()));
        SqliteTools::queryExec(&query);
        mutexData.unlock();
    }

} /* namespace actiontrigger */
