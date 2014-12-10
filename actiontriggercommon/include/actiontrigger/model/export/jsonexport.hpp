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

#ifndef JSONEXPORT_HPP_
#define JSONEXPORT_HPP_

#include <QString>
#include <QVariant>
#include <QVariantList>
#include <bb/data/JsonDataAccess>
#include <bb/data/DataAccessError>

#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/controller/procedure.hpp"

namespace actiontrigger
{

    class JsonExport
    {
    public:
        JsonExport();
        virtual ~JsonExport();

        // Importing
        bool loadFromFile(QString filename); // Returns status string
        bool loadFromString(QString rawData); // Returns status string
        QString getLastErrMsg();
        std::vector<Procedure*>* import();

        // Exporting
        void add(Procedure* procedure);
        bool exportToFile(QString filename);
        QString exportToString();
    private:
        static Logger* LOG;
        bb::data::JsonDataAccess jda;
        QVariantList data;
        QString lastErrMsg;
    }
    ;

} /* namespace actiontrigger */
#endif /* JSONEXPORT_HPP_ */
