
/***************************************************************************
    begin                : Sat Dec 22 2001
    copyright            : (C) 2001-2018 by Keith Isdale
    email                : keithisdale@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef XSLDBGSETTINGSMODEL_H
#define XSLDBGSETTINGSMODEL_H

#include <QVariant>
#include <QStringList>
#include <QAbstractTableModel>
#include <QHash>
#include <QMutex>
#include <QSettings>

class XsldbgSettingsModelPrivate;

class XsldbgSettingData
{
    public:
        XsldbgSettingData();
        XsldbgSettingData(const QString & name, const QVariant &value, int row);
        XsldbgSettingData(int optionID, const QVariant &value, int row);
        XsldbgSettingData & operator=(const XsldbgSettingData & other);

        QString m_name;
        QVariant m_value;
        int m_type;
        int m_id;
        int m_row;
        static QString myKey(const QString &name, int type);
        static int paramId;
};

typedef QHash<QString, XsldbgSettingData>::iterator XsldbgSettingDataIterator;
typedef QHash<QString, XsldbgSettingData>::const_iterator XsldbgSettingDataConstIterator;
QDebug operator<< ( QDebug stream, const XsldbgSettingData &item);


/**
 * class XsldbgSettingsModel
 */

class XsldbgSettingsModel : public QAbstractTableModel
{
    public:

        enum SettingsType {
            UnknownSettingType = -1,
            BoolSettingType = 1,
            IntSettingType = 2,
            StringSettingType = 4,
            ParamSettingType = 8,
            HiddenSettingType = 16,
            VisibleSettingType = BoolSettingType | IntSettingType | StringSettingType | ParamSettingType,
            AnySimpleSettingType = BoolSettingType | IntSettingType | StringSettingType | HiddenSettingType,
            AnySettingType = BoolSettingType | IntSettingType | StringSettingType | ParamSettingType | HiddenSettingType
        };

        enum SortType {
            SortNone,
            SortById = 1,
            SortByName 
        };

        static const int SettingTypeRole = Qt::UserRole + 2;
        static const int SettingNameRole = Qt::UserRole + 3;
        static const int SettingValueRole = Qt::UserRole + 4;
        static const int SettingIDRole = Qt::UserRole + 5;

        /**
         * Empty Constructor
         */
        XsldbgSettingsModel();

        // perform setup of the settings, reverting to a pristine state
        void init();

        /**
         * Empty Destructor
         */
        virtual ~XsldbgSettingsModel();

        /**
         * Sets the role data for the item at index to value. Returns true if successful;
         * otherwise returns false.
         * @return Sets the role data for the item at index to value. Returns true if successful;
         * otherwise returns false.
         * @param  index
         * @param  value
         * @param  role
         */
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);


        /**
         * Returns the data stored under the given role for the item referred to by the
         * index.
         * @return Returns the data stored under the given role for the item referred to by the 
         * index
         * @param  index
         * @param  role
         */
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;


        /**
         * Returns the number of columns for the children of the given parent. When the
         * parent is valid it means that rowCount is returning the number of children of
         * parent.
         * @return Returns the number of columns for the children of the given parent. When the
         * parent is valid it means that rowCount is returning the number of children of
         * parent 
         * @param  parent
         */
        int columnCount(const QModelIndex & parent = QModelIndex()) const;


        /**
         * Returns the number of rows under the given parent. When the parent is valid it
         * means that rowCount is returning the number of children of parent.
         * @return  Returns the number of rows under the given parent. When the parent is valid it
         * means that rowCount is returning the number of children of parent.
         * @param  parent
         */
        int rowCount(const QModelIndex & parent = QModelIndex()) const;


        /**
         * Add a XSLT parameter
         * @return Returns true if able to add XSLT parameter with name @a name and value @a value
         *            otherwise return false
         * @param  name
         * @param  value
         */
        bool addParameter(const QString & name, const QVariant & value);

        /**
         * Remove a XSLT parameter
         */
        bool removeParameter(const QString & name, bool removingAllParameters=false);

        void removeAllParameters();

        /**
         * Return a list of setting name that match type requested
         * @return Return a list of setting name that match type requested
         * @param  settingtype
         * @param  sortType what sorting is required, if non required
         */
        QStringList settingsList(SettingsType settingType, SortType sortType=SortNone) const;


        /**
         * Returns true if able to find the item named @a name that matches type @a settingType and 
         *  sets outItem to the value of item found otherwise false
         * @return Returns true if able to find the item named @a name that matches type @a type and 
         *  sets outItem to the value of item found otherwise false
         * @param name
         * @param settingType
         * @param outItem
         */
        bool findSetting(const QString & name, SettingsType settingType, XsldbgSettingData &outItem) const;


        /**
         * Returns true if able to find the item with optionID @a optionID and 
         *  sets outItem to the value of item found otherwise false
         * @returns Returns true if able to find the item with optionID @a optionID and 
         *  sets outItem to the value of item found otherwise false
         * @param optionID
         * @param outItem
         */
        bool findSetting(int optionID, XsldbgSettingData &outItem) const;


        int  findModelRow(int optionID, bool isParameter=false) const;


        bool updateSetting(int optionID, QVariant & value);


        /**
         * Return true if able to save settings to supplied configGroup
         * @return bool
         * @param  configGroup
         */
        bool saveSettings (QSettings & configSettings ) const;


        /**
         * Return true if able to loadsettings from supplied configGroup
         * @return bool
         * @param  configGroup
         */
        bool loadSettings (const QSettings & configSettings );

        void lock(bool lockState);

    private:
        XsldbgSettingsModelPrivate *d_ptr;
        QString optionPrefix;
        QString paramPrefix;
        QMutex mutext;
};

#endif // XSLDBGSETTINGSMODEL_H
