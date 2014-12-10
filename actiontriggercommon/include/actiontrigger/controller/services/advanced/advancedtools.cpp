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

#include "actiontrigger/controller/services/advanced/advancedtools.hpp"

namespace actiontrigger
{

    unsigned long AdvancedTools::getBootTime()
    {
        return SYSPAGE_ENTRY(qtime)->boot_time;
    }

    std::string AdvancedTools::invoke(std::string cmd)
    {
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe)
            return "Could not invoke shell command";
        char buffer[128];
        std::string output = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                output += buffer;
        }
        pclose(pipe);
        return output;
    }

    void AdvancedTools::execute(QString interpreter, QStringList args, std::string code,
            ExecutionState* state, int msecTimeout, bool reverseOutAndErr)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        QProcess proc;
        proc.start(interpreter, args);

        if (!proc.waitForStarted(5000)) {

            RUNLOG->error(
                    "Could not start interpreter process: " + proc.errorString().toStdString());
            return;
        }

        proc.write(code.c_str());
        proc.closeWriteChannel();

        if (!proc.waitForFinished(msecTimeout)) {
            proc.kill();
            RUNLOG->error("Timed out waiting for execution to finish");
            return;
        }

        QString out;
        QString err;
        if (reverseOutAndErr) {
            out = QString(proc.readAllStandardError());
            err = QString(proc.readAllStandardOutput());
        } else {
            err = QString(proc.readAllStandardError());
            out = QString(proc.readAllStandardOutput());
        }

        if (!err.isEmpty()) {
            RUNLOG->raw(err.toStdString());
        }

        state->setUserProperty("OUTPUT", out.toStdString());
    }

} /* namespace actiontrigger */
