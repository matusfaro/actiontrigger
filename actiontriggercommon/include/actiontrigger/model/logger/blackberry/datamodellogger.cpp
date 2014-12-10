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

#include "actiontrigger/model/logger/blackberry/datamodellogger.hpp"

namespace actiontrigger
{

    DataModelLogger::DataModelLogger(ConnectExecutionModel* dataModel, LogLevel loglevel) :
            dataModel(dataModel), loglevel(loglevel)
    {
        this->layers = new std::vector<std::string>();
    }

    DataModelLogger::~DataModelLogger()
    {
        delete this->dataModel;
    }

    void DataModelLogger::raw(std::string msg)
    {
        this->dataModel->log(msg);
    }

    void DataModelLogger::trace(std::string msg)
    {
        if (loglevel >= TRACE) {
            this->dataModel->log(
                    RichText::html(
                            getTimestamp() + " " + RichText::color("lightblue", "TRACE: ")
                                    + getLayers() + msg));
        }
    }

    void DataModelLogger::debug(std::string msg)
    {
        if (loglevel >= DEBUG) {
            this->dataModel->log(
                    RichText::html(
                            getTimestamp() + " " + RichText::color("aqua", "DEBUG: ") + getLayers()
                                    + msg));
        }
    }

    void DataModelLogger::warning(std::string msg)
    {
        if (loglevel >= WARNING) {
            this->dataModel->log(
                    RichText::html(
                            getTimestamp() + " " + RichText::color("gold", "WARN: ") + getLayers()
                                    + msg));
        }
    }

    void DataModelLogger::error(std::string msg)
    {
        if (loglevel >= ERROR) {
            this->dataModel->log(
                    RichText::html(
                            getTimestamp() + " " + RichText::color("red", "ERROR: ") + getLayers()
                                    + msg));
        }
    }

    void DataModelLogger::critical(std::string msg)
    {
        if (loglevel >= CRITICAL) {
            this->dataModel->log(
                    RichText::html(
                            getTimestamp() + " "
                                    + RichText::highlight("red",
                                            RichText::color("black", "CRITICAL: ")) + getLayers()
                                    + msg));
        }
    }

    void DataModelLogger::info(std::string msg)
    {
        this->dataModel->log(
                RichText::html(
                        getTimestamp() + " " + RichText::color("green", "INFO: ") + getLayers()
                                + msg));
    }

    void DataModelLogger::addVariable(std::string key, std::string value)
    {
        this->dataModel->variable(key, value);
    }

    ConnectExecutionModel* DataModelLogger::getDataModel()
    {
        return this->dataModel;
    }

    LogLevel DataModelLogger::getLoglevel()
    {
        return loglevel;
    }

    void DataModelLogger::setLoglevel(LogLevel loglevel)
    {
        this->loglevel = loglevel;
    }

    void DataModelLogger::pushLayer(std::string layer)
    {
        this->layers->push_back(layer);
    }

    void DataModelLogger::popLayer()
    {
        if (!this->layers->empty())
            this->layers->pop_back();
    }

    void DataModelLogger::popAllLayers()
    {
        this->layers->resize(0);
    }

    std::size_t DataModelLogger::layerCount()
    {
        return this->layers->size();
    }

    void DataModelLogger::shrinkLayers(std::size_t size)
    {
        if (this->layers->size() < size) {
            this->layers->resize(size);
        }
    }

    std::string DataModelLogger::getLayers()
    {
        std::string layersString = "";
        for (std::vector<std::string>::iterator it = this->layers->begin();
                it != this->layers->end(); ++it) {
            layersString += (*it) + ": ";
        }
        return layersString;
    }

} /* namespace actiontrigger */
