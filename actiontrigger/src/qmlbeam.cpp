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

#include "qmlbeam.hpp"

#include <QDeclarativeContext>

#include <bb/cascades/AbstractPane>
#include <bb/cascades/core/developmentsupport.h>
#include <bb/cascades/QmlDocument>

using namespace bb::cascades;

const QString QmlBeam::mDefaultMainFile("asset:///main.qml");

QmlBeam::QmlBeam(QObject *parent) :
        QObject(parent)
{
    bool connectResult;
    Q_UNUSED(connectResult);

    // Initiate the name of the application main QML file to the default value.
    mMainFile = mDefaultMainFile;

    // Get the application scene, its needed since the scene is recreated when
    // QML files are altered.
    mRoot = Application::instance()->scene();
    Q_ASSERT(mRoot);

    mRoot->setParent(this);

    // Create the Development support object and connect to its signal,
    // so that the application scene can be recreated as changes are made
    // to the QML files.
    DevelopmentSupport* devSupport = new DevelopmentSupport(this);
    connectResult = connect(devSupport, SIGNAL(assetsChanged(QUrl)), SLOT(reloadQML(QUrl)));
    Q_ASSERT(connectResult);

    connectResult = connect(Application::instance(), SIGNAL(aboutToQuit()), SLOT(cleanup()));

    Q_ASSERT(connectResult);
}

void QmlBeam::reloadQML(QUrl mainFile)
{
    // The mainFile parameter is ignored, instead a property is used to cover non-default asset names.
    Q_UNUSED(mainFile);

    // Get the context of the first scene root to keep the contextProperties.
    QDeclarativeContext* context = QDeclarativeEngine::contextForObject(mRoot);

    // Clear the QML cache.
    QDeclarativeEngine* appEngine = context->engine();
    appEngine->clearComponentCache();

    // Reload all QML.
    QmlDocument* qml = QmlDocument::create(mMainFile);
    AbstractPane *root = qml->createRootObject < AbstractPane > (context);
//    qml->setParent(root);
    Application::instance()->setScene(root);
}

void QmlBeam::cleanup()
{
    // Clean up.
//    Application::instance()->setScene(0);
//    mRoot->setParent(0);
//    delete mRoot;
}

void QmlBeam::setMainFile(const QString &mainFile)
{
    if (mainFile != mMainFile.toString()) {
        mMainFile = QUrl(mainFile);
        emit mainFileChanged(mainFile);
    }
}

QString QmlBeam::mainFile() const
{
    return mMainFile.toString();
}
