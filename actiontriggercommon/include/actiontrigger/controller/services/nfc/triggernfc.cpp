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

#include "actiontrigger/controller/services/nfc/triggernfc.hpp"

namespace actiontrigger
{

    std::string TriggerNfc::TYPE = "TRIGGER_TriggerNfc";
    Logger* TriggerNfc::StatementLOG = LoggerFactory::getInstance("TriggerNfc");
    const StatementInfo TriggerNfc::info(true, "NFC", TriggerNfc::TYPE, "Trigger on NFC tag.");

    const StatementInfo TriggerNfc::getInfo()
    {
        return TriggerNfc::info;
    }

    TriggerNfc::TriggerNfc(StatementModel* model) :
            TriggerValue(model)
    {
    }

    std::string TriggerNfc::getType() const
    {
        return TriggerNfc::TYPE;
    }

    TriggerNfc::~TriggerNfc()
    {
    }

    bool TriggerNfc::isEventOnlyTrigger()
    {
        return false;
    }

    std::vector<trigger_output_variable_definition*>* TriggerNfc::getValueDefinitions()
    {
        std::vector<trigger_output_variable_definition*>* definition = new std::vector<
                trigger_output_variable_definition*>();
        definition->push_back(
                new trigger_output_variable_definition("LOCK_STATE", "Lock state",
                        "Retrieve the device lock state. One of UNLOCKED, SCREEN_LOCKED, and PASSWORD_LOCKED.",
                        "STRING"));
        return definition;
    }

    bool TriggerNfc::isActive(ExecutionState* state)
    {
        DataModelLogger* RUNLOG = state->getLogger();
        std::vector<std::string>* values = new std::vector<std::string>();
        state->getRuntimeResources()->bpsInitialize();
        nfc_request_events();
        nfc_event_t * nfc_event = NULL;
        do {
            state->giveUpIfNotRunning();
            bps_event_t* bps_event = state->getRuntimeResources()->bpsGetEvent(nfc_get_domain(),
                    NFC_SNEP_CONNECTION_EVENT, 10000);
            if (bps_event)
                nfc_get_nfc_event(bps_event, &nfc_event);
        } while (!nfc_event);
        nfc_stop_events();

        RUNLOG->debug(SSTR("Retrieved nfc event"));

        nfc_target_t* nfc_target = 0;
        nfc_get_target(nfc_event, &nfc_target);

        unsigned int nfc_message_count = 0;
        nfc_get_ndef_message_count(nfc_target, &nfc_message_count);

        std::string msg = "";
        for (unsigned int i = 0; i < nfc_message_count; i++) {
            nfc_ndef_message_t* ndef_message;
            nfc_get_ndef_message(nfc_target, i, &ndef_message);
            if (i > 0)
                msg += ",";
            msg += decodeMessage(ndef_message);
        }

        values->push_back(msg);

        return checkRequirements(state, values);
    }

    std::string TriggerNfc::decodeMessage(nfc_ndef_message_t *nextMessage)
    {
        std::string msg = "";
        unsigned int recordCount;
        nfc_get_ndef_record_count(nextMessage, &recordCount);
        for (unsigned int j = 0; j < recordCount; j++) {
            nfc_ndef_record_t *newRecord;
            nfc_get_ndef_record(nextMessage, j, &newRecord);

//        char *recordType;
//        nfc_get_ndef_record_type(newRecord, &recordType);
//        QByteArray(recordType);

//        char *recordId;
//        nfc_get_ndef_record_id(newRecord, &recordId);
//        QByteArray(recordId);

//        tnf_type_t typeNameFormat;
//        nfc_get_ndef_record_tnf(newRecord, &typeNameFormat);

            uchar_t *payLoad;
            size_t payLoadSize;
            nfc_get_ndef_record_payload(newRecord, &payLoad, &payLoadSize);
            std::string payloadString(
                    std::string(
                            QByteArray(reinterpret_cast<const char*>(payLoad), payLoadSize).data()));
            if (j > 0)
                msg += ",";
            msg += payloadString;

//        delete recordType;
//        delete recordId;
            delete payLoad;
        }
        return msg;
    }

} /* namespace actiontrigger */
