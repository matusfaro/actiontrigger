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

#include "actiontrigger/view/cascades/variabledefinitioncascades.hpp"

namespace actiontrigger
{

    VariableDefinitionCascades::VariableDefinitionCascades(std::string statementName,
            statement_output_variable_definition* model) :
            statementName(statementName), model(model)
    {
    }

    VariableDefinitionCascades::~VariableDefinitionCascades()
    {
    }

    QString VariableDefinitionCascades::getStatementName() const
    {
        return QString::fromStdString(this->statementName);
    }

    QString VariableDefinitionCascades::getName() const
    {
        return QString::fromStdString(this->model->name);
    }

    QString VariableDefinitionCascades::getDescription() const
    {
        return QString::fromStdString(this->model->description);
    }

    QString VariableDefinitionCascades::getType() const
    {
        return QString::fromStdString(this->model->type);
    }

} /* namespace actiontrigger */
