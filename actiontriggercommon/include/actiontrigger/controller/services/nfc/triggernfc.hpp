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

#ifndef TriggerNfc_HPP_
#define TriggerNfc_HPP_

#include <errno.h>
#include "actiontrigger/controller/common.hpp"
#include "actiontrigger/controller/services/triggervalue.hpp"
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include <nfc/nfc.h>
#include <nfc/nfc_bps.h>
#include <nfc/nfc_ndef.h>

namespace actiontrigger
{

    class TriggerNfc: public TriggerValue
    {
    public:
        TriggerNfc(StatementModel* model);
        ~TriggerNfc();
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
    private:
        std::string decodeMessage(nfc_ndef_message_t *nextMessage);
        static Logger* StatementLOG;
    };

} /* namespace actiontrigger */
#endif /* TriggerNfc_HPP_ */
