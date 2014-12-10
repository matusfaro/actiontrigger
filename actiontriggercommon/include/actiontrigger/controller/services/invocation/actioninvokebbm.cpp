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

#include "actiontrigger/controller/services/invocation/actioninvokebbm.hpp"

namespace actiontrigger
{

    std::string ActionInvokeBbm::TYPE = "ACTION_ActionInvokeBbm";

    std::vector<ParameterDefinition*>* ActionInvokeBbm::parameterDefinitions = NULL;
    Logger* ActionInvokeBbm::StatementLOG = LoggerFactory::getInstance("ActionInvokeBbm");
    const StatementInfo ActionInvokeBbm::info(false, "BBM invocation", ActionInvokeBbm::TYPE,
            RichText::html(
                    "Invokes BBM application for various actions." + RichText::newline()
                            + RichText::color("red",
                                    "Warning: Due to Blackberry restrictions, invocation will not work if app is minimized.")));

    const StatementInfo ActionInvokeBbm::getInfo()
    {
        return ActionInvokeBbm::info;
    }

    ActionInvokeBbm::ActionInvokeBbm(StatementModel* model) :
            Action(model)
    {
    }

    ActionInvokeBbm::~ActionInvokeBbm()
    {
    }

    std::vector<ParameterDefinition*>* ActionInvokeBbm::getParameterDefinitions()
    {
        if (parameterDefinitions == NULL) {
            parameterDefinitions = new std::vector<ParameterDefinition*>();

            // First let's display the filter dropdown
            ParameterDefinition* filter = ParameterDefinition::getDropdownFilter("ACTION", "Action",
                    "BBM action to invoke", ".");
            parameterDefinitions->push_back(filter);

            // Insert all the parameters
            parameterDefinitions->push_back(
                    ParameterDefinition::getFileDialog("FILE", "File", "File to share",
                            bb::cascades::pickers::FilePickerMode::Picker,
                            bb::cascades::pickers::FileType::Other));
            int INDEX_FILE = parameterDefinitions->size() - 1;
            parameterDefinitions->push_back(
                    ParameterDefinition::getString("TEXT", "Text", "Text to share"));
            int INDEX_TEXT = parameterDefinitions->size() - 1;
            parameterDefinitions->push_back(
                    ParameterDefinition::getFileDialog("PICTURE_FILE",
                            "Picture to use as display picture", "",
                            bb::cascades::pickers::FilePickerMode::Picker,
                            bb::cascades::pickers::FileType::Picture));
            int INDEX_PICTURE_FILE = parameterDefinitions->size() - 1;
            parameterDefinitions->push_back(ParameterDefinition::getString("PIN", "PIN", ""));
            int INDEX_PIN = parameterDefinitions->size() - 1;

            // Associate filters with parameters
            filter->createFilter("Share text", "SHARE_TEXT_TO_PIN");
            filter->addToFilter(INDEX_TEXT);
            filter->createFilter("Share file", "SHARE_FILE_TO_PIN");
            filter->addToFilter(INDEX_FILE);
            filter->createFilter("Share text to group", "SHARE_TEXT_TO_GROUP");
            filter->addToFilter(INDEX_TEXT);
            filter->createFilter("Share file to group", "SHARE_FILE_TO_GROUP");
            filter->addToFilter(INDEX_FILE);
            filter->createFilter("Share text to channel", "SHARE_TEXT_TO_CHANNEL");
            filter->addToFilter(INDEX_TEXT);
            filter->createFilter("Share file to channel", "SHARE_FILE_TO_CHANNEL");
            filter->addToFilter(INDEX_FILE);
            filter->createFilter("Open BBM", "OPEN_BBM");
            filter->createFilter("Set display picture", "SET_DISPLAY_PIC");
            filter->addToFilter(INDEX_PICTURE_FILE);
            filter->createFilter("Open chat", "OPEN_OR_INVITE");
            filter->addToFilter(INDEX_PIN);
            filter->createFilter("Audio call", "AUDIO_CALL");
            filter->addToFilter(INDEX_PIN);
            filter->createFilter("Video call", "VIDEO_CALL");
            filter->addToFilter(INDEX_PIN);
            filter->createFilter("Open channel", "OPEN_CHANNEL");
            filter->addToFilter(INDEX_PIN);
        }
        return parameterDefinitions;
    }

    std::string ActionInvokeBbm::getDefaultParameter(std::string key)
    {
        if (key == "ACTION") {
            return "OPEN_BBM";
        } else {
            return Statement::getDefaultParameter(key);
        }
    }

    std::string ActionInvokeBbm::getType() const
    {
        return ActionInvokeBbm::TYPE;
    }

    std::vector<statement_output_variable_definition*>* ActionInvokeBbm::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        vars->push_back(
                new statement_output_variable_definition("INVOCATION_REPLY_DATA",
                        "Data received from invocation.", "STRING"));
        vars->push_back(
                new statement_output_variable_definition("INVOCATION_REPLY_DATA_TYPE",
                        "Type of data received from invocation.", "STRING"));
        return vars;
    }

    void ActionInvokeBbm::execute(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();

        // Retrieve desired action
        std::string action = getParameter("ACTION", state);

        // Determine parameters
        std::string targetId = "";
        std::string invokeAction = "";
        std::string mimeType = "";
        std::string uri = "";
        std::string data = "";
        if (action == "") {
            RUNLOG->error("Select an action to perform");
            return;
        } else if (action == "SHARE_TEXT_TO_PIN") {
            targetId = "sys.bbm.sharehandler";
            invokeAction = "bb.action.SHARE";
            mimeType = "text/plain";
            data = getParameter("TEXT", state);
        } else if (action == "SHARE_FILE_TO_PIN") {
            targetId = "sys.bbm.sharehandler";
            invokeAction = "bb.action.SHARE";
            uri = getParameter("FILE", state);
        } else if (action == "SHARE_TEXT_TO_GROUP") {
            targetId = "sys.bbgroups.sharehandler";
            invokeAction = "bb.action.SHARE";
            mimeType = "text/plain";
            data = getParameter("TEXT", state);
        } else if (action == "SHARE_FILE_TO_GROUP") {
            targetId = "sys.bbgroups.sharehandler";
            invokeAction = "bb.action.SHARE";
            uri = getParameter("FILE", state);
        } else if (action == "SHARE_TEXT_TO_CHANNEL") {
            targetId = "sys.channels.sharehandler";
            invokeAction = "bb.action.SHARE";
            mimeType = "text/plain";
            data = getParameter("TEXT", state);
        } else if (action == "SHARE_FILE_TO_CHANNEL") {
            targetId = "sys.channels.sharehandler";
            invokeAction = "bb.action.SHARE";
            uri = getParameter("FILE", state);
        } else if (action == "OPEN_BBM") {
            targetId = "sys.bbm";
            invokeAction = "bb.action.OPEN";
        } else if (action == "SET_DISPLAY_PIC") {
            targetId = "sys.bbm.imagehandler";
            invokeAction = "bb.action.SET";
            uri = getParameter("PICTURE_FILE", state);
        } else if (action == "OPEN_OR_INVITE") {
            targetId = "sys.bbm.sharehandler";
            invokeAction = "bb.action.BBMCHAT";
            uri = SSTR("pin:" << getParameter("PIN", state));
        } else if (action == "AUDIO_CALL") {
            targetId = "sys.service.videochat";
            invokeAction = "bb.action.OPEN";
            data = SSTR("dest=" << getParameter("PIN", state) << "&video=0");
        } else if (action == "VIDEO_CALL") {
            targetId = "sys.service.videochat";
            invokeAction = "bb.action.OPEN";
            data = SSTR("dest=" << getParameter("PIN", state) << "&video=1");
        } else if (action == "OPEN_CHANNEL") {
            targetId = "sys.bbm.channels.card.previewer";
            invokeAction = "bb.action.OPENBBMCHANNEL";
            uri = SSTR("bbmc:" << getParameter("PIN", state));
        } else {
            LOG->error(SSTR("Error invoking, unknown desired action: " << action));
        }

        Invocation* invocation = new Invocation();
        bb::system::InvokeTargetReply* reply = invocation->invoke(targetId, invokeAction, mimeType,
                uri, data);
        if (!reply->isFinished()) {
            RUNLOG->error("Timed out invoking target");
            return;
        } else if (reply->error() != bb::system::InvokeReplyError::None) {
            RUNLOG->error(
                    "Target invoke failed: " + Invocation::errorMessageToString(reply->error()));
            return;
        }
    }

} /* namespace actiontrigger */
