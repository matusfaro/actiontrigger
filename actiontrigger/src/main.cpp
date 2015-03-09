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

#include "applicationui.hpp"

#include "applicationwrapper.hpp"
#include <QLocale>
#include <QTranslator>
#include <QtNetwork/QAbstractSocket>

// include JS Debugger / CS Profiler enabler
// this feature is enabled by default in the debug build only
#include <Qt/qdeclarativedebug.h>

// Declare types
#include <QMetaType>
#include <QtNetwork/qabstractsocket.h>
Q_DECLARE_METATYPE(QAbstractSocket::SocketState)
Q_DECLARE_METATYPE(QAbstractSocket::SocketError)
#include <string>
Q_DECLARE_METATYPE(std::string)

using namespace bb::cascades;

Q_DECL_EXPORT int main(int argc, char **argv)
{
    // this is where the server is started etc
//    actiontrigger::ApplicationWrapper* app = (bb::cascades::Application*) new actiontrigger::ApplicationWrapper(argc, argv);
    bb::cascades::Application* app = new bb::cascades::Application(argc, argv);

    //Register all necessary types
    qmlRegisterType<actiontrigger::ActionTrigger>("ActionTrigger", 1, 0, "ActionTrigger");
    qmlRegisterType<QObject>("ActionTrigger", 1, 0, "Object");
    qmlRegisterType<actiontrigger::StatementCascadesView>("ActionTrigger", 1, 0, "StatementView");
    qmlRegisterType<actiontrigger::BranchInfoCascadesView>("ActionTrigger", 1, 0, "Branch");
    qmlRegisterType<actiontrigger::ServiceCascadesView>("ActionTrigger", 1, 0, "Service");
    qmlRegisterType<actiontrigger::ProcedureCascadesView>("ActionTrigger", 1, 0, "Procedure");
    qmlRegisterType<actiontrigger::ParameterCascadesView>("ActionTrigger", 1, 0, "Parameter");
    qmlRegisterType<actiontrigger::StatementInfoCascadesView>("ActionTrigger", 1, 0,
            "StatementInfo");
    qmlRegisterType<actiontrigger::PermissionCascadesView>("ActionTrigger", 1, 0, "Permission");
    qmlRegisterType<actiontrigger::RegexDataModel>("ActionTrigger", 1, 0, "RegexDataModel");
    qmlRegisterType<actiontrigger::SlidingDataModel>("ActionTrigger", 1, 0, "SlidingDataModel");
    qRegisterMetaType<bb::system::CardDoneMessage>("CardDoneMessage");
    qRegisterMetaType<std::string>();
    qRegisterMetaType<QAbstractSocket::SocketState>();
    qRegisterMetaType<QAbstractSocket::SocketError>();

    // localization support
    QTranslator translator;
    QString locale_string = QLocale().name();
    QString filename = QString("ActionTrigger_%1").arg(locale_string);
    if (translator.load(filename, "app/native/qm")) {
        app->installTranslator(&translator);
    }

    // create the application pane object to init UI etc.
    new ApplicationUI(app);

    // we complete the transaction started in the app constructor and start the client event loop here
    return Application::exec();
    // when loop is exited the Application deletes the scene which deletes all its children (per qt rules for children)
}
