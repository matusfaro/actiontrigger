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

#include "actiontrigger/controller/services/media/volumemanager.hpp"

VolumeManager::VolumeManager()
{
    // TODO Auto-generated constructor stub

}

VolumeManager::~VolumeManager()
{
    // TODO Auto-generated destructor stub
}

audio_manager_device_t VolumeManager::stringToAudioDevice(std::string str)
{
    if (str == "AUDIO_DEVICE_SPEAKER") {
        return AUDIO_DEVICE_SPEAKER;
    } else if (str == "AUDIO_DEVICE_HEADSET") {
        return AUDIO_DEVICE_HEADSET;
    } else if (str == "AUDIO_DEVICE_HEADPHONE") {
        return AUDIO_DEVICE_HEADPHONE;
    } else if (str == "AUDIO_DEVICE_A2DP") {
        return AUDIO_DEVICE_A2DP;
    } else if (str == "AUDIO_DEVICE_HANDSET") {
        return AUDIO_DEVICE_HANDSET;
    } else if (str == "AUDIO_DEVICE_HAC") {
        return AUDIO_DEVICE_HAC;
    } else if (str == "AUDIO_DEVICE_BT_SCO") {
        return AUDIO_DEVICE_BT_SCO;
    } else if (str == "AUDIO_DEVICE_HDMI") {
        return AUDIO_DEVICE_HDMI;
    } else if (str == "AUDIO_DEVICE_TOSLINK") {
        return AUDIO_DEVICE_TOSLINK;
    } else if (str == "AUDIO_DEVICE_TTY") {
        return AUDIO_DEVICE_TTY;
    } else if (str == "AUDIO_DEVICE_LINEOUT") {
        return AUDIO_DEVICE_LINEOUT;
    } else if (str == "AUDIO_DEVICE_USB") {
        return AUDIO_DEVICE_USB;
    } else if (str == "AUDIO_DEVICE_TONES") {
        return AUDIO_DEVICE_TONES;
    } else if (str == "AUDIO_DEVICE_VOICE") {
        return AUDIO_DEVICE_VOICE;
    } else if (str == "AUDIO_DEVICE_WIFI_DISPLAY") {
        return AUDIO_DEVICE_WIFI_DISPLAY;
    } else if (str == "AUDIO_DEVICE_MIRRORLINK") {
        return AUDIO_DEVICE_MIRRORLINK;
    } else if (str == "AUDIO_DEVICE_AUDIO_SHARE") {
        return AUDIO_DEVICE_AUDIO_SHARE;
    } else if (str == "AUDIO_DEVICE_COUNT") {
        return AUDIO_DEVICE_COUNT;
    } else {
        return AUDIO_DEVICE_DEFAULT;
    }
}

