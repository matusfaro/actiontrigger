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

#ifndef TriggerMagnetometer_HPP_
#define TriggerMagnetometer_HPP_

#include <errno.h>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/services/triggervalue.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include "actiontrigger/controller/services/sensors/sensorhandler.hpp"

namespace actiontrigger
{

    class TriggerMagnetometer: public TriggerValue
    {
    public:
        TriggerMagnetometer(StatementModel* model);
        ~TriggerMagnetometer();
        static bool isSupported();
        static std::string TYPE;
        std::string getType() const;
        static const StatementInfo info;
        const StatementInfo getInfo();
    protected:
        bool isEventOnlyTrigger();
        // Pair contains variable name and type
        std::vector<trigger_output_variable_definition*>* getValueDefinitions();
        // Retrieve all values in same order as value definitions
        bool isActive(ExecutionState* state);
        void blockUntilActive(ExecutionState* state);
    private:
        static Logger* StatementLOG;
    };

} /* namespace actiontrigger */
#endif /* TriggerMagnetometer_HPP_ */
