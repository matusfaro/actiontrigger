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

#ifndef EXECUTIONRESOURCES_HPP_
#define EXECUTIONRESOURCES_HPP_

#include <QObject>
#include <sys/neutrino.h>
#include "bps/bps.h"
#include "actiontrigger/model/loggerfactory.hpp"
#include "actiontrigger/model/logger/blackberry/datamodellogger.hpp"
#include <errno.h>
#include <unistd.h>
#include <QWaitCondition>
#include <QMutex>
#include <camera/camera_api.h>
#include "actiontrigger/controller/services/hub/messageservice.hpp"
#include "actiontrigger/controller/services/phone/phoneservice.hpp"
#include <bb/multimedia/AudioRecorder>
#include <bb/multimedia/MediaPlayer>

#define BLUETOOTH_INIT_TIMEOUT 3000

namespace actiontrigger
{

    class ExecutionResources
    {
    public:
        ExecutionResources(DataModelLogger* USERLOG);
        virtual ~ExecutionResources();
        // Attempts to notify blocking calls of procedure stop
        void notifyStop();
        // Message Channel
        int getChannel();
        void destroyChannel();
        int getChannelConnection();
        void destroyChannelConnection();
        // BPS
        void bpsInitialize();
        bps_event_t* bpsGetEvent(int domain, unsigned int eventCode = 0x0, int timeout = 1000);
        void destroyBps();
        // Sleep
        bool sleepSafe(unsigned long ms = ULONG_MAX);
        // Camera
        camera_error_t setupCamera(camera_unit_t unit = CAMERA_UNIT_REAR,
                uint32_t mode = CAMERA_MODE_PREAD | CAMERA_MODE_PWRITE);
        std::string uniqueFilename(std::string filename); // Append time to filename
        camera_handle_t getCamera();
        bool isCameraOk(camera_error_t error);
        void destroyCamera();
        // MessageService
        MessageService* getMessageService(bb::pim::account::AccountKey accountKey);
        void destroyMessageService();
        // Phone Service
        PhoneService* getPhoneService();
        void destroyPhoneService();
        // Audio recorder
        bb::multimedia::AudioRecorder* getAudioRecorder();
        void destroyAudioRecorder();
        // Media Player
        bb::multimedia::MediaPlayer* getMediaPlayer();
        void destroyMediaPlayer();
        // Callback
        void callbackReset();
        bool callbackWait(unsigned long ms = ULONG_MAX);
        void callbackFinished();
    private:
        static Logger* LOG;
        DataModelLogger* USERLOG;
        // Message Channel
        bool channelCreated;
        int chid;
        bool channelConnectionCreated;
        int coid;
        // BPS
        bool bpsCreated;
        // Sleep
        QWaitCondition sleep;
        QMutex mutexSleep;
        // Camera
        bool cameraCreated;
        camera_handle_t camera;
        // MessageService
        std::map<bb::pim::account::AccountKey, MessageService*>* messageServiceMap;
        // PhoneService
        bool phoneServiceCreated;
        PhoneService* phoneService;
        // Audio recorder
        bool recorderCreated;
        bb::multimedia::AudioRecorder* recorder;
        // Media Player
        bool playerCreated;
        bb::multimedia::MediaPlayer* player;
        // Callback wait
        QWaitCondition callbackWaitCondition;
        volatile bool callbackCalled;
        QMutex mutexCallbackWait;
    };

} /* namespace actiontrigger */
#endif /* EXECUTIONRESOURCES_HPP_ */
