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

#ifndef STATEMENT_HPP_
#define STATEMENT_HPP_

#define DEFAULT_TRIGGER_GRANULARITY_MICROSEC 500000

#include <QObject>
#include <vector>
#include "actiontrigger/controller/services/parameterdefinition.hpp"
#include "actiontrigger/controller/common.hpp"
#include <string>
#include "actiontrigger/model/database/statementmodel.hpp"
#include "actiontrigger/model/loggerfactory.hpp"
#include <regex.h>
#include "actiontrigger/controller/executionstate.hpp"
#include <pthread.h>
#include "actiontrigger/model/databasefactory.hpp"
#include "actiontrigger/model/logger/blackberry/connectexecutionmodel.hpp"

namespace actiontrigger
{

// Maximum amount of time to wait for any event inside an action or trigger.
// This is required to make sure an actiontrigger does not hang forever.
#define MAX_WAIT_TIME 15 * 60 * 1000

// Variable syntax definition
#define VAR_BEGIN_CHAR '{'
#define VAR_END_CHAR '}'

    struct StatementInfo
    {
        StatementInfo() :
                isTrigger(false), name(""), description("")
        {
        }
        StatementInfo(bool isTrigger, std::string name, std::string type, std::string description) :
                isTrigger(isTrigger), name(name), type(type), description(description)
        {
        }
        bool isTrigger;
        std::string name;
        std::string type;
        std::string description;
    };

    class Action;
    class Trigger;

    class Statement: public QObject
    {
    Q_OBJECT

    public:
        Statement(StatementModel* model);
        virtual ~Statement();
        virtual void remove();

        int getId();
        int getPosition();
        virtual std::string getType();
        virtual const StatementInfo getInfo() = 0;

        // This is for standalone execution of statement. This is not used when running part of procedure.
        ExecutionState* prepareStart(ConnectExecutionModel* dataModel, bool debug = true);
        void start();
        void stop();

        virtual bool checkParameterValidity(ExecutionState* state = NULL);
        static bool checkRegexMatch(std::string value, std::string validation, bool caseSensitive =
                true);
        virtual bool checkParameterValidityAndAllChildren(ExecutionState* state) = 0;

        StatementModel* getModel() const;

        int getParameterDefinitionCount();
        virtual std::vector<ParameterDefinition*>* getParameterDefinitions();

        /**Gets parameter. If state is set, resolves variables.*/
        std::string getParameter(std::string key, ExecutionState* state = NULL);
        /**Gets sub parameter*/
        std::string getSubParameter(std::string key, std::string subkey, ExecutionState* state =
                NULL);
        void setParameter(std::string key, std::string value); /**Sets parameter. If state is set, makes variables visible to UI.*/
        void setSubParameter(std::string key, std::string subkey, std::string value);

        virtual std::vector<statement_output_variable_definition*>* getExposedVariableDefinitions();

        static std::string isValueTrue(bool value);
        static bool isValueTrue(std::string value);

        // Derived branch classes should populate branchInfo variable and implement getBranch,newBranchChild methods
        std::vector<branch_info> getBranchInfo();
        virtual Statement* getBranch(std::string key, std::string type);
        virtual Statement* newBranchChild(std::string branchName, std::string statementType);
        virtual Statement* pasteBranchChild(std::string branchName);

        virtual void initNextStatement(Statement* nextStatement);
        virtual Statement* getNextStatement() const;
        virtual Statement* getLastStatement() const;
        virtual void setNextStatement(Statement* nextStatement);
        virtual void unsetNextStatement();
        virtual void unsetPreviousStatement();

        // Retrieves granularity parameter if set or default value in microseconds
        useconds_t getGranularity(ExecutionState* state);

        // Statement execute
        virtual void execute(ExecutionState* state) = 0;

    Q_SIGNALS:
        void notifyChangePosition();
    protected:
        StatementModel* model;
        std::vector<branch_info> branchInfo;
        /**Gets default parameter. Should be implemented in subclass*/
        virtual std::string getDefaultParameter(std::string key);
    private:
        inline std::string combine(std::string key, std::string subkey);
        std::string expandVariables(std::string value, ExecutionState* state) const;
        std::vector<std::string> availableVariables;
        static Logger* LOG;

        int currentExecutionNumber;
        ExecutionState* pendingExecutionState;
        ExecutionState* currentExecutionState;
        pthread_mutex_t deadbolt;
        pthread_mutexattr_t mattr;

        Statement* nextStatement;
    };

} /* namespace actiontrigger */
#endif /* STATEMENT_HPP_ */
