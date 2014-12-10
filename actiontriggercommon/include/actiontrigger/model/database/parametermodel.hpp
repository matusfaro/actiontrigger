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

#ifndef PARAMETERMODEL_HPP_
#define PARAMETERMODEL_HPP_

#include <string>
#include <qt4/QtSql/QSqlQuery>
#include <qt4/QtCore/QVariant>

namespace actiontrigger
{

    class ParameterModel
    {
    public:
        /*
         * Destructor
         */
        virtual ~ParameterModel()
        {
        }
        ;
        /*
         * Delete permanently
         */
        virtual void remove() = 0;
        /*
         * Getters and setters
         */
        virtual int getId() const = 0;
        virtual std::string getValue() const = 0;
        virtual void setValue(std::string value) = 0;
        virtual std::string getName() const = 0;
        virtual void setName(std::string name) = 0;
    };

} /* namespace actiontrigger */
#endif /* PARAMETERMODEL_HPP_ */
