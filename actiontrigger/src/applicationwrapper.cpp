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

#include "applicationwrapper.hpp"

namespace actiontrigger
{

    Logger* ApplicationWrapper::LOG = LoggerFactory::getInstance("ApplicationWrapper");

    ApplicationWrapper::ApplicationWrapper(int& argc, char** argv) :
            Application(argc, argv)
    {
    }

    bool ApplicationWrapper::notify(QObject* receiver, QEvent* event)
    {
        try {
            return bb::cascades::Application::notify(receiver, event);
        } catch (const std::exception& e) {
            LOG->critical(SSTR("Caught exception: " << e.what()));
        } catch (...) {
            LOG->critical("Caught unidentified object");
        }
        return true;
    }

} /* namespace actiontrigger */
