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

#ifndef BLACKBERRYFILESYSTEM_HPP_
#define BLACKBERRYFILESYSTEM_HPP_

#include <string>
#include "actiontrigger/model/loggerfactory.hpp"
#include <qt4/QtCore/QString>
#include <qt4/QtCore/QDir>

namespace actiontrigger
{

    class BlackberryFilesystem
    {
    public:
        /*
         * Checks if file exists, if it doesn't it loads it from the asset folder. Returns path to the file.
         */
        static std::string loadFile(std::string filename, bool forceReload = false);
    private:
        BlackberryFilesystem();
        virtual ~BlackberryFilesystem();
        static Logger* LOG;
    };

} /* namespace actiontrigger */
#endif /* BLACKBERRYFILESYSTEM_HPP_ */
