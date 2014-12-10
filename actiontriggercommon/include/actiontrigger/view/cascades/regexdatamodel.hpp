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

#ifndef RegexDataModel_HPP
#define RegexDataModel_HPP

#include <QObject>
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <QVariantList>
#include <QRegExp>
#include <bb/cascades/GroupDataModel>
#include "actiontrigger/model/loggerfactory.hpp"

namespace actiontrigger
{

    /**
     * TODO Cache filtered item instead of calculating on the fly
     */
    class RegexDataModel: public bb::cascades::GroupDataModel
    {
    Q_OBJECT

    public:
        RegexDataModel(QObject *parent = 0);

        virtual int childCount(const QVariantList& indexPath);
        virtual bool hasChildren(const QVariantList& indexPath);
        virtual QVariant data(const QVariantList &indexPath);

        Q_INVOKABLE
        void setFilter(QString pattern);Q_INVOKABLE
        void resetFilter();

    private:
        QVariantList populateData(QString pattern, QVariantList indexPath = QVariantList());
        QVariantList getFilteredItemParent(const QVariantList& indexPath);
        bool isPatternSet();
        bool isMatch(QVariant item, std::string field);

        static Logger* LOG;
        QRegExp filter;
        /**
         * Holds copy of filtered data. Item at index 0 is always the data item while items from index 1..x are recursive children of type QVariantList
         */
        QVariantList filteredData;
        QReadWriteLock filteredDataLock;
    };

} /* namespace actiontrigger */
#endif /* RegexDataModel_HPP */
