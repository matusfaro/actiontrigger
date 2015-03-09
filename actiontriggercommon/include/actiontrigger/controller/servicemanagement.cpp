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

#include "actiontrigger/controller/servicemanagement.hpp"

#include "actiontrigger/controller/services/noop/statementcorrupt.hpp"
#include "actiontrigger/controller/services/console/actionconsoleout.hpp"
#include "actiontrigger/controller/services/geolocation/actiongeolocationretrieve.hpp"
#include "actiontrigger/controller/services/system/actionsystemnop.hpp"
#include "actiontrigger/controller/services/system/actionbattery.hpp"
#include "actiontrigger/controller/services/system/actionled.hpp"
#include "actiontrigger/controller/services/system/actionsetwallpaper.hpp"
#include "actiontrigger/controller/services/system/actionmemory.hpp"
#include "actiontrigger/controller/services/system/triggersystemtrue.hpp"
#include "actiontrigger/controller/services/system/triggersystemfalse.hpp"
#include "actiontrigger/controller/services/system/triggerbattery.hpp"
#include "actiontrigger/controller/services/vibration/actionvibrate.hpp"
#include "actiontrigger/controller/services/wifi/triggerwifistatus.hpp"
#include "actiontrigger/controller/services/wifi/actionwifipower.hpp"
#include "actiontrigger/controller/services/nfc/triggernfc.hpp"
#include "actiontrigger/controller/services/sensors/triggerlinearacceleration.hpp"
#include "actiontrigger/controller/services/sensors/triggeraccelerometer.hpp"
#include "actiontrigger/controller/services/sensors/triggeraltimeter.hpp"
#include "actiontrigger/controller/services/sensors/triggergravity.hpp"
#include "actiontrigger/controller/services/sensors/triggergyroscope.hpp"
#include "actiontrigger/controller/services/sensors/triggerlight.hpp"
#include "actiontrigger/controller/services/sensors/triggermagnetometer.hpp"
#include "actiontrigger/controller/services/sensors/triggerproximity.hpp"
#include "actiontrigger/controller/services/sensors/triggertemperature.hpp"
#include "actiontrigger/controller/services/sensors/triggerrotationmatrix.hpp"
#include "actiontrigger/controller/services/sensors/triggerazimuthpitchroll.hpp"
#include "actiontrigger/controller/services/sensors/triggerorientation.hpp"
#include "actiontrigger/controller/services/sensors/triggerpressure.hpp"
#include "actiontrigger/controller/services/sensors/triggercompass.hpp"
#include "actiontrigger/controller/services/sensors/actionaccelerometer.hpp"
#include "actiontrigger/controller/services/sensors/actionmagnetometer.hpp"
#include "actiontrigger/controller/services/sensors/actiongyroscope.hpp"
#include "actiontrigger/controller/services/sensors/actionaltimeter.hpp"
#include "actiontrigger/controller/services/sensors/actiontemperature.hpp"
#include "actiontrigger/controller/services/sensors/actionproximity.hpp"
#include "actiontrigger/controller/services/sensors/actionlight.hpp"
#include "actiontrigger/controller/services/sensors/actiongravity.hpp"
#include "actiontrigger/controller/services/sensors/actionlinearacceleration.hpp"
#include "actiontrigger/controller/services/sensors/actionorientation.hpp"
#include "actiontrigger/controller/services/sensors/actionrotationmatrix.hpp"
#include "actiontrigger/controller/services/sensors/actionazimuthpitchroll.hpp"
#include "actiontrigger/controller/services/sensors/actionpressure.hpp"
#include "actiontrigger/controller/services/sensors/actioncompass.hpp"
#include "actiontrigger/controller/services/invocation/actioninvokecustom.hpp"
#include "actiontrigger/controller/services/invocation/actioninvokebbm.hpp"
#include "actiontrigger/controller/services/invocation/actioninvokephone.hpp"
#include "actiontrigger/controller/services/clock/triggeralarm.hpp"
#include "actiontrigger/controller/services/clock/triggertimerange.hpp"
#include "actiontrigger/controller/services/clock/actiontime.hpp"
#include "actiontrigger/controller/services/clock/actionsleep.hpp"
#include "actiontrigger/controller/services/clock/actionhalt.hpp"
#include "actiontrigger/controller/services/device/actionsetclipboard.hpp"
#include "actiontrigger/controller/services/device/actiongetclipboard.hpp"
#include "actiontrigger/controller/services/device/triggercustom.hpp"
#include "actiontrigger/controller/services/device/triggerholster.hpp"
#include "actiontrigger/controller/services/device/triggerscreenlock.hpp"
#include "actiontrigger/controller/services/device/triggerbedsidemode.hpp"
#include "actiontrigger/controller/services/device/actionholster.hpp"
#include "actiontrigger/controller/services/device/actionscreenlock.hpp"
#include "actiontrigger/controller/services/device/actionbedsidemode.hpp"
#include "actiontrigger/controller/services/device/actionprofile.hpp"
#include "actiontrigger/controller/services/device/actiongetprofile.hpp"
#include "actiontrigger/controller/services/device/triggerprofile.hpp"
#include "actiontrigger/controller/services/phone/actionplacecall.hpp"
#include "actiontrigger/controller/services/phone/triggercallupdated.hpp"
#include "actiontrigger/controller/services/phone/actioncellularinfo.hpp"
#include "actiontrigger/controller/services/phone/triggercellularinfo.hpp"
#include "actiontrigger/controller/services/camera/actionflash.hpp"
#include "actiontrigger/controller/services/cover/actioncovercontent.hpp"
#include "actiontrigger/controller/services/cover/actioncovertitle.hpp"
#include "actiontrigger/controller/services/bluetooth/actionbluetoothpower.hpp"
#include "actiontrigger/controller/services/bluetooth/triggerbluetoothstatus.hpp"
#include "actiontrigger/controller/services/filesystem/actionrandomfile.hpp"
#include "actiontrigger/controller/services/hub/triggerreceivemessage.hpp"
#include "actiontrigger/controller/services/hub/triggerreceiveopenmessage.hpp"
#include "actiontrigger/controller/services/hub/actionsendmessage.hpp"
#include "actiontrigger/controller/services/internal/actionnotification.hpp"
#include "actiontrigger/controller/services/internal/actionpostnotification.hpp"
#include "actiontrigger/controller/services/internal/actionprocedurestartstop.hpp"
#include "actiontrigger/controller/services/internal/actiondisplaytoast.hpp"
#include "actiontrigger/controller/services/internal/actionsetvar.hpp"
#include "actiontrigger/controller/services/internal/actionmath.hpp"
#include "actiontrigger/controller/services/internal/actionrandom.hpp"
#include "actiontrigger/controller/services/internal/actiongetglobalvar.hpp"
#include "actiontrigger/controller/services/internal/actionsetglobalvar.hpp"
#include "actiontrigger/controller/services/internal/actiongetpersistentvar.hpp"
#include "actiontrigger/controller/services/internal/actionsetpersistentvar.hpp"
#include "actiontrigger/controller/services/internal/actionregex.hpp"
#include "actiontrigger/controller/services/internal/triggerregex.hpp"
#include "actiontrigger/controller/services/advanced/actionshell.hpp"
#include "actiontrigger/controller/services/advanced/triggernetworkusage.hpp"
#include "actiontrigger/controller/services/advanced/actionnetworkusage.hpp"
#include "actiontrigger/controller/services/advanced/actionclearnetworkusage.hpp"
#include "actiontrigger/controller/services/advanced/actionboottime.hpp"
#include "actiontrigger/controller/services/advanced/actionjavascript.hpp"
#include "actiontrigger/controller/services/advanced/actionpython.hpp"
#include "actiontrigger/controller/services/advanced/actionksh.hpp"
#include "actiontrigger/controller/services/flow/branchif.hpp"
#include "actiontrigger/controller/services/flow/branchifthen.hpp"
#include "actiontrigger/controller/services/flow/branchwhile.hpp"
#include "actiontrigger/controller/services/flow/branchwaituntil.hpp"
#include "actiontrigger/controller/services/flow/branchdowhile.hpp"
#include "actiontrigger/controller/services/flow/actionbreak.hpp"
#include "actiontrigger/controller/services/flow/actioncontinue.hpp"
#include "actiontrigger/controller/services/media/actionrecordaudio.hpp"
#include "actiontrigger/controller/services/media/actionrecordvideo.hpp"
#include "actiontrigger/controller/services/media/actionrecorddesktop.hpp"
#include "actiontrigger/controller/services/media/actionrecordpicture.hpp"
#include "actiontrigger/controller/services/media/actionrecorddesktoppicture.hpp"
#include "actiontrigger/controller/services/media/actioncustomsound.hpp"
#include "actiontrigger/controller/services/media/actionsystemsound.hpp"
#include "actiontrigger/controller/services/media/actiongetvolume.hpp"
#include "actiontrigger/controller/services/media/actionsetvolume.hpp"

namespace actiontrigger
{

    Logger* ServiceManagement::LOG = LoggerFactory::getInstance("ServiceManagement");

    ServiceManagement* ServiceManagement::instance = NULL;

    ServiceManagement* ServiceManagement::getInstance()
    {
        if (!instance) {
            instance = new ServiceManagement();
        }
        return instance;
    }

    ServiceManagement::ServiceManagement()
    {
        // Initialize statement cache
        triggerMap = new std::map<StatementModel*, Trigger*>();
        actionMap = new std::map<StatementModel*, Action*>();
        // Initialize groups holder
        groups = new std::vector<StatementGroup*>();
        all = new std::vector<StatementInfo>();
        // Initialiaze statement factory holders
        triggers = new std::map<std::string, StatementFactory<Trigger>*>();
        actions = new std::map<std::string, StatementFactory<Action>*>();
        branches = new std::map<std::string, StatementFactory<Branch>*>();

        /*
         * Insert new services here
         */

        {
            StatementGroup* group = newGroup("Clock", "Time based triggers and information");
            addActionToGroup<ActionSleep>(group);
            addActionToGroup<ActionTime>(group);
            addTriggerToGroup<TriggerAlarm>(group);
            addTriggerToGroup<TriggerTimeRange>(group);
        }

        // Invocation stopped working... taking it out for now
        {
            addActionDeprecated<ActionInvokeBbm>();
            addActionDeprecated<ActionInvokePhone>();
//        StatementGroup* group = newGroup("Invocation Framework", "Invokes external applications, services and cards");
//        addActionToGroup<ActionInvokeBbm>(group);
//        addActionToGroup<ActionInvokePhone>(group);
        }

        {
            StatementGroup* group = newGroup("Sensors", "Control and monitor sensors");
            addActionToGroup<ActionGeoLocationRetrieve>(group);
            addActionToGroup<ActionGravity>(group);
            addTriggerToGroup<TriggerGravity>(group);
            addActionToGroup<ActionLinearAcceleration>(group);
            addTriggerToGroup<TriggerLinearAcceleration>(group);
            addActionToGroup<ActionAccelerometer>(group);
            addTriggerToGroup<TriggerAccelerometer>(group);
            addActionToGroup<ActionAltimeter>(group);
            addTriggerToGroup<TriggerAltimeter>(group);
            addActionToGroup<ActionGyroscope>(group);
            addTriggerToGroup<TriggerGyroscope>(group);
            addActionToGroup<ActionProximity>(group);
            addTriggerToGroup<TriggerProximity>(group);
            addActionToGroup<ActionRotationMatrix>(group);
            addTriggerToGroup<TriggerRotationMatrix>(group);
            addActionToGroup<ActionAzimuthPitchRoll>(group);
            addTriggerToGroup<TriggerAzimuthPitchRoll>(group);
            addActionToGroup<ActionOrientation>(group);
            addTriggerToGroup<TriggerOrientation>(group);
            addActionToGroup<ActionCompass>(group);
            addTriggerToGroup<TriggerCompass>(group);

            addActionToGroup<ActionLight>(group);
            addActionToGroup<ActionMagnetometer>(group);
            addActionToGroup<ActionTemperature>(group);
            addActionToGroup<ActionPressure>(group);
            addTriggerToGroup<TriggerLight>(group);
            addTriggerToGroup<TriggerMagnetometer>(group);
            addTriggerToGroup<TriggerTemperature>(group);
            addTriggerToGroup<TriggerPressure>(group);
        }

        {
            StatementGroup* group = newGroup("Media", "Audio and video controls");
            addActionToGroup<ActionCustomSound>(group);
            addActionToGroup<ActionSystemSound>(group);
//        addActionToGroup<ActionRecordDesktopPicture>(group);
            addActionToGroup<ActionRecordPicture>(group);
            addActionToGroup<ActionRecordAudio>(group);
            addActionToGroup<ActionRecordVideo>(group);
            addActionToGroup<ActionRecordDesktop>(group);
            addActionToGroup<ActionSetWallpaper>(group);
            addActionToGroup<ActionFlash>(group);
            addActionToGroup<ActionRandomFile>(group);
            addActionToGroup<ActionGetVolume>(group);
            addActionToGroup<ActionSetVolume>(group);
        }

        {
            StatementGroup* group = newGroup("Phone", "Control and monitor phone");
            addActionToGroup<ActionPlaceCall>(group);
            addTriggerToGroup<TriggerCallUpdated>(group);
            addActionToGroup<ActionCellularInfo>(group);
            addTriggerToGroup<TriggerCellularInfo>(group);
        }

        {
            StatementGroup* group = newGroup("Device", "Control and monitor device");
            addActionToGroup<ActionSetClipboard>(group);
            addActionToGroup<ActionGetClipboard>(group);
            addActionToGroup<ActionPostNotification>(group);
            addActionToGroup<ActionProfile>(group);
            addActionToGroup<ActionGetProfile>(group);
            addTriggerToGroup<TriggerProfile>(group);
            addActionToGroup<ActionBootTime>(group);
            addActionToGroup<ActionVibrate>(group);
            addActionToGroup<ActionBedsideMode>(group);
            addTriggerToGroup<TriggerBedsideMode>(group);
            addActionToGroup<ActionBattery>(group);
            addActionToGroup<ActionLed>(group);
            addActionToGroup<ActionMemory>(group);
            addTriggerToGroup<TriggerBattery>(group);
            addActionToGroup<ActionScreenLock>(group);
            addTriggerToGroup<TriggerScreenLock>(group);
            addActionToGroup<ActionHolster>(group);
            addTriggerToGroup<TriggerHolster>(group);
            addActionDeprecated<ActionSystemNop>();
        }

        {
            StatementGroup* group = newGroup("Hub", "Manage accounts and messages in hub.");
            addActionToGroup<ActionNotification>(group);
            addActionToGroup<ActionSendMessage>(group);
            addTriggerToGroup<TriggerReceiveMessage>(group);
            addTriggerToGroup<TriggerReceiveOpenMessage>(group);
        }

        {
            StatementGroup* group = newGroup("Application", "Manage application resources.");
            addTriggerToGroup<TriggerCustom>(group);
            addActionToGroup<ActionDisplayToast>(group);
            addActionToGroup<ActionCoverTitle>(group);
            addActionToGroup<ActionCoverContent>(group);
            addActionToGroup<ActionConsoleOut>(group);
            addActionToGroup<ActionProcedureStartStop>(group);
            addActionToGroup<ActionSetVar>(group);
            addActionToGroup<ActionRandom>(group);
            addActionToGroup<ActionMath>(group);
            addActionToGroup<ActionGetGlobalVar>(group);
            addActionToGroup<ActionSetGlobalVar>(group);
            addActionToGroup<ActionGetPersistentVar>(group);
            addActionToGroup<ActionSetPersistentVar>(group);
        }

        {
            StatementGroup* group = newGroup("Advanced", "Advanced actions.");
            addActionToGroup<ActionRegex>(group);
            addTriggerToGroup<TriggerRegex>(group);
            addActionToGroup<ActionInvokeCustom>(group);
            addActionToGroup<ActionJavascript>(group);
            addActionToGroup<ActionPython>(group);
            addActionToGroup<ActionKsh>(group);
            addActionDeprecated<ActionShell>(); // Deprecated by ActionKsh
        }

        {
            StatementGroup* group = newGroup("Network", "Control and monitor network services");
            // Not implemented yet
            // addTriggerToGroup<TriggerNfc>(group);
            addActionToGroup<ActionWifiPower>(group);
            addTriggerToGroup<TriggerWifiStatus>(group);
            addTriggerToGroup<TriggerBluetoothStatus>(group);
            addActionToGroup<ActionBluetoothPower>(group);
            addTriggerToGroup<TriggerNetworkUsage>(group);
            addActionToGroup<ActionNetworkUsage>(group);
            addActionToGroup<ActionClearNetworkUsage>(group);
        }

        {
            StatementGroup* group = newGroup("Execution flow",
                    "Handles various branching, loops and conditionals.");
            addActionToGroup<ActionHalt>(group);
            addActionToGroup<ActionBreak>(group);
            addActionToGroup<ActionContinue>(group);
            addBranchToGroup<BranchIf>(group);
            addBranchToGroup<BranchIfThen>(group);
            addBranchToGroup<BranchWhile>(group);
            addBranchToGroup<BranchDoWhile>(group);
            addBranchToGroup<BranchWaitUntil>(group);
            addTriggerToGroup<TriggerSystemTrue>(group);
            addTriggerToGroup<TriggerSystemFalse>(group);
        }
    }

    ServiceManagement::~ServiceManagement()
    {
    }

    std::vector<StatementGroup*>* ServiceManagement::getGroups()
    {
        LOG->trace("getGroups");
        return this->groups;
    }

    std::vector<StatementInfo>* ServiceManagement::getAllInfos()
    {
        LOG->trace("getAllInfos");
        return this->all;
    }

    Trigger* ServiceManagement::loadTrigger(StatementModel* model)
    {
        LOG->trace("loadTrigger");
        // Check for existing instance
        if (triggerMap->find(model) == triggerMap->end()) {
            Trigger* myTrigger;

            // Find trigger
            if (this->triggers->find(model->getType()) != this->triggers->end()) {
                myTrigger = this->triggers->find(model->getType())->second->getStatement(model);
            } else {
                LOG->error("Cannot load statement: unknown statement");
                myTrigger = new StatementCorrupt(model);
            }

            // Recursively initialize children
            if (NULL != model->getNextStatement()) {
                myTrigger->initNextStatement(this->loadTrigger(model->getNextStatement()));
            }

            triggerMap->insert(std::pair<StatementModel*, Trigger*>(model, myTrigger));
        }
        return triggerMap->find(model)->second;
    }

    Action* ServiceManagement::loadAction(StatementModel* model)
    {
        LOG->trace("loadAction");
        // Check for existing instance
        if (actionMap->find(model) == actionMap->end()) {
            Action* myAction;

            // Find action
            if (this->actions->find(model->getType()) != this->actions->end()) {
                myAction = this->actions->find(model->getType())->second->getStatement(model);
            } else if (this->branches->find(model->getType()) != this->branches->end()) {
                myAction = (Action*) this->branches->find(model->getType())->second->getStatement(
                        model);
            } else if (this->triggers->find(model->getType()) != this->triggers->end()) {
                myAction = (Action*) this->triggers->find(model->getType())->second->getStatement(
                        model);
            } else {
                LOG->error(SSTR("Cannot load statement: unknown statement" << model->getType()));
                myAction = (Action*) new StatementCorrupt(model);
            }

            // Recursively initialize branches
            if (!myAction->getBranchInfo().empty()) {
                std::map<std::string, Trigger*>* triggerBranches = new std::map<std::string,
                        Trigger*>();
                std::map<std::string, Action*>* actionBranches =
                        new std::map<std::string, Action*>();
                std::vector<branch_info> branches = myAction->getBranchInfo();
                for (std::vector<branch_info>::iterator it = branches.begin(); it != branches.end();
                        ++it) {
                    StatementModel* myBranchStatementModel = model->getBranchStatement(it->key);
                    if (myBranchStatementModel == NULL) {
                        continue;
                    }
                    if (it->type == "TRIGGER") {
                        triggerBranches->insert(
                                std::pair<std::string, Trigger*>(it->key,
                                        this->loadTrigger(myBranchStatementModel)));
                    } else if (it->type == "ACTION") {
                        actionBranches->insert(
                                std::pair<std::string, Action*>(it->key,
                                        this->loadAction(myBranchStatementModel)));
                    } else {
                        LOG->warning("Invalid branch type: " + it->type);
                    }
                }
                myAction->initBranches(triggerBranches, actionBranches);
            }

            // Recursively initialize children
            if (NULL != model->getNextStatement()) {
                myAction->initNextStatement(this->loadAction(model->getNextStatement()));
            }

            actionMap->insert(std::pair<StatementModel*, Action*>(model, myAction));
        }
        return actionMap->find(model)->second;
    }

    StatementGroup* ServiceManagement::newGroup(std::string name, std::string description)
    {
        LOG->trace("newGroup");
        StatementGroup* group = new StatementGroup(name, description);
        groups->push_back(group);
        return group;
    }

} /* namespace actiontrigger */
