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

#ifndef COMMON_HPP_
#define COMMON_HPP_

#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <string>
#include <QUuid>

namespace actiontrigger
{

    struct NameDescription
    {
        std::string name;
        std::string description;
        NameDescription() :
                name(""), description("")
        {
        }
        NameDescription(std::string name, std::string description) :
                name(name), description(description)
        {
        }
    };

    struct NameDescriptionType: public NameDescription
    {
        std::string type;
        NameDescriptionType() :
                NameDescription("", ""), type("")
        {
        }
        NameDescriptionType(std::string name, std::string description, std::string type) :
                NameDescription(name, description), type(type)
        {
        }
    };

    struct KeyNameDescriptionType: public NameDescriptionType
    {
        std::string key;
        KeyNameDescriptionType() :
                NameDescriptionType("", "", ""), key("")
        {
        }
        KeyNameDescriptionType(std::string key, std::string name, std::string description,
                std::string type) :
                NameDescriptionType(name, description, type), key(key)
        {
        }
    };

    struct IdNameDescription
    {
        std::string id;
        NameDescription info;
    };

    struct NameDescriptionValue
    {
        NameDescription info;
        std::string value;
    };

    class Common
    {
    public:
        static bool compare(double a, double b)
        {
            return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
        }

        template<typename T>
        static T StringToNumber(const std::string& stringToConvert)
        {
            if (stringToConvert.empty() || stringToConvert.compare("0") == 0)
                return 0;
            T valor;

            std::stringstream stream(stringToConvert);
            stream >> valor;
            if (stream.fail()) {
                std::runtime_error e(
                        "Failed to convert string as number: '" + stringToConvert + "'");
                throw e;
            }
            return valor;
        }

        // Generate UUID without non-standard curly braces
        static std::string genUuidStr()
        {
            QString quuid = QUuid::createUuid().toString();
            return quuid.mid(1, quuid.size() - 2).toUpper().toStdString();
        }
    };

// TYPES

// Variable definition
    typedef NameDescriptionType statement_output_variable_definition;

    /**
     * Branch definition
     *
     * Key: database key
     * Name, Description: User visible branch info
     * Type: TRIGGER or ACTION (Possibly more in future)
     */
    typedef KeyNameDescriptionType branch_info;

}

#endif /* COMMON_HPP_ */
