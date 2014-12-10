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

#include "actiontrigger/controller/executionresources.hpp"

namespace actiontrigger
{

    Logger* ExecutionResources::LOG = LoggerFactory::getInstance("ExecutionResources");

    ExecutionResources::ExecutionResources(DataModelLogger* USERLOG) :
            USERLOG(USERLOG), channelCreated(false), chid(-1), channelConnectionCreated(false), coid(
                    -1), bpsCreated(false), cameraCreated(false), phoneServiceCreated(false), phoneService(
                    NULL), recorderCreated(false), playerCreated(false), player(NULL), callbackCalled(
                    false)
    {
        messageServiceMap = new std::map<bb::pim::account::AccountKey, MessageService*>();
    }

    ExecutionResources::~ExecutionResources()
    {
        LOG->trace("~ExecutionResources");
        destroyChannel();
        destroyBps();
        destroyCamera();
        destroyMessageService();
        destroyPhoneService();
        destroyAudioRecorder();
        destroyMediaPlayer();
    }

    int ExecutionResources::getChannel()
    {
        if (!channelCreated) {
            this->chid = ChannelCreate(0);
            channelCreated = true;
        }
        return this->chid;
    }

    void ExecutionResources::destroyChannel()
    {
        if (channelCreated) {
            this->destroyChannelConnection();
            ChannelDestroy(this->chid);
            channelCreated = false;
        }
    }

    int ExecutionResources::getChannelConnection()
    {
        if (!channelConnectionCreated) {
            this->coid = ConnectAttach(0, 0, this->getChannel(), _NTO_SIDE_CHANNEL, 0);
            channelConnectionCreated = true;
        }
        return this->coid;
    }

    void ExecutionResources::destroyChannelConnection()
    {
        if (channelConnectionCreated) {
            ConnectDetach(this->coid);
            channelConnectionCreated = false;
        }
    }

    void ExecutionResources::bpsInitialize()
    {
        errno = EOK;
        if (!bpsCreated) {
            if (BPS_SUCCESS != bps_initialize()) {
                LOG->warning(SSTR("Error starting BPS: " << strerror(errno)));
                USERLOG->error(SSTR("Error starting BPS: " << strerror(errno)));
                return;
            }
            this->bpsCreated = true;
        }
    }

    bps_event_t* ExecutionResources::bpsGetEvent(int domain, unsigned int eventCode, int timeout)
    {
        bps_event_t *event;
        errno = EOK;
        for (int n = 0; n < 100; n++) {
            event = NULL;
            if (BPS_SUCCESS != bps_get_event(&event, timeout)) {
                LOG->warning(SSTR("Unable to retrieve event: " << strerror(errno)));
                USERLOG->error(SSTR("Unable to retrieve event: " << strerror(errno)));
                return NULL;
            }
            if (event) {
                if (bps_event_get_domain(event) == domain) {
                    if (eventCode == 0x0 || bps_event_get_code(event) == eventCode) {
                        return event;
                    } else {
                        LOG->warning(
                                SSTR(
                                        "Received event with unknown code: "
                                                << bps_event_get_code(event)));
                    }
                } else {
                    LOG->warning(
                            SSTR(
                                    "Received event with unknown domain id: "
                                            << bps_event_get_domain(event)));
                }
            } else {
                LOG->warning(SSTR("Event is NULL, timeout has been reached"));
            }
        }
        USERLOG->error(SSTR("Did not receive value, timeout has been reached"));
        return NULL;
    }

    void ExecutionResources::destroyBps()
    {
        if (bpsCreated) {
            bps_shutdown();
            bpsCreated = false;
        }
    }

    bool ExecutionResources::sleepSafe(unsigned long ms)
    {
        mutexSleep.lock();
        bool forcedWakeUp = this->sleep.wait(&mutexSleep, ms);
        mutexSleep.unlock();
        return forcedWakeUp;
    }

    void ExecutionResources::notifyStop()
    {
        // Sleep
        this->sleep.wakeAll();
    }

    camera_error_t ExecutionResources::setupCamera(camera_unit_t unit, uint32_t mode)
    {
        if (!cameraCreated) {
            camera_error_t status = camera_open(unit, mode, &this->camera);
            if (status == CAMERA_EOK) {
                cameraCreated = true;
            }
            return status;
        }
        return CAMERA_EDESKTOPCAMERAINUSE;
    }

    std::string ExecutionResources::uniqueFilename(std::string filename)
    {
        size_t dotPos = filename.find_last_of('.');
        std::string timestamp =
                QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss.zzz").toStdString();
        if (dotPos == std::string::npos) {
            // No dot character found, append time at end
            return SSTR(filename << '.' << timestamp);
        } else {
            // Dot character found, put time before the extension
            return SSTR(filename.substr(0, dotPos) << '.' << timestamp << filename.substr(dotPos));
        }
    }

    bool ExecutionResources::isCameraOk(camera_error_t error)
    {
        switch (error) {
            case CAMERA_EOK:
                return true;
            case CAMERA_EAGAIN:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the specified camera was not available");
                break;
            case CAMERA_EINVAL:
                USERLOG->error(
                        "Error Opening camera: The function call failed because of an invalid argument");
                break;
            case CAMERA_ENODEV:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the specified camera was not found");
                break;
            case CAMERA_EMFILE:
                USERLOG->error(
                        "Error Opening camera: The function call failed because of a file table overflow");
                break;
            case CAMERA_EBADF:
                USERLOG->error(
                        "Error Opening camera: The function call failed because an invalid handle was used");
                break;
            case CAMERA_EACCESS:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the necessary permissions to access the camera are not available");
                break;
            case CAMERA_EBADR:
                USERLOG->error(
                        "Error Opening camera: The function call failed because an invalid file descriptor was used");
                break;
            case CAMERA_ENODATA:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the requested data does not exist");
                break;
            case CAMERA_ENOENT:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the specified file or directory does not exist");
                break;
            case CAMERA_ENOMEM:
                USERLOG->error(
                        "Error Opening camera: The function call failed because memory allocation failed");
                break;
            case CAMERA_EOPNOTSUPP:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the requested operation is not supported");
                break;
            case CAMERA_ETIMEDOUT:
                USERLOG->error(
                        "Error Opening camera: The function call failed due to communication problem or time-out with the camera");
                break;
            case CAMERA_EALREADY:
                USERLOG->error(
                        "Error Opening camera: The function call failed because an operation on the camera is already in progress");
                break;
            case CAMERA_EBUSY:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the camera is busy");
                break;
            case CAMERA_ENOSPC:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the disk is full");
                break;
            case CAMERA_EUNINIT:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the Camera library has not been initialized");
                break;
            case CAMERA_EREGFAULT:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the registration of a callback failed");
                break;
            case CAMERA_EMICINUSE:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the microphone is already in use");
                break;
            case CAMERA_EDESKTOPCAMERAINUSE:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the operation cannot be completed while the camera is in use");
                break;
            case CAMERA_EPOWERDOWN:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the camera is in the power down state");
                break;
            case CAMERA_3ALOCKED:
                USERLOG->error(
                        "Error Opening camera: The function call failed because the 3A have been locked");
                break;
        }
        return false;
    }

    camera_handle_t ExecutionResources::getCamera()
    {
        return cameraCreated ? this->camera : NULL;
    }

    void ExecutionResources::destroyCamera()
    {
        if (cameraCreated) {
            camera_close(this->camera);
            cameraCreated = false;
        }
    }

    MessageService* ExecutionResources::getMessageService(bb::pim::account::AccountKey accountKey)
    {
        if (messageServiceMap->find(accountKey) == messageServiceMap->end()) {
            messageServiceMap->insert(
                    std::pair<bb::pim::account::AccountKey, MessageService*>(accountKey,
                            new MessageService(accountKey)));
        }
        return messageServiceMap->find(accountKey)->second;
    }

    PhoneService* ExecutionResources::getPhoneService()
    {
        if (!phoneService) {
            phoneService = new PhoneService();
            phoneServiceCreated = true;
        }
        return phoneService;
    }

    void ExecutionResources::destroyMessageService()
    {
        for (std::map<bb::pim::account::AccountKey, MessageService*>::iterator it =
                messageServiceMap->begin(); it != messageServiceMap->end(); ++it) {
            delete it->second;
        }
        messageServiceMap->clear();
    }

    void ExecutionResources::destroyPhoneService()
    {
        if (phoneServiceCreated) {
            delete phoneService;
            phoneServiceCreated = false;
        }
    }

    bb::multimedia::AudioRecorder* ExecutionResources::getAudioRecorder()
    {
        if (!recorderCreated) {
            recorder = new bb::multimedia::AudioRecorder(0);
            recorderCreated = true;
        }
        return recorder;
    }

    void ExecutionResources::destroyAudioRecorder()
    {
        if (recorderCreated) {
            recorder->reset();
            recorder->deleteLater();
            recorderCreated = false;
        }
    }

    bb::multimedia::MediaPlayer* ExecutionResources::getMediaPlayer()
    {
        if (!playerCreated) {
            player = new bb::multimedia::MediaPlayer(0);
            playerCreated = true;
        }
        return player;
    }

    void ExecutionResources::destroyMediaPlayer()
    {
        if (playerCreated) {
            player->stop();
            player->deleteLater();
            playerCreated = false;
        }
    }

    void ExecutionResources::callbackReset()
    {
        this->callbackCalled = false;
    }

    bool ExecutionResources::callbackWait(unsigned long ms)
    {
        if (this->callbackCalled) {
            this->callbackCalled = false;
            return false;
        }
        mutexCallbackWait.lock();
        bool forcedWakeUp = this->callbackWaitCondition.wait(&mutexCallbackWait, ms);
        mutexCallbackWait.unlock();
        if (this->callbackCalled) {
            this->callbackCalled = false;
            return false;
        }
        return forcedWakeUp;
    }

    void ExecutionResources::callbackFinished()
    {
        // Sleep
        this->callbackCalled = true;
        this->callbackWaitCondition.wakeAll();
    }

} /* namespace actiontrigger */
