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

#include "actiontrigger/model/export/jsonexport.hpp"

namespace actiontrigger
{

    Logger* JsonExport::LOG = LoggerFactory::getInstance("JsonExport");

    JsonExport::JsonExport()
    {
        // TODO Auto-generated constructor stub

    }

    JsonExport::~JsonExport()
    {
        // TODO Auto-generated destructor stub
    }

    bool JsonExport::loadFromFile(QString filename)
    {
        this->data = jda.load(filename).toList();
        if (jda.hasError()) {
            std::cerr << "Error parsing JSON data: " << jda.error().errorMessage().toStdString();
            lastErrMsg = "Error importing JSON data: " + jda.error().errorMessage();
            return false;
        }
        return true;
    }

    bool JsonExport::loadFromString(QString rawData)
    {
        this->data = jda.loadFromBuffer(rawData).toList();
        if (jda.hasError()) {
            LOG->warning(SSTR("Error importing json: " << rawData.toStdString()));
            std::cerr << "Error parsing JSON data: " << jda.error().errorMessage().toStdString();
            lastErrMsg = "Error importing JSON data: " + jda.error().errorMessage();
            return false;
        }
        return true;
    }

    QString JsonExport::getLastErrMsg()
    {
        return lastErrMsg;
    }

    std::vector<Procedure*>* JsonExport::import()
    {
        std::vector<Procedure*>* procedures = new std::vector<Procedure*>();
        for (QVariantList::const_iterator iter = data.begin(); iter != data.end(); ++iter) {
            procedures->push_back(new Procedure(*iter));
        }
        return procedures;
    }

    void JsonExport::add(Procedure* procedure)
    {
        data.push_back(procedure->toQVariant());
    }

    bool JsonExport::exportToFile(QString filename)
    {
        jda.save(QVariant(data), filename);
        if (jda.hasError()) {
            std::cerr << "Error exporting JSON data: " << jda.error().errorMessage().toStdString();
            lastErrMsg = "Error exporting JSON data: " + jda.error().errorMessage();
            return false;
        }
        return true;
    }

    QString JsonExport::exportToString()
    {
        QString outString;
        jda.saveToBuffer(QVariant(data), &outString);
        return outString;
    }

} /* namespace actiontrigger */
