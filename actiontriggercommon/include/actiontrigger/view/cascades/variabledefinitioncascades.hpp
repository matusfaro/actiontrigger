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

#ifndef VARIABLEDEFINITIONCASCADES_HPP_
#define VARIABLEDEFINITIONCASCADES_HPP_

#include <QObject>
#include <QString>
#include <string>
#include "actiontrigger/controller/services/statement.hpp"

namespace actiontrigger
{

    class VariableDefinitionCascades: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString statementName READ getStatementName FINAL)
    Q_PROPERTY (QString name READ getName FINAL)
    Q_PROPERTY (QString description READ getDescription FINAL)
    Q_PROPERTY (QString type READ getType FINAL)
    public:
        VariableDefinitionCascades(std::string statementName,
                statement_output_variable_definition* model);
        virtual ~VariableDefinitionCascades();
        QString getStatementName() const;
        QString getName() const;
        QString getDescription() const;
        QString getType() const;
    private:
        std::string statementName;
        statement_output_variable_definition* model;
    };

} /* namespace actiontrigger */
#endif /* VARIABLEDEFINITIONCASCADES_HPP_ */
