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

#include "actiontrigger/view/cascades/toolscascades.hpp"

namespace actiontrigger
{

    Logger* ToolsCascades::LOG = LoggerFactory::getInstance("ToolsCascades");

    ToolsCascades::ToolsCascades()
    {
    }

    ToolsCascades::~ToolsCascades()
    {
    }

    void ToolsCascades::populateDataModel(bb::cascades::GroupDataModel* dataModel,
            std::vector<QObject*>* objects)
    {
        LOG->trace("populateDataModel");
        for (std::vector<QObject*>::iterator it = objects->begin(); it != objects->end(); ++it) {
            if (*it) {
                if (dataModel) {
                    dataModel->insert(*it);
                } else {
                    LOG->critical("NULL pointer listViewTriggers");
                }
            } else {
                LOG->critical("NULL pointer it");
            }
        }
        LOG->trace("Done inserting");
    }

} /* namespace actiontrigger */
