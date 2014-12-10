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

#include "actiontrigger/model/filesystem/blackberry/blackberryfilesystem.hpp"

namespace actiontrigger
{

    Logger* BlackberryFilesystem::LOG = LoggerFactory::getInstance("BlackberryFilesystem");

    BlackberryFilesystem::BlackberryFilesystem()
    {
        // TODO Auto-generated constructor stub

    }

    BlackberryFilesystem::~BlackberryFilesystem()
    {
        // TODO Auto-generated destructor stub
    }

    std::string BlackberryFilesystem::loadFile(std::string filename, bool forceReload)
    {
        LOG->trace("prepareDatabaseFile");
        QString dataFolder = QDir::homePath();
        QString newFileName = dataFolder + "/" + QString::fromStdString(filename);
        QFile newFile(newFileName);
        LOG->trace("Loading file: " + filename);
        if (forceReload || !newFile.exists()) {
            if (newFile.exists()) {
                LOG->trace("Forced reload, replacing...");
                newFile.remove();
            }
            LOG->trace("Loading from assets...");
            QString appFolder(QDir::homePath());
            appFolder.chop(4);
            QString originalFileName = appFolder + "app/native/assets/"
                    + QString::fromStdString(filename);
            QFile originalFile(originalFileName);
            if (originalFile.exists()) {
                QFileInfo fileInfo(newFileName);
                QDir().mkpath(fileInfo.dir().path());

                if (!originalFile.copy(newFileName)) {
                    LOG->critical("failed to copy database file to path");
                }
            } else {
                LOG->critical("base database file does not exist");
            }
        }
        return std::string(newFileName.toUtf8().constData());
    }

} /* namespace actiontrigger */
