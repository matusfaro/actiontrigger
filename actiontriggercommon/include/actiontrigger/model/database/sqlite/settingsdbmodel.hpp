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

#ifndef SETTINGSDBMODEL_HPP_
#define SETTINGSDBMODEL_HPP_

#include <string>
#include <map>
#include "actiontrigger/model/database/settingsmodel.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include <QSqlQuery>
#include "actiontrigger/model/database/sqlite/sqlitetools.hpp"

namespace actiontrigger
{

    class SettingsDBModel: public SettingsModel
    {
    public:
        SettingsDBModel();
        virtual ~SettingsDBModel();

        std::string getSetting(std::string name);
        void setSetting(std::string name, std::string value);
    private:
        std::map<std::string, std::string>* settings;
        static Logger* LOG;
        std::string getDefaultSetting(std::string name);
    };

} /* namespace actiontrigger */
#endif /* SETTINGSDBMODEL_HPP_ */
