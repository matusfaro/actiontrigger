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

#ifndef DATAMODELLOGGER_HPP_
#define DATAMODELLOGGER_HPP_

#include "actiontrigger/model/logger/blackberry/connectexecutionmodel.hpp"
#include "actiontrigger/model/logger/logger.hpp"
#include <string>
#include "actiontrigger/view/cascades/richtext.hpp"
#include <vector>
#include <bb/cascades/ListView>

namespace actiontrigger
{

    class DataModelLogger: public Logger
    {
    public:
        DataModelLogger(ConnectExecutionModel* dataModel, LogLevel loglevel);
        virtual ~DataModelLogger();
        void raw(std::string msg);
        void trace(std::string msg);
        void debug(std::string msg);
        void warning(std::string msg);
        void error(std::string msg);
        void critical(std::string msg);
        void info(std::string msg);
        void addVariable(std::string key, std::string value);
        ConnectExecutionModel* getDataModel();
        LogLevel getLoglevel();
        void setLoglevel(LogLevel loglevel);
        void pushLayer(std::string layer);
        void popLayer();
        void popAllLayers();
        std::size_t layerCount();
        void shrinkLayers(std::size_t size);
    private:
        ConnectExecutionModel* dataModel;
        LogLevel loglevel;
        std::vector<std::string>* layers;
        std::string getLayers();
    };

} /* namespace actiontrigger */
#endif /* DATAMODELLOGGER_HPP_ */
