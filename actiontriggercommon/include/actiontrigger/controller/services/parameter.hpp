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

#ifndef PARAMETER_HPP_
#define PARAMETER_HPP_

#include <string>
#include "actiontrigger/model/database/parametermodel.hpp"

namespace actiontrigger
{

    class Parameter
    {
    public:
        Parameter(ParameterModel* model);
        virtual ~Parameter();

        std::string getName();
        std::string getValue();
        void setName(std::string);
        void setValue(std::string);
    private:
        ParameterModel* model;
    };

} /* namespace actiontrigger */
#endif /* PARAMETER_HPP_ */
