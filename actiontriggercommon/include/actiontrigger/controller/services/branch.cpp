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

#include "actiontrigger/controller/services/branch.hpp"

// Prevent cyclic dependancy
#include "actiontrigger/controller/servicemanagement.hpp"

namespace actiontrigger
{

    Logger* Branch::LOG = LoggerFactory::getInstance("Branch");

    Branch::Branch(StatementModel* model) :
            Action(model), triggerBranches(NULL), actionBranches(NULL)
    {
    }

    void Branch::initBranches(std::map<std::string, Trigger*>* triggerBranches,
            std::map<std::string, Action*>* actionBranches)
    {
        this->triggerBranches = triggerBranches;
        this->actionBranches = actionBranches;
    }

    Branch::~Branch()
    {
    }

//void Branch::remove() {
//    Action::remove();
//    for (std::map<std::string, Action*>::iterator iter = actionBranches->begin(); iter != actionBranches->end(); ++iter) {
//        iter->second->remove();
//        delete iter->second;
//    }
//    actionBranches->clear();
//    for (std::map<std::string, Trigger*>::iterator iter = triggerBranches->begin(); iter != triggerBranches->end(); ++iter) {
//        iter->second->remove();
//        delete iter->second;
//    }
//    triggerBranches->clear();
//}

    void Branch::removeChild(std::string branchName, Statement* statement)
    {
        // Find the correct branch
        Statement* branchStatement;
        if (triggerBranches->find(branchName) != triggerBranches->end()) {
            branchStatement = triggerBranches->find(branchName)->second;
        } else if (actionBranches->find(branchName) != actionBranches->end()) {
            branchStatement = actionBranches->find(branchName)->second;
        } else {
            LOG->error("Statement not found");
            return;
        }

        if (branchStatement == NULL) {
            LOG->error("Statement not found");
            return;
        }

        // Holds pointers to three consecutive statements
        Statement* previousStatement = NULL; // Needs to be found
        Statement* deleteStatement = statement;
        Statement* nextStatement = deleteStatement->getNextStatement();

        if (branchStatement->getId() != deleteStatement->getId()) {
            // Statement is not at the head, need to find the previous statement
            previousStatement = branchStatement;
            if (previousStatement->getNextStatement() == NULL) {
                LOG->error("Statement not found");
                return;
            }
            while (previousStatement->getNextStatement()->getId() != deleteStatement->getId()) {
                previousStatement = previousStatement->getNextStatement();
                if (previousStatement->getNextStatement() == NULL) {
                    LOG->error("Statement not found");
                    return;
                }
            }
        }

        // Perform the removal
        if (NULL == previousStatement) {
            if (nextStatement != NULL) {
                // Statement to delete is at head of the branch
                if (triggerBranches->find(branchName) != triggerBranches->end()) {
                    triggerBranches->find(branchName)->second = (Trigger*) nextStatement;
                } else if (actionBranches->find(branchName) != actionBranches->end()) {
                    actionBranches->find(branchName)->second = (Action*) nextStatement;
                } else {
                    LOG->critical("This should never happen");
                    return;
                }
                model->setBranchStatement(branchName, nextStatement->getModel());
                nextStatement->unsetPreviousStatement();
            } else {
                // Statement to delete is the only statement in the branch
                if (triggerBranches->find(branchName) != triggerBranches->end()) {
                    triggerBranches->erase(branchName);
                } else if (actionBranches->find(branchName) != actionBranches->end()) {
                    actionBranches->erase(branchName);
                } else {
                    LOG->critical("This should never happen");
                    return;
                }
                model->unsetBranchStatement(branchName);
            }
        } else {
            if (nextStatement != NULL) {
                // Statement to delete is in the middle of the chain
                previousStatement->setNextStatement(nextStatement);
            } else {
                // Statement to delete is the last statement in chain
                previousStatement->unsetNextStatement();
            }
        }
        deleteStatement->remove();
    }

    bool Branch::moveDown(std::string branchName, Statement* moveDownStatement)
    {
        if (moveDownStatement->getNextStatement() == NULL) {
            LOG->error("Statement not found");
            return false;
        }
        return moveUp(branchName, moveDownStatement->getNextStatement());
    }

    bool Branch::moveUp(std::string branchName, Statement* statement)
    {
        // Find the correct branch
        Statement* branchStatement;
        if (triggerBranches->find(branchName) != triggerBranches->end()) {
            branchStatement = triggerBranches->find(branchName)->second;
        } else if (actionBranches->find(branchName) != actionBranches->end()) {
            branchStatement = actionBranches->find(branchName)->second;
        } else {
            LOG->error("Statement not found");
            return false;
        }

        if (branchStatement->getId() == statement->getId()) {
            LOG->error("Statement already at the top");
            return false;
        }
        if (branchStatement->getNextStatement() == NULL) {
            LOG->error("Statement not found");
            return false;
        }

        // Holds pointers to four consecutive actions
        Statement* previousStatement = NULL; // Needs to be found
        Statement* moveDownStatement = branchStatement; // Needs to be found
        Statement* moveUpStatement = statement;
        Statement* nextStatement = moveUpStatement->getNextStatement();

        // Find the previousStatement and moveDownStatement
        while (moveDownStatement->getNextStatement()->getId() != moveUpStatement->getId()) {
            previousStatement = moveDownStatement;
            moveDownStatement = moveDownStatement->getNextStatement();
            if (moveDownStatement->getNextStatement() == NULL) {
                LOG->error("Statement not found");
                return false;
            }
        }

        // Perform the swap
        if (NULL == previousStatement) {
            // Is moved to the top of the branch, let's find that branch again...
            if (triggerBranches->find(branchName) != triggerBranches->end()) {
                triggerBranches->find(branchName)->second = (Trigger*) moveUpStatement;
            } else if (actionBranches->find(branchName) != actionBranches->end()) {
                actionBranches->find(branchName)->second = (Action*) moveUpStatement;
            } else {
                LOG->critical("This should never happen");
                return false;
            }
            moveUpStatement->unsetPreviousStatement();
        } else {
            previousStatement->setNextStatement(moveUpStatement);
        }
        moveUpStatement->setNextStatement(moveDownStatement);
        if (NULL == nextStatement) {
            moveDownStatement->unsetNextStatement();
        } else {
            moveDownStatement->setNextStatement(nextStatement);
        }

        return true;
    }

    Statement* Branch::newBranchChild(std::string branchName, std::string statementType)
    {
        return appendBranchChild(branchName, StatementDBModel::create(READ_CACHE, // Database mode
                statementType // Type
                ));
    }

    Statement* Branch::pasteBranchChild(std::string branchName)
    {
        return appendBranchChild(branchName,
                StatementDBModel::create(READ_CACHE, GlobalResources::get()->clipboardRetrieve()));
    }

    Statement* Branch::appendBranchChild(std::string branchName, StatementModel* newModel)
    {
        std::string branchType = "";
        for (std::vector<branch_info>::iterator it = branchInfo.begin(); it != branchInfo.end();
                ++it) {
            if ((*it).key == branchName) {
                branchType = (*it).type;
                break;
            }
        }
        if (branchType == "ACTION") {
            Action* newAction = ServiceManagement::getInstance()->loadAction(newModel);
            Action* lastAction = getActionBranch(branchName);
            if (lastAction == NULL) {
                // Branch is empty, make this the first one
                model->setBranchStatement(branchName, newModel);
                actionBranches->insert(std::pair<std::string, Action*>(branchName, newAction));
            } else {
                // Branch is non empty, append at the end
                lastAction->getLastStatement()->setNextStatement(newAction);
            }
            return (Statement*) newAction;
        } else if (branchType == "TRIGGER") {
            Trigger* newTrigger = ServiceManagement::getInstance()->loadTrigger(newModel);
            Trigger* lastTrigger = getTriggerBranch(branchName);
            if (lastTrigger == NULL) {
                // Branch is empty, make this the first one
                model->setBranchStatement(branchName, newModel);
                triggerBranches->insert(std::pair<std::string, Trigger*>(branchName, newTrigger));
            } else {
                // Branch is non empty, append at the end
                lastTrigger->getLastStatement()->setNextStatement(newTrigger);
            }
            return (Statement*) newTrigger;
        } else {
            LOG->error("Unknown branch type: " + branchType);
        }
        return NULL;
    }

    Statement* Branch::getBranch(std::string key)
    {
        LOG->trace("getBranch(" + key + ")");
        for (std::vector<branch_info>::iterator it = branchInfo.begin(); it != branchInfo.end();
                ++it) {
            if ((*it).key == key) {
                if ((*it).type == "ACTION") {
                    return getActionBranch(key);
                }
                if ((*it).type == "TRIGGER") {
                    return getTriggerBranch(key);
                }
                LOG->error("Unknown branch type: " + (*it).type);
            }
        }
        return NULL;
    }

    Statement* Branch::getBranch(std::string key, std::string type)
    {
        LOG->trace("getBranch(" + key + "," + type + ")");
        if (type == "ACTION") {
            return getActionBranch(key);
        }
        if (type == "TRIGGER") {
            return getTriggerBranch(key);
        }
        LOG->error("Unknown branch type: " + type);
        return NULL;
    }

    Action* Branch::getActionBranch(std::string key)
    {
        if (actionBranches->find(key) != actionBranches->end())
            return actionBranches->find(key)->second;
        return NULL;
    }

    Trigger* Branch::getTriggerBranch(std::string key)
    {
        if (triggerBranches->find(key) != triggerBranches->end())
            return triggerBranches->find(key)->second;
        return NULL;
    }

    std::vector<statement_output_variable_definition*>* Branch::getExposedVariableDefinitions()
    {
        std::vector<statement_output_variable_definition*>* vars = new std::vector<
                statement_output_variable_definition*>();
        for (std::map<std::string, Action*>::iterator iter = actionBranches->begin();
                iter != actionBranches->end(); ++iter) {
            Statement* nextStatement = iter->second->getNextStatement();
            while (nextStatement) {
                std::vector<statement_output_variable_definition*>* statementVars =
                        nextStatement->getExposedVariableDefinitions();
                if (NULL != statementVars) {
                    vars->insert(vars->end(), statementVars->begin(), statementVars->end());
                    delete statementVars;
                }
                nextStatement = nextStatement->getNextStatement();
            }
        }
        for (std::map<std::string, Trigger*>::iterator iter = triggerBranches->begin();
                iter != triggerBranches->end(); ++iter) {
            Statement* nextStatement = iter->second->getNextStatement();
            while (nextStatement) {
                std::vector<statement_output_variable_definition*>* statementVars =
                        nextStatement->getExposedVariableDefinitions();
                if (NULL != statementVars) {
                    vars->insert(vars->end(), statementVars->begin(), statementVars->end());
                    delete statementVars;
                }
                nextStatement = nextStatement->getNextStatement();
            }
        }
        return vars;
    }

} /* namespace actiontrigger */
