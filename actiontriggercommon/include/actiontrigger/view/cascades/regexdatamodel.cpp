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

#include "actiontrigger/view/cascades/regexdatamodel.hpp"

namespace actiontrigger
{

    Logger* RegexDataModel::LOG = LoggerFactory::getInstance("RegexDataModel");

    RegexDataModel::RegexDataModel(QObject *parent) :
            bb::cascades::GroupDataModel(parent), filteredDataLock(QReadWriteLock::Recursive)
    {
        this->filter.setCaseSensitivity(Qt::CaseInsensitive);
    }

    int RegexDataModel::childCount(const QVariantList& indexPath)
    {
//    QReadLocker locker(&filteredDataLock);
        if (!isPatternSet()) {
            return GroupDataModel::childCount(indexPath);
        }
        return getFilteredItemParent(indexPath).size() - 1;
    }

    bool RegexDataModel::hasChildren(const QVariantList& indexPath)
    {
//    QReadLocker locker(&filteredDataLock);
        return childCount(indexPath) != 0;
    }

    QVariant RegexDataModel::data(const QVariantList &indexPath)
    {
//    QReadLocker locker(&filteredDataLock);
        if (!isPatternSet()) {
            return GroupDataModel::data(indexPath);
        }
        return getFilteredItemParent(indexPath).first();
    }

    void RegexDataModel::resetFilter()
    {
//    QWriteLocker locker(&filteredDataLock);
        setFilter("");
    }

    void RegexDataModel::setFilter(QString pattern)
    {
//    QWriteLocker locker(&filteredDataLock);
        filter.setPattern(pattern);
        if (isPatternSet()) {
            filteredData = populateData(pattern);
        }
        emit itemsChanged(bb::cascades::DataModelChangeType::AddRemove);
    }

// Base case
    QVariantList RegexDataModel::populateData(QString pattern, QVariantList indexPath)
    {
        QVariant item = GroupDataModel::data(indexPath);
        QVariantList data;
        data.append(item);
        int childCount = GroupDataModel::childCount(indexPath);

        // Check if item is a leaf
        if (childCount == 0) {
            if (isMatch(item, "name") || isMatch(item, "description")) {
                // Leaf node that is a match
                return data;
            } else {
                // Leaf node that is not a match
                return QVariantList(); // Empty list
            }
        }

        // Item is a parent, iterate all children
        for (int i = 0; i < GroupDataModel::childCount(indexPath); i++) {
            QVariantList childIndexPath = QVariantList(indexPath);
            childIndexPath.append(QVariant(i));
            QVariantList child = populateData(pattern, childIndexPath);
            if (!child.isEmpty()) {
                data.append(QVariant(child));
            }
        }

        // Base case will always be set even if no children are present
        if (indexPath.isEmpty()) {
            return data;
        }

        // If there are no children, ignore this parent
        if (data.size() <= 1 && !indexPath.isEmpty()) {
            return QVariantList(); // Empty list
        }

        // Is a parent with at least one child that is a match
        return data;
    }

    QVariantList RegexDataModel::getFilteredItemParent(const QVariantList& indexPath)
    {
        // Find the itemParent by searching through our filtered data
        int indexPathPointer = 0;
        QVariantList itemParent = filteredData;
        while (indexPathPointer < indexPath.size()) {
            itemParent = itemParent.at(1 + indexPath.at(indexPathPointer++).toInt()).toList();
        }
        return itemParent;
    }

    bool RegexDataModel::isPatternSet()
    {
        return !filter.isEmpty() && filter.isValid();
    }

    bool RegexDataModel::isMatch(QVariant item, std::string field)
    {
        // Retrieve name variant
        if (item.userType() != qMetaTypeId<QObject*>()) {
            LOG->error(
                    SSTR("Item is not a QObject, it is meta type id: " << qMetaTypeId<QObject*>()));
            return false;
        }
        if (!item.canConvert<QObject*>()) {
            LOG->error("Item cannot be converted to QObject");
            return false;
        }
        QVariant nameVariant = item.value<QObject *>()->property(field.c_str());

        // Ensure name is present
        if (!nameVariant.isValid()) {
            LOG->error("Item does not have a name property");
            return false;
        }

        // Check if name is a string
        if (nameVariant.userType() != qMetaTypeId<QString>()) {
            LOG->error(
                    SSTR(
                            "Property name is not a QString, it is meta type id: "
                                    << qMetaTypeId<QObject*>()));
            return false;
        }
        QString name = qvariant_cast<QString>(nameVariant);

        // Check regex match
        return name.contains(filter);
    }

} /* namespace actiontrigger */
