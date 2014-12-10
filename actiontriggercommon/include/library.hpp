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

#ifndef Library_HPP_
#define Library_HPP_

#include "actiontriggercommon_global.hpp"

#include <bb/cascades/CustomControl>

namespace bb
{
    namespace cascades
    {
        class Container;
    }
}

/*!
 * @brief Library custom control
 */
class ACTIONTRIGGERCOMMON_EXPORT Library : public bb::cascades::CustomControl
{
    Q_OBJECT
public:
    explicit Library(bb::cascades::Container *parent = 0);
    virtual ~Library();
    void test();
};

#endif /* Library_HPP_ */
