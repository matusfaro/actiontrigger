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

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application* app) :
        QObject(app), app(app)
{

    // Create the invocation manager and intentionally leak it.... (We don't want to destroy it)
    bb::system::InvokeManager* invokeManager = new bb::system::InvokeManager(0);

    // For future use, we may want to show different UI here if invoked instead of launched
    switch (invokeManager->startupMode()) {
        case bb::system::ApplicationStartupMode::LaunchApplication:
            // An application can initialize if it
            // was launched from the home screen
            qDebug("Init: home screen invoked startup");
            break;
        case bb::system::ApplicationStartupMode::InvokeApplication:
            // If the application is invoked,
            // it must wait until it receives an invoked(..) signal
            // so that it can determine the UI that it needs to initialize
            qDebug("Init: invocation startup");
            break;
        case bb::system::ApplicationStartupMode::InvokeViewer:
            qDebug("Init: InvokeViewer startup, we shouldn't be here");
            break;
        case bb::system::ApplicationStartupMode::InvokeCard:
            qDebug("Init: InvokeCard startup, we shouldn't be here");
            break;
        default:
            qDebug("Init: What app is it and how did it get here?");
            break;
    }

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *mainqml = QmlDocument::create("asset:///main.qml");
    mainqml->setParent(0);

    // Setup actiontrigger
    this->actiontrigger = new actiontrigger::ActionTrigger();

    // Register listener for invocations
    this->connect(invokeManager, SIGNAL(invoked(const bb::system::InvokeRequest&)), this->actiontrigger,
            SLOT(onInvoke(const bb::system::InvokeRequest&)));

    // Register fullscreen event to clear all notifications
    connect(app, SIGNAL(fullscreen()), &notification, SLOT(clearEffectsForAll()));

    // Setup exit strategy
    connect(app, SIGNAL(manualExit()), this, SLOT(onExit()));
    app->setAutoExit(false);

    mainqml->setContextProperty("_actiontrigger", actiontrigger);
    mainqml->setContextProperty("_app", this);

    // Setup active frame cover
    QmlDocument *coverqml = QmlDocument::create("asset:///ActiveCover.qml").parent(this);
    if (!coverqml->hasErrors()) {
        // Pass action trigger to the cover page for dynamic content
        coverqml->setContextProperty("_actiontrigger", actiontrigger);
        coverqml->setContextProperty("_app", this);
        // Create the QML Container from using the QMLDocument, create a SceneCover, and set the application cover.
        app->setCover(SceneCover::create().content(coverqml->createRootObject<Container>()));
    }

    // create root object for the UI
    TabbedPane *root = mainqml->createRootObject<TabbedPane>();

    // set created root object as a scene
    app->setScene(root);

    // Listen for qml changes
    new QmlBeam(this);
}

void ApplicationUI::asyncLoad(bool resetApplication, bool preventAutoStart,
        bb::cascades::ArrayDataModel* splashLogDataModel,
        bb::cascades::DataModel* dataModelLibraryList,
        bb::cascades::TranslateTransition* endAnimation)
{
    actiontrigger::UnitOfWork* unitwork =
            (actiontrigger::UnitOfWork*) new actiontrigger::AsyncAppLoad(this->actiontrigger,
                    resetApplication, preventAutoStart, splashLogDataModel,
                    static_cast<actiontrigger::RegexDataModel*>(dataModelLibraryList));
    actiontrigger::Work* asyncload = new actiontrigger::Work(unitwork);
    connect(asyncload, SIGNAL(finished()), endAnimation, SLOT(play()));
    asyncload->start();
}

void ApplicationUI::onExit()
{
    delete this->actiontrigger;
    app->quit();
}

void ApplicationUI::onSystemLanguageChanged()
{
    // TODO
}

void ApplicationUI::resendNotification()
{
    // TODO
}
