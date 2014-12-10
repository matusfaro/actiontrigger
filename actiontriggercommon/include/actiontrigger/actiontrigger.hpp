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

#ifndef ACTIONTRIGGER_HPP_
#define ACTIONTRIGGER_HPP_

#define DEFAULT_PROCEDURE_NAME "New Procedure"

#define IS_BETA true
#define PREMIUM_STAMP "ACTIONTRIGGER_IS_PREMIUM"
#define TRIAL_PERIOD_STAMP "ACTIONTRIGGER_TRIAL_PERIOD_START_JAN_2014"
#define TRIAL_PERIOD_IN_DAYS (8)

#include "errno.h"
#include <string>
#include <bps/bps.h>
#include <ids.h>
#include <ids_blackberry_id.h>
#include "actiontrigger/tools/orphanhome.hpp"
#include <QSettings>
#include <QVariantMap>
#include "actiontrigger/view/userinterface.hpp"
#include "actiontrigger/view/userinterfacefactory.hpp"
#include "actiontrigger/view/cascades/cascades.hpp"
#include <vector>
#include "actiontrigger/view/cascades/procedurecascadesview.hpp"
#include "actiontrigger/view/cascades/servicecascadesview.hpp"
#include "actiontrigger/model/databasefactory.hpp"
#include "actiontrigger/model/database/database.hpp"
#include "actiontrigger/controller/procedure.hpp"
#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/GroupDataModel>
#include "actiontrigger/view/cascades/regexdatamodel.hpp"
#include "actiontrigger/view/cascades/toolscascades.hpp"
#include <QObject>
#include <QCryptographicHash>
#include "actiontrigger/model/logger/blackberry/blackberrylogger.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/tools/work.hpp"
#include "actiontrigger/tools/unitofwork.hpp"
#include "actiontrigger/controller/permissionmanagement.hpp"
#include <bb/cascades/itemgrouping>
#include "actiontrigger/controller/globalresources.hpp"
#include "actiontrigger/model/export/jsonexport.hpp"
#include "bb/platform/ExistingPurchasesReply.hpp"
#include <bb/platform/PurchaseReply>
#include <bb/platform/ExistingPurchasesReply>
#include "actiontrigger/model/database/sqlite/persistentstorage.hpp"
#include <bb/system/InvokeManager>
#include <bb/system/SystemListDialog>

namespace actiontrigger
{

    class ActionTrigger: public QObject
    {
    Q_OBJECT
    public:
        ActionTrigger();
        virtual ~ActionTrigger();

        /**
         * 0 - Trial
         * 1 - Premium
         * 2 - Beta
         */
        Q_INVOKABLE
        int hasPremium();Q_INVOKABLE
        void setPremium(bool value);

        /**
         * Returns list of two qvariants:
         * 1. Boolean if full experience enabled
         * 2. String description why experience is degraded
         */
        Q_INVOKABLE
        QVariantList premiumStatus();

        Q_INVOKABLE
        bool checkPurchase(bb::platform::PurchaseReply* reply);Q_INVOKABLE
        bool checkPurchase(bb::platform::ExistingPurchasesReply* reply);

        Q_INVOKABLE
        bool hasDetectedCrash();Q_INVOKABLE
        void crash();

        Q_INVOKABLE
        QString getPersistentVar(QString key);Q_INVOKABLE
        void setPersistentVar(QString key, QString val);

        Q_INVOKABLE
        qint32 getAppOpenCount();

        Q_INVOKABLE
        void setupLogOutputForLoading(bb::cascades::ArrayDataModel* splashLogDataModel = NULL);Q_INVOKABLE
        void load(RegexDataModel* dataModelLibraryList, bool resetApplication,
                bool preventAutoStart);

        Q_INVOKABLE
        bb::cascades::DataModel* getStatementGroupList();Q_INVOKABLE
        bb::cascades::DataModel* getStatementSearchList();

        Q_INVOKABLE
        QObject* createProcedure();

        Q_INVOKABLE
        QString getSetting(QString name);Q_INVOKABLE
        void setSetting(QString name, QString value);

        Q_INVOKABLE
        QObject* getGlobalResources();

        /**
         * Requests Blackberry ID identity. The identityResult signal is emitted when done.
         */
        Q_INVOKABLE
        void requestIdentity();

        Q_INVOKABLE
        QString sign(QString content);

        Q_INVOKABLE
        void startAllProcedures();Q_INVOKABLE
        void stopAllProcedures();

        Q_INVOKABLE
        QString duplicate(QVariant procedure);Q_INVOKABLE
        QString exportData(QString filename, QVariant procedures = QVariant()); Q_INVOKABLE
        QString importDataFromFile(QString filename);Q_INVOKABLE
        QString importDataFromString(QString rawData);

        Q_INVOKABLE
        bool filter(QString content, QString filter);

        Q_INVOKABLE
        void processEventLoop();

        Q_INVOKABLE
        void populateProcedures(bb::system::SystemListDialog* dialog);Q_INVOKABLE
        QObject* getProcedureFromDialogResult(bb::system::SystemListDialog* dialog);Q_INVOKABLE
        QString serializeProcedure(QObject* procedure);

        void emitIdentityResult(QVariantMap result);

    public slots:
        void onInvoke(const bb::system::InvokeRequest& request);
        void displayUserMessagePropagate(QString msg);
        void postNotificationPropagate(QVariantMap data);
        void headlessProcedureRunning(int);

    Q_SIGNALS:
        void displayUserMessage(QString msg);
        void postNotification(QVariantMap data);
        // Contains 'success' (bool), 'error' (error message on failure), and 'profile' (profile on success)
        void identityResult(QVariantMap identity);

    private:
        void loadProcedures();
        void loadServices();
        void loadPermissions();
        void loadSettings();
        void loadPersistentVariables();
        void loadStatementInfoList();
        void populateOverviewList(RegexDataModel* dataModel);
        void populateLibraryList(RegexDataModel* dataModel);
        void populatePermissionsList(RegexDataModel* dataModel);
        void autostart();
        void setPremiumSupport();
        void unsetPremiumSupport();

        bool loaded;
        RegexDataModel* dataModelLibraryList;
        RegexDataModel* dataModelPermissionsList;
        RegexDataModel* dataModelGroupList;
        RegexDataModel* dataModelSearchList;
        std::vector<Procedure*>* procedures;
        std::vector<Permission*>* permissions;
        SettingsModel* settings;
        bool detectedCrash;
        bool isPremium;
        int appOpenCount;
        QDateTime trialStart;
        QSettings qsettings;
        static Logger* LOG;

        bool areProceduresLoaded;
        std::vector<int> queuedHeadlessRunning;
        std::vector<std::string> queuedAutoStarts;
        static ids_provider_t* ids_provider;
        static int identityBpsIoHandler(int fd, int event, void* data);
        static void identitySuccessHandler(ids_request_id_t request_id, int property_count,
                const ids_property_t* properties, void* cb_data);
        static void identityFailureHandler(ids_request_id_t request_id, ids_result_t result,
                const char* info, void* cb_data);
    };

} /* namespace actiontrigger */
#endif /* ACTIONTRIGGER_HPP_ */
