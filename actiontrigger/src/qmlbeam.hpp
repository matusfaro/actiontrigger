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

#ifndef QMLBEAM_H_
#define QMLBEAM_H_

#include <bb/cascades/Application>
#include <QObject>
#include <QUrl>

namespace bb
{
    namespace cascades
    {
        class AbstractPane;
    }
}

using namespace bb::cascades;

/**
 * QmlBeam Description
 *
 * Beam me up! An utility object that can be used to run the application
 * using the DevelopmentSupport signals to update the QML part of your app
 * on the fly. Every time you save a QML file the file is uploaded
 * to the device and the application scene is reloaded. Note that the
 * Preferences -> BlackBerry ->"Send QML files to device on save" option
 * in Momentics has to be enabled in order for this to work.
 */
class QmlBeam: public QObject
{
    Q_OBJECT

    /**
     * The application main QML File, the default value is set to "asset:///main.qml",
     * but if the application use a different name this property has to be set in order
     * for the correct file to be loaded when the reloadQML signal is triggered.
     */
    Q_PROPERTY( QString mainFile READ mainFile WRITE setMainFile NOTIFY mainFileChanged FINAL )

public:
    /**
     * Constructor, creates a QmlBeam object, which connects to the
     * DevelopmentSupport signals that trigger reloads of the UI.
     *
     * @param parent The parent object.
     */
    QmlBeam(QObject *parent = 0);

    /**
     * Returns the application main QML file.
     *
     * @return The asset name of the main QML file.
     */
    Q_SLOT
    QString mainFile() const;

    /**
     * Sets which QML file should be used to reload the UI when the reloadQML signal is triggered.
     * This has to be an assets path in the form of "asset:///myui.qml".
     *
     * @param mainFile The QML asset name of the main QML application file.
     */
    Q_SLOT
    void setMainFile(const QString &mainFile);

    signals:
    /**
     * Emitted when the application main QML file changes.
     *
     * @param mainFile The asset name of the main QML application file.
     */
    void mainFileChanged(const QString &mainFile);

private slots:
    /**
     * The reloadQML method is where the QML gets reloaded in the application.
     *
     * @param mainFile The default name of the application root QML file, not used in this implementation.
     */
    void reloadQML(QUrl mainFile);

    /**
     * The cleanup is where the saved application context is cleaned up.
     */
    void cleanup();

private:
    // The default name of the application main QML file.
    static const QString mDefaultMainFile;

    // Root pane of the application scene.
    bb::cascades::AbstractPane* mRoot;

    // The application main QML file.
    QUrl mMainFile;
};

#endif /* QMLBEAM_H_ */
