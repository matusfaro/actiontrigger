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

#ifndef EXCEPTION_HPP_
#define EXCEPTION_HPP_

namespace actiontrigger
{

    class NotSetException: public std::runtime_error
    {
    public:
        NotSetException() :
                std::runtime_error("Item is not set")
        {
        }
    };

    class NotExistException: public std::runtime_error
    {
    public:
        NotExistException() :
                std::runtime_error("Item does not exist")
        {
        }
    };

    class InvalidParameterException: public std::runtime_error
    {
    public:
        InvalidParameterException(std::string msg) :
                std::runtime_error(msg)
        {
        }
    };

    class ExecutionAbortedException: public std::runtime_error
    {
    public:
        ExecutionAbortedException() :
                std::runtime_error("Execution aborted")
        {
        }
    };

    class LoopBreakException: public std::runtime_error
    {
    public:
        LoopBreakException() :
                std::runtime_error("Loop break")
        {
        }
    };

    class LoopContinueException: public std::runtime_error
    {
    public:
        LoopContinueException() :
                std::runtime_error("Loop continue")
        {
        }
    };

} /* namespace actiontrigger */
#endif /* EXCEPTION_HPP_ */
