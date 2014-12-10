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

#include "actiontrigger/model/database/sqlite/settingsdbmodel.hpp"

namespace actiontrigger
{

    Logger* SettingsDBModel::LOG = LoggerFactory::getInstance("SettingsDBModel");

    SettingsDBModel::SettingsDBModel()
    {
        LOG->trace("SettingsDBModel");
        QSqlQuery query;
        query.prepare("SELECT NAME, VALUE FROM SETTINGS");
        SqliteTools::queryExec(&query);
        this->settings = new std::map<std::string, std::string>();
        while (query.next()) {
            LOG->trace(
                    "Loading.. Setting: name=\""
                            + std::string(query.value(0).toString().toUtf8().constData()) + "\"");
//        LOG->loading(" Setting: " + std::string(query.value(0).toString().toUtf8().constData()));
            this->settings->insert(
                    std::pair<std::string, std::string>(
                            query.value(0).toString().toUtf8().constData(), // Name
                            query.value(1).toString().toUtf8().constData())); // Value
        }
    }

    SettingsDBModel::~SettingsDBModel()
    {
    }

    std::string SettingsDBModel::getSetting(std::string name)
    {
        std::string value;
        if (this->settings->find(name) != this->settings->end()) {
            value = this->settings->find(name)->second;
            LOG->trace("Read property \"" + name + "\" with value \"" + value + "\"");
        } else {
            value = getDefaultSetting(name);
            LOG->trace("Read default property \"" + name + "\" with value \"" + value + "\"");
        }
        return value;
    }

    std::string SettingsDBModel::getDefaultSetting(std::string name)
    {
        (void) name;
        return "";
    }

    void SettingsDBModel::setSetting(std::string name, std::string value)
    {
        LOG->trace("Setting property \"" + name + "\" with value \"" + value + "\"");
        QSqlQuery query;
        if (this->settings->find(name) != this->settings->end()) {
            this->settings->find(name)->second = value;
            query.prepare("UPDATE SETTINGS SET VALUE = :value WHERE name = :name");
        } else {
            this->settings->insert(std::pair<std::string, std::string>(name, value));
            query.prepare("INSERT INTO SETTINGS (NAME, VALUE) VALUES (:name, :value)");
        }
        query.bindValue(":name", QString::fromUtf8(name.c_str()));
        query.bindValue(":value", QString::fromUtf8(value.c_str()));
        SqliteTools::queryExec(&query);
    }

} /* namespace actiontrigger */
