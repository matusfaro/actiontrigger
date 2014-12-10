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

#include "actiontrigger/controller/services/system/actionmemory.hpp"

namespace actiontrigger
{

    std::string ActionMemory::TYPE = "ACTION_ActionMemory";

    std::vector<ParameterDefinition*>* ActionMemory::parameterDefinitions = NULL;
    Logger* ActionMemory::StatementLOG = LoggerFactory::getInstance("ActionMemory");
    const StatementInfo ActionMemory::info(false, "System Memory", ActionMemory::TYPE,
            "View total system memory and current usage");

    const StatementInfo ActionMemory::getInfo()
    {
        return ActionMemory::info;
    }

    ActionMemory::ActionMemory(StatementModel* model) :
            Action(model)
    {
    }

    ActionMemory::~ActionMemory()
    {
    }

    std::string ActionMemory::getType() const
    {
        return ActionMemory::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionMemory::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("MEMORY_TOTAL",
                        "Total available device memory in bytes", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MEMORY_AVAIL",
                        "Available device memory in bytes", "NUMBER"));
//    vars->push_back(new VariableDefinition("MEMORY_APP_USED", "Used device memory by this application in bytes", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MEMORY_TOTAL_GB",
                        "Total available device memory in gigabytes", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MEMORY_TOTAL_MB",
                        "Total available device memory in megabytes", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MEMORY_AVAIL_MB",
                        "Available device memory in megabytes", "NUMBER"));
//    vars->push_back(new VariableDefinition("MEMORY_APP_USED_MB", "Used device memory by this application in megabytes", "NUMBER"));
        vars->push_back(
                new statement_output_variable_definition("MEMORY_AVAIL_PERCENT",
                        "Available device memory as a percentage of total device memory",
                        "NUMBER"));
        return vars;
    }

    void ActionMemory::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        bb::MemoryInfo memory;
        qint64 totalMem = memory.totalDeviceMemory();
        qint64 availMem = memory.availableDeviceMemory();
        /**
         * Memory used by current process gives enormous values.
         * Most likely this corresponds to the total of all user
         * apps, not just the current one. Take it out for now.
         */
//    qint64 appMem = memory.memoryUsedByCurrentProcess();
        qint64 totalMemMb = totalMem / 1024 / 1024;
        qint64 totalMemGb = totalMemMb / 1024;
        qint64 availMemMb = availMem / 1024 / 1024;
//    qint64 appMemMb = appMem / 1024 / 1024;
        qint64 availMemPercent = (100 * availMem) / totalMem;
        RUNLOG->debug(
                SSTR(
                        "BytesTotal=" << totalMem << " BytesAvail=" << availMem
//            << " BytesApp=" << appMem
                                << " GBytesTotal=" << totalMemGb << " MBytesTotal=" << totalMemMb
                                << " MBytesAvail=" << availMemMb
//            << " MBytesApp=" << appMemMb
                                << " AvailPercent=" << availMemPercent));
        state->setUserProperty("MEMORY_TOTAL_B", SSTR(totalMem));
        state->setUserProperty("MEMORY_AVAIL_B", SSTR(availMem));
//    state->setUserProperty("MEMORY_APP_USED_B", SSTR(appMem));
        state->setUserProperty("MEMORY_TOTAL_GB", SSTR(totalMemGb));
        state->setUserProperty("MEMORY_TOTAL_MB", SSTR(totalMemMb));
        state->setUserProperty("MEMORY_AVAIL_MB", SSTR(availMemMb));
//    state->setUserProperty("MEMORY_APP_USED_MB", SSTR(appMemMb));
        state->setUserProperty("MEMORY_AVAIL_PERCENT", SSTR(availMemPercent));
    }

} /* namespace actiontrigger */
