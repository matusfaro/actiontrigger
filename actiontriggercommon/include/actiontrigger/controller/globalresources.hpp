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

#ifndef GLOBALRESOURCES_HPP_
#define GLOBALRESOURCES_HPP_

#include <QObject>
#include <QString>
#include <QVariant>
#include <QMutex>
#include <errno.h>
#include <btapi/btdevice.h>
#include <vector>
#include <map>
#include "actiontrigger/model/loggerfactory.hpp"

namespace actiontrigger
{

    class Procedure;

    class GlobalResources: public QObject
    {
    Q_OBJECT

    Q_PROPERTY (QString coverTitle READ getCoverTitle NOTIFY coverTitleChangeRequest FINAL)
    Q_PROPERTY (QString coverContent READ getCoverContent NOTIFY coverContentChangeRequest FINAL)
    Q_PROPERTY (QString coverImage READ getCoverImage NOTIFY coverImageChangeRequest FINAL)

    public:
        static GlobalResources* get();

        // Properties
        std::string getUserProperty(std::string property, std::string defaultValue = "");
        void setUserProperty(std::string property, std::string value);
        // Active frame cover
        void resetCover();
        QString getCoverTitle() const;
        void setCoverTitle(std::string value);
        QString getCoverContent() const;
        void setCoverContent(std::string value);
        QString getCoverImage() const;
        void setCoverImage(std::string value);
        // Bluetooth
        int setupBluetooth();
        // Procedures
        void setProcedures(std::vector<Procedure*>* procedures);
        std::vector<Procedure*>* getProcedures() const;
        // Clipboard
        void clipboardStore(QVariant data, bool isTrigger);
        QVariant clipboardRetrieve();Q_INVOKABLE
        bool clipboardHasStatement();Q_INVOKABLE
        bool clipboardHasTrigger();
        // User facing message
        void sendUserFacingMessage(QString msg);
        void requestNotification(QVariantMap msg);
        // Headless Connector
        Q_INVOKABLE
        QObject* getHeadlessConnector();
        // Logging
        Q_INVOKABLE
        void setLogLevel(QString newLevel);Q_INVOKABLE
        void setDebugDataModel(bb::cascades::ArrayDataModel* dataModel);

    Q_SIGNALS:
        void displayUserMessage(QString msg);
        void postNotification(QVariantMap msg);
        void coverTitleChangeRequest();
        void coverContentChangeRequest();
        void coverImageChangeRequest();
        void clipboardChanged();

    private:
        GlobalResources();
        virtual ~GlobalResources();
        static GlobalResources* instance;
        static Logger* LOG;

        // Properties
        static QMutex mutexUserProperties;
        std::map<std::string, std::string>* userProperties;
        // Active frame cover
        std::string title;
        std::string content;
        std::string image;
        // Bluetooth
        static QMutex mutexBluetoothGlobalInit;
        static bool bluetoothCreated;
        // Procedures
        std::vector<Procedure*>* procedures;
        // Clipboard
        bool clipboardIsTrigger;
        QVariant clipboardData;
    };

} /* namespace actiontrigger */
#endif /* GLOBALRESOURCES_HPP_ */
