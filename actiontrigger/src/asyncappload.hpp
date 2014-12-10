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

#ifndef ASYNCAPPLOAD_HPP_
#define ASYNCAPPLOAD_HPP_

#include <actiontrigger/tools/unitofwork.hpp>
#include <actiontrigger/actiontrigger.hpp>
#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/GroupDataModel>
#include <actiontrigger/view/cascades/regexdatamodel.hpp>

namespace actiontrigger
{

    class AsyncAppLoad: public UnitOfWork
    {
    public:
        AsyncAppLoad(ActionTrigger* actiontrigger, bool resetApplication, bool preventAutoStart,
                bb::cascades::ArrayDataModel* splashLogDataModel,
                RegexDataModel* dataModelLibraryList);
        virtual ~AsyncAppLoad();
        void performWork();
    private:
        ActionTrigger* actiontrigger;
        bool resetApplication;
        bool preventAutoStart;
        bb::cascades::ArrayDataModel* splashLogDataModel;
        RegexDataModel* dataModelLibraryList;
    };

} /* namespace actiontrigger */
#endif /* ASYNCAPPLOAD_HPP_ */
