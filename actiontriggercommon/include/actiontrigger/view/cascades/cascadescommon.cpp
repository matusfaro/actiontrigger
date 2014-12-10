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

#include "actiontrigger/view/cascades/cascadescommon.hpp"

namespace actiontrigger
{

    QVector<QString>* CascadesCommon::convert(std::vector<std::string>* vector)
    {
        QVector<QString>* qvector = new QVector<QString>();
        for (std::vector<std::string>::iterator it = vector->begin(); it != vector->end(); ++it) {
            QString* string = new QString(QString::fromStdString((*it).c_str()));
            qvector->push_back(*string);
        }
        return qvector;
    }

} /* namespace actiontrigger */
