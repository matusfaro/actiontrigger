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

#ifndef actiontriggercommon_Global_HPP_
#define actiontriggercommon_Global_HPP_

#include <QtCore/qglobal.h>

#ifndef ACTIONTRIGGERCOMMON_STATIC_LINK
#ifdef ACTIONTRIGGERCOMMON_LIBRARY
#define ACTIONTRIGGERCOMMON_EXPORT Q_DECL_EXPORT
#else
#define ACTIONTRIGGERCOMMON_EXPORT Q_DECL_IMPORT
#endif
#else
#define ACTIONTRIGGERCOMMON_EXPORT
#endif /* ACTIONTRIGGERCOMMON_STATIC_LINK */

#endif /* actiontriggercommon_Global_HPP_ */
