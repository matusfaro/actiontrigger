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

#ifndef ADVANCEDTOOLS_HPP_
#define ADVANCEDTOOLS_HPP_

#include "errno.h"
#include <string>
#include <sys/syspage.h>
#include <stdio.h>
#include <iostream>
#include <QProcess>
#include "actiontrigger/controller/executionstate.hpp"

namespace actiontrigger
{

    class AdvancedTools
    {
    public:
        // UTC seconds when machine booted
        static unsigned long getBootTime();
        // Invoke shell command and returns STDOUT as string
        static std::string invoke(std::string cmd);
        // Executes interpreter with code. Sets OUTPUT var
        static void execute(QString interpreter, QStringList args, std::string code,
                ExecutionState* state, int msecTimeout, bool reverseOutAndErr = false);
    private:
        AdvancedTools()
        {
        }
    };

} /* namespace actiontrigger */
#endif /* ADVANCEDTOOLS_HPP_ */
