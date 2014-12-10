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

#ifndef ApplicationUI_HPP_
#define ApplicationUI_HPP_

#include <QObject>
#include "qmlbeam.hpp"

#include <QBool>
#include "mylistmodel.hpp"
#include <bb/system/InvokeManager>

#include "applicationwrapper.hpp"
#include <bb/cascades/QmlDocument>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/NavigationPane>

#include <iostream.h>

#include "actiontrigger/view/cascades/procedurecascadesview.hpp"
#include "actiontrigger/model/logger/logger.hpp"
#include "actiontrigger/actiontrigger.hpp"
#include <vector>

#include "actiontrigger/tools/unitofwork.hpp"
#include "actiontrigger/tools/work.hpp"
#include "asyncappload.hpp"

#include <bb/cascades/GroupDataModel>
#include "actiontrigger/view/cascades/regexdatamodel.hpp"
#include "actiontrigger/view/cascades/slidingdatamodel.hpp"
#include <bb/cascades/ListView>
#include <bb/cascades/TranslateTransition>

#include "qt4/QtDeclarative/qdeclarative.h"

#include <bb/cascades/Container>
#include <bb/cascades/SceneCover>

#include <bb/system/CardDoneMessage>

#include "actiontrigger/view/cascades/statementcascadesview.hpp"
#include "actiontrigger/view/cascades/servicecascadesview.hpp"
#include "actiontrigger/view/cascades/procedurecascadesview.hpp"
#include "actiontrigger/view/cascades/parametercascadesview.hpp"
#include "actiontrigger/view/cascades/statementinfocascadesview.hpp"
#include "actiontrigger/view/cascades/permissioncascadesview.hpp"
#include "actiontrigger/view/cascades/branchinfocascadesview.hpp"

#include <bb/platform/Notification>

namespace bb
{
    namespace cascades
    {
        class Application;
    }
}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class ApplicationUI: public QObject
{
    Q_OBJECT
public Q_SLOTS:
    void onExit();
public:
    ApplicationUI(bb::cascades::Application* app);
    virtual ~ApplicationUI()
    {
    }
    Q_INVOKABLE
    void asyncLoad(bool resetApplication, bool preventAutoStart,
            bb::cascades::ArrayDataModel* splashLogDataModel,
            bb::cascades::DataModel* dataModelLibraryList,
            bb::cascades::TranslateTransition* endAnimation);
    void onSystemLanguageChanged();
    void resendNotification();
private:
    bb::cascades::Application* app;
    actiontrigger::ActionTrigger* actiontrigger;
    bb::platform::Notification notification;
};

#endif /* ApplicationUI_HPP_ */
