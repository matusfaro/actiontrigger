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

#include "actiontrigger/controller/globalresources.hpp"
#include "actiontrigger/view/cascades/statementcascadesview.hpp"
#include "actiontrigger/controller/headlessconnector.hpp"

namespace actiontrigger
{

    GlobalResources* GlobalResources::instance = NULL;
    Logger* GlobalResources::LOG = LoggerFactory::getInstance("GlobalResources");

// Bluetooth synchronization
    QMutex GlobalResources::mutexBluetoothGlobalInit;
    bool GlobalResources::bluetoothCreated = false;

    GlobalResources* GlobalResources::get()
    {
        if (!GlobalResources::instance) {
            GlobalResources::instance = new GlobalResources();
        }
        return GlobalResources::instance;
    }

    GlobalResources::GlobalResources() :
            clipboardIsTrigger(false)
    {
        resetCover();
        this->procedures = NULL;
        this->userProperties = new std::map<std::string, std::string>();
    }

    GlobalResources::~GlobalResources()
    {
    }

    void GlobalResources::resetCover()
    {
        this->title = "ActionTrigger";
        this->content = "";
        this->image = "";
        emit coverTitleChangeRequest();
        emit coverContentChangeRequest();
        emit coverImageChangeRequest();
    }

    QString GlobalResources::getCoverTitle() const
    {
        return QString::fromStdString(this->title);
    }

    void GlobalResources::setCoverTitle(std::string value)
    {
        this->title = value;
        emit coverTitleChangeRequest();
    }

    QString GlobalResources::getCoverContent() const
    {
        return QString::fromStdString(this->content);
    }

    void GlobalResources::setCoverContent(std::string value)
    {
        this->content = value;
        emit coverContentChangeRequest();
    }

    QString GlobalResources::getCoverImage() const
    {
        return QString::fromStdString(this->image);
    }

    void GlobalResources::setCoverImage(std::string value)
    {
        this->image = value;
        emit coverImageChangeRequest();
    }

    /*
     * Blackberry's API sends bluetooth events through a callback. Ignore all for now.
     */
    void bluetoothInitCallback(const int event, // the Bluetooth event
            const char *bt_addr, // the Bluetooth MAC address
            const char *event_data)
    { // the event data
        (void) event;
        (void) bt_addr;
        (void) event_data;
    }

    int GlobalResources::setupBluetooth()
    {
        mutexBluetoothGlobalInit.lock();
        if (bluetoothCreated) {
            mutexBluetoothGlobalInit.unlock();
            return 0;
        }
        errno = EOK;
        int status = bt_device_init(bluetoothInitCallback);
        mutexBluetoothGlobalInit.unlock();
        if (status != 0) {
            if (errno == EINVAL) {
                // Bluetooth already initialized, everything is ok
                return 0;
            }
            // Error occured
            return errno;
        }
        return 0; // Bluetooth initiliazed correctly
    }

    void GlobalResources::setProcedures(std::vector<Procedure*>* procedures)
    {
        this->procedures = procedures;
    }

    std::vector<Procedure*>* GlobalResources::getProcedures() const
    {
        return this->procedures;
    }

    QMutex GlobalResources::mutexUserProperties;

    std::string GlobalResources::getUserProperty(std::string property, std::string defaultValue)
    {
        std::string value;
        mutexUserProperties.lock();
        if (this->userProperties->find(property) != this->userProperties->end()) {
            value = this->userProperties->operator[](property);
        } else {
            value = defaultValue;
        }
        mutexUserProperties.unlock();
        return value;
    }

    void GlobalResources::setUserProperty(std::string property, std::string value)
    {
        LOG->trace(SSTR("Set global property: " << property << " = \"" << value << "\""));
        mutexUserProperties.lock();
        this->userProperties->operator[](property) = value;
        mutexUserProperties.unlock();
    }

    void GlobalResources::clipboardStore(QVariant data, bool isTrigger)
    {
        this->clipboardIsTrigger = isTrigger;
        this->clipboardData = data;
        emit clipboardChanged();
    }

    QVariant GlobalResources::clipboardRetrieve()
    {
        return clipboardData;
    }

    bool GlobalResources::clipboardHasStatement()
    {
        return clipboardData.isValid() && !clipboardData.isNull();
    }
    bool GlobalResources::clipboardHasTrigger()
    {
        return clipboardIsTrigger;
    }

    void GlobalResources::sendUserFacingMessage(QString msg)
    {
        emit displayUserMessage(msg);
    }

    void GlobalResources::requestNotification(QVariantMap data)
    {
        emit postNotification(data);
    }

    QObject* GlobalResources::getHeadlessConnector()
    {
        return (QObject*) HeadlessConnector::get();
    }

    void GlobalResources::setLogLevel(QString newLevel)
    {
        BlackberryLogger::setLogLevel(Logger::parseLogLevel(newLevel.toStdString()));
    }

    void GlobalResources::setDebugDataModel(bb::cascades::ArrayDataModel* dataModel)
    {
        BlackberryLogger::setContainerAll(dataModel);
    }

} /* namespace actiontrigger */
