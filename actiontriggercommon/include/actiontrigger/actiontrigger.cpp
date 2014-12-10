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

#include "actiontrigger/actiontrigger.hpp"
#include "actiontrigger/controller/headlessconnector.hpp"

namespace actiontrigger
{

    Logger* ActionTrigger::LOG = LoggerFactory::getInstance("ActionTrigger");
    ids_provider_t* ActionTrigger::ids_provider = NULL;

    ActionTrigger::ActionTrigger() :
            loaded(false), dataModelLibraryList(NULL), dataModelPermissionsList(NULL), dataModelGroupList(
            NULL), dataModelSearchList(NULL), procedures(
            NULL), permissions(NULL), settings(NULL), areProceduresLoaded(false)
    {
        LOG->trace("ActionTrigger");

        // Build Orphan Home
//    OrphanHome::build(this);

// Crash detection
        if ("TRUE"
                == qsettings.value("ACTIONTRIGGER_IS_RUNNING", "FALSE").toString().toStdString()) {
            LOG->trace("Detected app crash");
            this->detectedCrash = true;
        } else {
            this->detectedCrash = false;
        }
        qsettings.setValue("ACTIONTRIGGER_IS_RUNNING", "TRUE");

        // Premium support detection
        if (IS_BETA) {
            this->isPremium = true;
        } else if ("TRUE" == qsettings.value(PREMIUM_STAMP, "FALSE").toString().toStdString()) {
            this->isPremium = true;
        } else {
            this->isPremium = false;
        }

        // Number of times app was open counter
        // Get
        this->appOpenCount = qsettings.value("ACTIONTRIGGER_OPEN_COUNT", "0").toInt();
        // Set
        qsettings.setValue("ACTIONTRIGGER_OPEN_COUNT", QString(++this->appOpenCount));

        // Trial period setup
        this->trialStart =
                qsettings.value(TRIAL_PERIOD_STAMP, QDateTime::currentDateTimeUtc()).toDateTime();
        qsettings.setValue(TRIAL_PERIOD_STAMP, this->trialStart);

        // Setup headless
        HeadlessConnector* headless = HeadlessConnector::get();
        connect(headless, SIGNAL(procedureRunning(int)), this, SLOT(headlessProcedureRunning(int)));
        headless->start();

        // Setup user facing message signal propagation
        connect(GlobalResources::get(), SIGNAL(displayUserMessage(QString)), this,
                SLOT(displayUserMessagePropagate(QString)));
        connect(GlobalResources::get(), SIGNAL(postNotification(QVariantMap)), this,
                SLOT(postNotificationPropagate(QVariantMap)));
    }

    ActionTrigger::~ActionTrigger()
    {
        LOG->trace("~ActionTrigger");
        qsettings.setValue("ACTIONTRIGGER_IS_RUNNING", "FALSE");
    }

    qint32 ActionTrigger::getAppOpenCount()
    {
        return (qint32) this->appOpenCount;
    }

    int ActionTrigger::hasPremium()
    {
        return IS_BETA ? 2 : (this->isPremium ? 1 : 0);
    }

    void ActionTrigger::setPremium(bool value)
    {
        if (value)
            setPremiumSupport();
        else
            unsetPremiumSupport();
    }

    QVariantList ActionTrigger::premiumStatus()
    {
        QVariantList* response = new QVariantList();

        do {
            if (IS_BETA) {
                response->append(true);
                response->append("Unlimited Beta");
                break;
            }

            if (hasPremium()) {
                response->append(true);
                response->append("Premium account");
                break;
            }

            QDateTime currentTime = QDateTime::currentDateTimeUtc();
            QDateTime trialEnd = QDateTime(currentTime);
            trialEnd.addDays(TRIAL_PERIOD_IN_DAYS);

            if (currentTime < trialEnd) {
                response->append(true);
                response->append(
                        QString::fromStdString(
                                SSTR(
                                        "In trial period with " << currentTime.daysTo(trialEnd) << " days left")));
                break;
            }

            if (currentTime.date().dayOfWeek() == 2 /*Tuesday*/) {
                response->append(true);
                response->append("Trial period expired, currently in a Tuesday bypass");
                break;
            } else {
                response->append(false);
                response->append("Trial period expired");
                break;
            }

        } while (false);

        return *response;
    }

    void ActionTrigger::setPremiumSupport()
    {
        LOG->trace("Setting premium support");
        qsettings.setValue(PREMIUM_STAMP, "TRUE");
        this->isPremium = true;
    }

    void ActionTrigger::unsetPremiumSupport()
    {
        LOG->trace("Unsetting premium support");
        qsettings.setValue(PREMIUM_STAMP, "FALSE");
        this->isPremium = false;
    }

    bool ActionTrigger::checkPurchase(bb::platform::PurchaseReply* reply)
    {
        if (reply == NULL || !reply->receipt().isValid()) {
            return false;
        }
        setPremiumSupport();
        return true;
    }

    bool ActionTrigger::checkPurchase(bb::platform::ExistingPurchasesReply* reply)
    {
        if (reply == NULL || reply->purchases().count() > 0) {
            return false;
        }
        setPremiumSupport();
        return true;
    }

    bool ActionTrigger::hasDetectedCrash()
    {
        return this->detectedCrash ? 1 : 0;
    }

    void ActionTrigger::crash()
    {
        int *pointer = NULL;
        *pointer = 1;
    }

    QString ActionTrigger::getPersistentVar(QString key)
    {
        return qsettings.value(key).toString();
    }

    void ActionTrigger::setPersistentVar(QString key, QString val)
    {
        qsettings.setValue(key, val);
    }

    bool ActionTrigger::filter(QString content, QString filter)
    {
        LOG->trace(
                SSTR(
                        "Content: \"" << content.toStdString() << "\" Filter: \"" << filter.toStdString() << "\""));
        return content.simplified().replace(" ", "").indexOf(filter.simplified().replace(" ", ""),
                0, Qt::CaseInsensitive) != -1;
    }

    void ActionTrigger::headlessProcedureRunning(int id)
    {
        if (procedures) {
            for (std::vector<Procedure*>::iterator it = procedures->begin();
                    it != procedures->end(); ++it) {
                if (*it && (*it)->getId() == id) {
                    (*it)->alreadyRunningHeadless();
                    return;
                }
            }
        }
        // Couldn't find it, probably because we haven't loaded it yet
        queuedHeadlessRunning.push_back(id);
    }

    void ActionTrigger::populateProcedures(bb::system::SystemListDialog* dialog)
    {
        for (std::vector<Procedure*>::iterator it = this->procedures->begin();
                it != this->procedures->end(); ++it) {
            dialog->appendItem(QString::fromStdString((*it)->getName()));
        }
    }

    QObject* ActionTrigger::getProcedureFromDialogResult(bb::system::SystemListDialog* dialog)
    {
        return UserInterfaceFactory::getInstance()->displayProcedure(
                this->procedures->at(dialog->selectedIndices().first()));
    }

    QString ActionTrigger::serializeProcedure(QObject* procedure)
    {
        JsonExport je;
        je.add(((ProcedureCascadesView*) procedure)->getController());
        return je.exportToString();
    }

    void ActionTrigger::populateLibraryList(RegexDataModel* dataModel)
    {
        LOG->trace("populateLibraryList");
        LOG->loading("Drawing UI...");
        this->dataModelLibraryList = dataModel;
        ToolsCascades::populateDataModel(dataModel,
                UserInterfaceFactory::getInstance()->displayProcedures(this->procedures));
    }

    void ActionTrigger::populatePermissionsList(RegexDataModel* dataModel)
    {
        LOG->trace("populatePermissionsList");
        this->dataModelPermissionsList = dataModel;
        ToolsCascades::populateDataModel(dataModel,
                UserInterfaceFactory::getInstance()->displayPermissions(this->permissions));
    }

    bb::cascades::DataModel* ActionTrigger::getStatementGroupList()
    {
        LOG->trace("getStatementGroupList");
        return (bb::cascades::DataModel*) this->dataModelGroupList;
    }

    bb::cascades::DataModel* ActionTrigger::getStatementSearchList()
    {
        LOG->trace("getStatementSearchList");
        return (bb::cascades::DataModel*) this->dataModelSearchList;
    }

    void ActionTrigger::requestIdentity()
    {
        LOG->trace("requestIdentity");

        ids_result_t result;
        if (ids_provider == NULL) {
            // Step 1 - Initialize IDS.
            result = ids_initialize();
            if (result != IDS_SUCCESS) {
                LOG->warning(SSTR("Failure to initialize IDS, errno: " << strerror(errno)));
                QVariantMap result;
                result.insert("success", false);
                result.insert("error",
                        "Failed to retrieve Blackberry ID. You may not be signed in or allowed this application the proper permissions.");
                emit identityResult(result);
                return;
            }

            // Step 2 - Register to use BlackBerry ID as an identity provider.
            static int ids_fd = -1;

            result = ids_register_provider(BLACKBERRY_ID_PROVIDER, &ids_provider, &ids_fd);

            if (result != IDS_SUCCESS) {
                LOG->warning(
                        SSTR(
                                "Failure to initialize blackberry ID provider, errno: " << strerror(errno)));
                QVariantMap result;
                result.insert("success", false);
                result.insert("error",
                        "Failed to retrieve Blackberry ID. You may not be signed in or allowed this application the proper permissions.");
                emit identityResult(result);
                return;
            }

            // Step 3 - Add an FD handler for the IDS library communications.
            if (bps_add_fd(ids_fd, BPS_IO_INPUT, identityBpsIoHandler, this) != BPS_SUCCESS) {
                LOG->warning(
                        SSTR(
                                "Failure to add fd handler for IDS communication, errno: " << strerror(errno)));
                QVariantMap result;
                result.insert("success", false);
                result.insert("error",
                        "Failed to retrieve Blackberry ID. You may not be signed in or allowed this application the proper permissions.");
                emit identityResult(result);
                return;
            }
        }

// Step 4 - Request identity
        const char* properties[] = { IDS_BBID_PROP_USERNAME, IDS_BBID_PROP_FIRSTNAME,
        IDS_BBID_PROP_SCREENNAME, IDS_BBID_PROP_LASTNAME, IDS_BBID_PROP_UID };

        result = ids_get_properties(ids_provider, BBID_PROPERTY_CORE, 5, properties,
                identitySuccessHandler, identityFailureHandler, this, NULL);
        if (result != IDS_SUCCESS) {
            LOG->warning(
                    SSTR("Failure to retrieve blackberry id info, errno: " << strerror(errno)));
            QVariantMap result;
            result.insert("success", false);
            result.insert("error",
                    "Failed to retrieve Blackberry ID. You may not be signed in or allowed this application the proper permissions.");
            emit identityResult(result);
            return;
        }
    }

    int ActionTrigger::identityBpsIoHandler(int fd, int event, void* data)
    {
        (void) event;
        (void) data;
// process the message that's now available on that FD.
        if (ids_process_msg(fd) != IDS_SUCCESS) {
            return BPS_FAILURE;
        }
        return BPS_SUCCESS;
    }

    void ActionTrigger::identitySuccessHandler(ids_request_id_t request_id, int property_count,
            const ids_property_t* properties, void* cb_data)
    {
        (void) request_id;
        QVariantMap successResult;
        for (int i = 0; i < property_count; ++i) {
            successResult.insert(properties[i].name, properties[i].value);
        }
        successResult.insert("success", true);
        ((ActionTrigger*) cb_data)->emitIdentityResult(successResult);
    }

    void ActionTrigger::identityFailureHandler(ids_request_id_t request_id, ids_result_t result,
            const char* info, void* cb_data)
    {
        (void) request_id;
        (void) info;
        LOG->warning(SSTR("Blackberry id background retrieval failed, error: " << result));
        QVariantMap failureResult;
        failureResult.insert("success", false);
        failureResult.insert("error",
                "Failed to retrieve Blackberry ID. You may not be signed in or allowed this application the proper permissions.");
        ((ActionTrigger*) cb_data)->emitIdentityResult(failureResult);
    }

    void ActionTrigger::emitIdentityResult(QVariantMap result)
    {
        emit identityResult(result);
    }

    QString ActionTrigger::sign(QString content)
    {
        QCryptographicHash hash(QCryptographicHash::Sha1);
        hash.addData(content.toUtf8());
        hash.addData("Welcome to ActionTrigger, please rate our application in the app store.");
        return QString(hash.result().toBase64());
    }

    void ActionTrigger::loadStatementInfoList()
    {
        LOG->loading("Populating cache...");

// Categories
        this->dataModelGroupList = new RegexDataModel(this);
        this->dataModelGroupList->setSortingKeys(QList<QString>() << QString("name"));
        this->dataModelGroupList->setGrouping(bb::cascades::ItemGrouping::ByFullValue);
        std::vector<QObject*>* statementGroups =
                UserInterfaceFactory::getInstance()->displayStatementGroups(
                        ServiceManagement::getInstance()->getGroups());
        ToolsCascades::populateDataModel(this->dataModelGroupList, statementGroups);
        delete statementGroups;

// Search
        this->dataModelSearchList = new RegexDataModel(this);
        this->dataModelSearchList->setSortingKeys(QList<QString>() << QString("name"));
        this->dataModelSearchList->setGrouping(bb::cascades::ItemGrouping::ByFullValue);
        std::vector<QObject*>* statementInfos =
                UserInterfaceFactory::getInstance()->displayStatementInfos(
                        ServiceManagement::getInstance()->getAllInfos());
        ToolsCascades::populateDataModel(this->dataModelSearchList, statementInfos);
        delete statementInfos;
    }

    void ActionTrigger::setupLogOutputForLoading(bb::cascades::ArrayDataModel* splashLogDataModel)
    {
        LOG->trace("loadProcedures");

        if (NULL != splashLogDataModel) {
            BlackberryLogger::setContainerLoading(splashLogDataModel);
        }
        LOG->loading("Setup logging...");
    }

    void ActionTrigger::loadServices()
    {
        LOG->trace("loadServices");
        LOG->loading("Finding actions and triggers...");
        ServiceManagement::getInstance();
    }

    void ActionTrigger::loadProcedures()
    {
        LOG->trace("loadProcedures");

        procedures = new std::vector<Procedure*>();

// Make procedures globally available
        GlobalResources::get()->setProcedures(procedures);

        LOG->trace("Loading procedure Models");
        LOG->loading("Reading database...");
        std::vector<ProcedureModel*>* procedureModels =
                DatabaseFactory::getInstance()->loadProcedures();

        LOG->trace("Loading procedure Controllers");
        LOG->loading("Initializing procedures...");
        for (std::vector<ProcedureModel*>::iterator it = procedureModels->begin();
                it != procedureModels->end(); ++it) {
            LOG->trace("Loading... " + (*it)->getName());
//        LOG->loading("     " + (*it)->getName());
            procedures->push_back(new Procedure(*it));
            LOG->trace("Loaded... " + (*it)->getName());
        }
        delete procedureModels;
        LOG->trace("Loading complete");
        this->areProceduresLoaded = true;
    }

    void ActionTrigger::loadPermissions()
    {
        LOG->trace("loadServices");
        LOG->loading("Checking permissions...");
        permissions = PermissionManagement::getInstance()->getPermissions();
    }

    void ActionTrigger::loadSettings()
    {
        LOG->trace("loadSettings");
        LOG->loading("Loading settings...");
        settings = DatabaseFactory::getInstance()->getSettingsModel();
    }

    void ActionTrigger::loadPersistentVariables()
    {
        LOG->trace("loadPersistentVariables");
        LOG->loading("Loading Persistent variables...");
        PersistentStorage::load();
    }

    QObject* ActionTrigger::createProcedure()
    {
        Procedure* procedure = new Procedure(std::string(DEFAULT_PROCEDURE_NAME));
        procedures->push_back(procedure);
        QObject* procedureView = UserInterfaceFactory::getInstance()->displayProcedure(procedure);
        if (dataModelLibraryList) {
            dataModelLibraryList->insert(procedureView);
        }
        return procedureView;
    }

    QObject* ActionTrigger::getGlobalResources()
    {
        return (QObject*) GlobalResources::get();
    }

    void ActionTrigger::load(RegexDataModel* dataModelLibraryList, bool resetApplication,
            bool preventAutoStart)
    {
        // Load
        DatabaseFactory::getInstance(resetApplication);
        this->loadSettings();
        this->loadServices();
        this->loadPersistentVariables();
        this->loadProcedures();
        this->loadStatementInfoList();
        // TODO Implement permissions
        //this->loadPermissions();
        if (!preventAutoStart) {
            this->autostart();
        }

        this->populateLibraryList(dataModelLibraryList);
        // TODO Implement permissions
        //this->populatePermissionsList(dataModelPermissionsList);

        loaded = true;
        LOG->loading("Done");
    }

    void ActionTrigger::autostart()
    {
        LOG->loading("Auto start procedures");
        for (std::vector<Procedure*>::iterator it = procedures->begin(); it != procedures->end();
                ++it) {
            if (*it) {
                bool shouldStart = false;
                for (std::vector<int>::iterator itId = queuedHeadlessRunning.begin();
                        itId != queuedHeadlessRunning.end(); ++itId) {
                    if ((*itId) == (*it)->getId()) {
                        LOG->loading("     " + (*it)->getName() + " (running)");
                        shouldStart = true;
                        break;
                    }
                }
                if (!shouldStart) {
                    if ((*it)->getSetting("AUTO_START").compare("TRUE") == 0) {
                        LOG->loading("     " + (*it)->getName());
                        shouldStart = true;
                    } else {
                        for (std::vector<std::string>::iterator itUuid = queuedAutoStarts.begin();
                                itUuid != queuedAutoStarts.end(); ++itUuid) {
                            if ((*itUuid).compare((*it)->getUuid()) == 0) {
                                LOG->loading("     " + (*it)->getName() + " (invoked)");
                                shouldStart = true;
                                break;
                            }
                        }
                    }
                }
                if (shouldStart) {
                    ProcedureCascadesView* procedureView =
                            (ProcedureCascadesView*) UserInterfaceFactory::getInstance()->displayProcedure(
                                    *it);
                    procedureView->start();
                }
            } else {
                LOG->critical("NULL pointer it");
            }
        }
        LOG->trace("Done auto starting");
    }

    void ActionTrigger::startAllProcedures()
    {
        for (std::vector<Procedure*>::iterator it = procedures->begin(); it != procedures->end();
                ++it) {
            if (*it) {
                LOG->trace(" Starting " + (*it)->getName());
                ProcedureCascadesView* procedureView =
                        (ProcedureCascadesView*) UserInterfaceFactory::getInstance()->displayProcedure(
                                *it);
                procedureView->start();
            } else {
                LOG->critical("NULL pointer it");
            }
        }
    }

    void ActionTrigger::stopAllProcedures()
    {
        for (std::vector<Procedure*>::iterator it = procedures->begin(); it != procedures->end();
                ++it) {
            if (*it) {
                LOG->trace(" Stopping " + (*it)->getName());
                ProcedureCascadesView* procedureView =
                        (ProcedureCascadesView*) UserInterfaceFactory::getInstance()->displayProcedure(
                                *it);
                procedureView->stop();
            } else {
                LOG->critical("NULL pointer it");
            }
        }
    }

    QString ActionTrigger::getSetting(QString name)
    {
        return QString::fromStdString(settings->getSetting(name.toStdString()).c_str());
    }

    void ActionTrigger::setSetting(QString name, QString value)
    {
        settings->setSetting(name.toStdString(), value.toStdString());
    }

    QString ActionTrigger::duplicate(QVariant procedure)
    {
        LOG->trace("duplicate()");
        if (procedure.isNull() || !procedure.isValid()) {
            return "Failed to duplicate";
        }
        QObject* obj = qvariant_cast<QObject*>(procedure);
        ProcedureCascadesView* view = qobject_cast<ProcedureCascadesView*>(obj);
        Procedure* newProc = new Procedure(view->getController()->toQVariant());
        procedures->push_back(newProc);
        ProcedureCascadesView* newView =
                (ProcedureCascadesView*) UserInterfaceFactory::getInstance()->displayProcedure(
                        newProc);
        newView->setFlag("Duplicated");
        this->dataModelLibraryList->insert(newView);
        return "Duplicated";
    }

    QString ActionTrigger::exportData(QString filename, QVariant proceduresVariant)
    {
        LOG->trace("Exporting to file: " + filename.toStdString());
        JsonExport* je = new JsonExport();
        if (proceduresVariant.isNull()) {
            // Export all
            for (std::vector<Procedure*>::iterator it = procedures->begin();
                    it != procedures->end(); ++it) {
                if (*it) {
                    je->add(*it);
                }
            }
        } else {
            // Export selected
            if (proceduresVariant.type() == QVariant::List) {
                QVariantList proceduresList = proceduresVariant.toList();
                for (QVariantList::const_iterator it = proceduresList.begin();
                        it != proceduresList.end(); ++it) {
                    QObject* obj = qvariant_cast<QObject*>(*it);
                    ProcedureCascadesView* view = qobject_cast<ProcedureCascadesView*>(obj);
                    je->add(view->getController());
                }
            } else {
                LOG->warning("Export: given proceduresVariant is not of type List");
            }
        }
        if (!je->exportToFile(filename)) {
            delete je;
            LOG->error("Failed to export: " + je->getLastErrMsg().toStdString());
            return "Failed to export: " + je->getLastErrMsg();
        }
        delete je;
        return "Exported";
    }

    QString ActionTrigger::importDataFromFile(QString filename)
    {
        LOG->trace("Importing file: " + filename.toStdString());
        JsonExport* je = new JsonExport();
        if (!je->loadFromFile(filename)) {
            QString errMsg = je->getLastErrMsg();
            delete je;
            return errMsg;
        }
        std::vector<Procedure*>* importedProcedures;
        try {
            importedProcedures = je->import();
        } catch (...) {
            return "Malformed data";
        }
        delete je;
        for (std::vector<Procedure*>::iterator iter = importedProcedures->begin();
                iter != importedProcedures->end(); ++iter) {
            this->procedures->push_back(*iter);
            ProcedureCascadesView* view =
                    (ProcedureCascadesView*) UserInterfaceFactory::getInstance()->displayProcedure(
                            *iter);
            view->setFlag("Imported");
            this->dataModelLibraryList->insert(view);
        }
        return "Imported";
    }

    QString ActionTrigger::importDataFromString(QString rawData)
    {
        LOG->trace("Importing data");
        JsonExport* je = new JsonExport();
        if (!je->loadFromString(rawData)) {
            QString errMsg = je->getLastErrMsg();
            delete je;
            return errMsg;
        }
        std::vector<Procedure*>* importedProcedures;
        try {
            importedProcedures = je->import();
        } catch (...) {
            return "Malformed data";
        }
        delete je;
        for (std::vector<Procedure*>::iterator iter = importedProcedures->begin();
                iter != importedProcedures->end(); ++iter) {
            Procedure* procedure = *iter;
            procedure->setGroup("Downloads");

            ProcedureCascadesView* view =
                    (ProcedureCascadesView*) UserInterfaceFactory::getInstance()->displayProcedure(
                            procedure);
            view->setFlag("Downloaded");

            this->procedures->push_back(procedure);
            this->dataModelLibraryList->insert(view);
        }
        return "Downloaded";
    }

    void ActionTrigger::processEventLoop()
    {
        QCoreApplication::processEvents();
    }

    void ActionTrigger::displayUserMessagePropagate(QString msg)
    {
        emit displayUserMessage(msg);
    }

    void ActionTrigger::postNotificationPropagate(QVariantMap data)
    {
        emit postNotification(data);
    }

    void ActionTrigger::onInvoke(const bb::system::InvokeRequest& request)
    {
        std::string action = request.action().toStdString();

// Shortcut from homescreen with only the uri set
        if ("actiontrigger" == request.uri().scheme()) {
            // The uuid is embedded in the "authority" of the url
            std::string uuid = request.uri().authority().toUpper().toStdString();

            if (!areProceduresLoaded) {
                this->queuedAutoStarts.push_back(uuid);
                LOG->trace(SSTR("Invocation: Not initialized"));
                return;
            }
            for (std::vector<Procedure*>::iterator it = procedures->begin();
                    it != procedures->end(); ++it) {
                LOG->trace(SSTR("Invocation: checking procedure uuid: " << (*it)->getUuid()));
                if ((*it)->getUuid() == uuid) {
                    if ((*it)->prepareStart()) {
                        ProcedureWork* work = new ProcedureWork((*it));
                        work->start();
                        GlobalResources::get()->sendUserFacingMessage(
                                QString::fromStdString("Starting " + (*it)->getName()));
                        LOG->trace("Starting: " + (*it)->getName());
                    } else {
                        GlobalResources::get()->sendUserFacingMessage(
                                "Cannot start: Already starting");
                        LOG->trace("Procedure already starting: " + (*it)->getName());
                    }
                    return; // Successful return
                }
            }

            GlobalResources::get()->sendUserFacingMessage("Cannot start: Not found");
            LOG->trace(SSTR("Invocation: Unknown uuid: " << uuid));
        } else {
            LOG->trace(
                    SSTR(
                            "Invocation: Unknown invocation action: " << request.action().toStdString() << " scheme:" << request.uri().scheme().toStdString() << " authority:" << request.uri().authority().toStdString()));
        }
    }

} /* namespace actiontrigger */
