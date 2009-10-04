/**
 *
 *  This file is part of the kxsldbg package
 *  Copyright (c) 2008 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or
 *  This library is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU General Public License as 
 *  published by the Free Software Foundation; either version 2 of 
 *  the License, or (at your option) any later version.
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this library; see the file COPYING.LIB.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA 02110-1301, USA.
 **/


#include <QDebug>
#include "libxsldbg/options.h"


QString XsldbgSettingData::myKey(const QString &name, int type)
{
    QString result = name;

    if (type >0 &&type < XsldbgSettingsModel::AnySettingType)
        result += '0' + type;

    return result;
}

XsldbgSettingData & XsldbgSettingData::operator=(const XsldbgSettingData & other)
{
    m_name = other.m_name;
    m_value = other.m_value;
    m_type = other.m_type;
    m_id = other.m_id;
    m_row = other.m_row;

    return *this;
}


uint qHash(const XsldbgSettingData &key);
bool operator== (const XsldbgSettingData &e1, const XsldbgSettingData &e2);

QDebug operator<< ( QDebug stream, const XsldbgSettingData &item)
{
    stream << " XsldbgSettingData(" << item.m_name << "," << \
        item.m_value << "," << item.m_type << "," << item.m_id << "," << item.m_row << ")";

    return stream;
}

class XsldbgSettingsModelPrivate
{
    public:
        void updateIndex();
        QHash<QString, XsldbgSettingData> settingData;
        QStringList keyIndex;
};

// speed things up by having a mapping from row index to hash keys in settingData
void XsldbgSettingsModelPrivate::updateIndex()
{
    keyIndex.clear();
    for (XsldbgSettingDataIterator it = settingData.begin(); it !=  settingData.end(); it++)
        keyIndex.append(XsldbgSettingData::myKey(it->m_name, it->m_type));

    // ensure the row values are updated
    int row= 0;
    for (XsldbgSettingDataIterator it = settingData.begin();
            it != settingData.end(); it++)
        it->m_row = row++;
}


    XsldbgSettingData::XsldbgSettingData()
: m_type(XsldbgSettingsModel::UnknownSettingType),
    m_id(-1),
    m_row(-1)
{
}


    XsldbgSettingData::XsldbgSettingData(const QString & name, const QVariant &value, int row)
: m_name(name),
    m_value(value),
    m_type(XsldbgSettingsModel::ParamSettingType),
    m_row(row)
{
    static int paramId = 0;
    m_id = paramId++;
}



    XsldbgSettingData::XsldbgSettingData(int optionID, const QVariant &value, int row)
: m_id(optionID),
    m_row(row)
{
    int type = XsldbgSettingsModel::UnknownSettingType;

    if (optionID >= OPTIONS_FIRST_BOOL_OPTIONID &&
            optionID <= OPTIONS_LAST_BOOL_OPTIONID) {
        type = XsldbgSettingsModel::BoolSettingType;
        m_value = QVariant(QVariant::Bool);
        m_value = value.toBool();
    }else if (optionID >= OPTIONS_FIRST_OPTIONID &&
            optionID <= OPTIONS_LAST_INT_OPTIONID){
        type = XsldbgSettingsModel::IntSettingType;
        m_value = QVariant(QVariant::Int);
        m_value = value.toInt();
    }else if (optionID >= OPTIONS_FIRST_STRING_OPTIONID &&
            optionID <= OPTIONS_LAST_STRING_OPTIONID){
        type = XsldbgSettingsModel::StringSettingType;
        m_value = QVariant(QVariant::String);
        m_value = value.toString();
    }
    m_type = type;
    Q_ASSERT(type != XsldbgSettingsModel::UnknownSettingType);
    m_name = optionsGetOptionName(OptionTypeEnum(optionID));
    // hidden options should not be written to configuration file nor displayed to thge user
    if (m_name.left(2) == "*_")
        m_type |= XsldbgSettingsModel::HiddenSettingType;
}


bool operator==(const XsldbgSettingData &e1, const XsldbgSettingData &e2)
{
    return e1.m_type == e2.m_type && e1.m_name == e2.m_name;
}


uint qHash(const XsldbgSettingData &key)
{
    return qHash(key.m_name + ('0' + key.m_type));
}


XsldbgSettingsModel::XsldbgSettingsModel()
{
    d_ptr = new XsldbgSettingsModelPrivate;
    Q_ASSERT(d_ptr);
    QVariant value;
    optionPrefix = "option_";
    paramPrefix = "param_";

    // Add xsldbg options
    beginInsertRows(QModelIndex(), 0, OPTIONS_LAST_OPTIONID - OPTIONS_FIRST_OPTIONID);
    for (int optionID = OPTIONS_FIRST_OPTIONID; optionID <= OPTIONS_LAST_OPTIONID; optionID++) {
        XsldbgSettingData item(optionID, value, d_ptr->settingData.count());
        QString key(XsldbgSettingData::myKey(item.m_name, item.m_type));
        d_ptr->settingData.insert(key, item);
    }
    endInsertRows();
    init(); // set the default valus and clear any parameters
}

XsldbgSettingsModel::~XsldbgSettingsModel()
{
}

void XsldbgSettingsModel::init()
{
    d_ptr->updateIndex();
    QVariant boolValue(QVariant::Bool);
    QVariant intValue(QVariant::Int);
    QVariant stringValue(QVariant::String);

    // set the default values for certain options
    /* setup the docs path */
    stringValue = langLookupDir("xsldbghelp.xml");
    updateSetting(OPTIONS_DOCS_PATH, stringValue);

    intValue = TRACE_OFF;
    updateSetting(OPTIONS_TRACE, intValue);

    intValue = WALKSPEED_STOP;
    updateSetting(OPTIONS_WALK_SPEED, intValue);

    /* always try to use encoding if found */
    boolValue = true;
    updateSetting(OPTIONS_AUTOENCODE, boolValue);

    /* always try starting with net loading entity driver enabled */
    boolValue = true;
    updateSetting(OPTIONS_NET, boolValue);

    /* start up with auto config load turned on */
    boolValue = true;
    updateSetting(OPTIONS_AUTOLOADCONFIG, boolValue);

    /* start up with auto restart turned off */
    boolValue = false ;
    updateSetting(OPTIONS_AUTORESTART, boolValue);

    /* start up with gdb mode turned on */
    intValue = 1;
    updateSetting(OPTIONS_GDB, intValue);

    /* start up with output mode turned on */
    boolValue = true;
    updateSetting(OPTIONS_OUT, boolValue);

    /* start up with validation turned on */
    boolValue = true;
    updateSetting(OPTIONS_VALID, boolValue);

    /* start up with xinclude turned on */
    boolValue = true;
    updateSetting(OPTIONS_XINCLUDE, boolValue);

    /* set output default as standard output. Must be changed if not using
     * xsldbg's command line. Or the tty command is used */
    stringValue = QString();
    updateSetting(OPTIONS_OUTPUT_FILE_NAME, stringValue);

    QStringList paramsList = settingsList(XsldbgSettingsModel::ParamSettingType);
    QString param;
    foreach (param, paramsList)
        (void)removeParameter(param);

    d_ptr->updateIndex();
    reset();
}

bool XsldbgSettingsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    lock(true);
    bool result = false;
    if (index.row() < d_ptr->keyIndex.count()){
        QString key = d_ptr->keyIndex[index.row()];
        XsldbgSettingDataIterator it = d_ptr->settingData.find(key);
        if (it != d_ptr->settingData.end()){
            if (index.column() == 0  || role == SettingNameRole) {
                it->m_name = value.toString();
                result = true;
                dataChanged(index,index);
            }else if (index.column() == 1 || role == SettingValueRole) {
                if (it->m_type & XsldbgSettingsModel::StringSettingType){
                    it->m_value = value.toString();
                    result = true;
                    dataChanged(index,index);
                }else if ( it->m_type & XsldbgSettingsModel::BoolSettingType) {
                    bool newValue = value.toBool();
                    it->m_value = newValue;
                    result = true;
                    dataChanged(index,index);
                }else {
                    int intValue = value.toInt(&result);
                    if (result){
                        it->m_value  = intValue;
                        dataChanged(index,index);
                    }
                }
            }
        }
    }
    lock(false);
    return result;
}


QVariant XsldbgSettingsModel::data(const QModelIndex & index, int role) const
{
    QVariant result;
    if (index.row() < d_ptr->keyIndex.count()){
        QString key = d_ptr->keyIndex[index.row()];
        XsldbgSettingDataConstIterator it = d_ptr->settingData.constFind(key);
        if (it != d_ptr->settingData.constEnd()){
            switch(role) {
                case SettingTypeRole:
                result = it->m_type;
                break;

                case SettingNameRole:
                result = it->m_name;
                break;

                case SettingValueRole:
                result = it->m_value;
                break;

                case SettingIDRole:
                result = it->m_id;
                break;

                case Qt::DisplayRole:
                case Qt::EditRole:
                if (index.column() == 0) 
                    result = it->m_name;
                else if (index.column() == 1) 
                    result = it->m_value;
                break;
            }
        }
    }
    return result;
}


int XsldbgSettingsModel::columnCount(const QModelIndex & parent) const
{
    int result;

    if (!parent.isValid())
        result = 2;
    else
        result = 0;

    return result;
}


int XsldbgSettingsModel::rowCount(const QModelIndex & parent) const
{
    int result;

    if (!parent.isValid())
        result = d_ptr->settingData.count();
    else
        result = 0;

    return result;
}


bool XsldbgSettingsModel::addParameter(const QString & name, const QVariant &value)
{
    bool result = false;
    if (name.isEmpty())
        return result;

    lock(true);
    QString key(XsldbgSettingData::myKey(name, XsldbgSettingsModel::ParamSettingType));
    if (d_ptr->settingData.find(key) == d_ptr->settingData.end()){
        int row = d_ptr->settingData.count();
        XsldbgSettingData item(name, value, row);
        beginInsertRows(QModelIndex(), row, row);
        d_ptr->settingData.insert(key, item);
        d_ptr->updateIndex();
        endInsertRows();
        result = true;
        reset();
    }else {
        qWarning("Unable to update existing libxslt parameter %s", name.toLatin1().constData());
        //TODO update the existing value
    }
    lock(false);
    return result;
}

bool XsldbgSettingsModel::removeParameter(const QString & name)
{
    bool result = false;
    int paramId = -1;
    bool parsedId = false;

    if (name.isEmpty())
        return result;

    lock(true);
    paramId = name.toInt(&parsedId);
    if (!parsedId){
        // try looking for parameter with provided name
        QString key(XsldbgSettingData::myKey(name, XsldbgSettingsModel::ParamSettingType));
        XsldbgSettingDataIterator it = d_ptr->settingData.find(key);
        if (it != d_ptr->settingData.end()){
            int row = it->m_row;
            beginRemoveRows(QModelIndex(), row, row);
            result = d_ptr->settingData.remove(key) > 0;
            d_ptr->updateIndex();
            endRemoveRows();
            result = true;
        }
    }else {
        for (XsldbgSettingDataIterator it = d_ptr->settingData.begin();
                it != d_ptr->settingData.end(); it++) {
            if (it->m_type == XsldbgSettingsModel::ParamSettingType &&
                    it->m_id == paramId) {
                QString key(XsldbgSettingData::myKey(it->m_name, XsldbgSettingsModel::ParamSettingType));
                int row = it->m_row;
                beginRemoveRows(QModelIndex(), row, row);
                result = d_ptr->settingData.remove(key) > 0;
                d_ptr->updateIndex();
                endRemoveRows();
                result = true;
                break;
            }
        }

    }
    lock(false);
    if (result)
        reset();
    return result;
}

void XsldbgSettingsModel::removeAllParameters()
{
    QStringList paramList = settingsList(XsldbgSettingsModel::ParamSettingType);
    foreach (QString param, paramList)
        removeParameter(param);
}


QStringList XsldbgSettingsModel::settingsList(SettingsType settingType, SortType sortType) const
{
    QStringList result;
    if (sortType == SortNone || sortType == SortByName){
        for (XsldbgSettingDataConstIterator it = d_ptr->settingData.constBegin(); it != d_ptr->settingData.constEnd(); it++) {
            if (it->m_type & settingType) 
                result.append(it->m_name);
        }
        if (sortType == SortByName)
            result.sort();
    }else {
        int seekRow=0;
        bool match;
        XsldbgSettingDataConstIterator lastMatch;
        for (int index =0; index < d_ptr->settingData.count(); index++){
            match = false;
            lastMatch = d_ptr->settingData.constEnd();
            for (XsldbgSettingDataConstIterator it = d_ptr->settingData.constBegin(); it != d_ptr->settingData.constEnd(); it++) {
                if ((it->m_type & settingType) && (it->m_id >= seekRow)){
                    if (lastMatch != d_ptr->settingData.constEnd() && (it->m_id < lastMatch->m_id)){
                        lastMatch = it;
                        match = true;
                    }else if (lastMatch == d_ptr->settingData.constEnd()){
                        lastMatch = it;
                        match = true;
                    }
                }
            }
            if (match){
                result.append(lastMatch->m_name);
                seekRow = lastMatch->m_id + 1;
            }else {
                seekRow++;
            }
        }
    }
    return result;
}


bool XsldbgSettingsModel::findSetting(const QString & name, SettingsType settingType,
        XsldbgSettingData &outItem) const
{
    bool result = false;
    for (XsldbgSettingDataConstIterator it = d_ptr->settingData.constBegin();
            it != d_ptr->settingData.constEnd(); it++) {
        if ((it->m_name == name) && (it->m_type & settingType)) {
            outItem = (*it);
            result = true;
            break;
        }
    }
    return result;
}

bool XsldbgSettingsModel::findSetting(int optionID, XsldbgSettingData &outItem) const
{
    bool result = false;
    for (XsldbgSettingDataConstIterator it = d_ptr->settingData.constBegin();
            it != d_ptr->settingData.constEnd(); it++) {
        if ((it->m_id == optionID) && (it->m_type & AnySimpleSettingType)) {
            outItem = (*it);
            result = true;
            break;
        }
    }
    return result;
}


int XsldbgSettingsModel::findModelRow(int optionID, bool isParameter/*=false*/) const
{
    int result=-1;
    // return what is the row for optionID
    int optionType;
    if (!isParameter)
      optionType = AnySimpleSettingType;
    else
      optionType = ParamSettingType;
    for (XsldbgSettingDataIterator it = d_ptr->settingData.begin(); it !=  d_ptr->settingData.end(); it++){
        if ((it->m_id == optionID)  && (it->m_type & optionType)) {
            result = it->m_row;
            break;
        }
    }

    return result;
}

bool XsldbgSettingsModel::updateSetting(int optionID, QVariant & value)
{
    bool result = false;
    for (XsldbgSettingDataConstIterator it = d_ptr->settingData.constBegin();
            it != d_ptr->settingData.constEnd(); it++) {
        if ((it->m_id == optionID ) &&
                (it->m_type & (BoolSettingType | IntSettingType | StringSettingType) )){
            result = setData(index(it->m_row,1), value, Qt::EditRole);
            break;
        }
    }
    return result;
}


bool XsldbgSettingsModel::saveSettings (QSettings & configSettings ) const
{
    bool result = true;

    // remove any old parameters 
    QStringList settingsList = configSettings.allKeys();
    QString setting;
    foreach (setting, settingsList){
        if (setting.startsWith(paramPrefix))
            configSettings.remove(setting);
    }

    for (XsldbgSettingDataConstIterator it = d_ptr->settingData.constBegin();
            it != d_ptr->settingData.constEnd(); it++) {
        // do not save any hidden options to disk
        if (!(it->m_type & XsldbgSettingsModel::HiddenSettingType)){
            if (it->m_type & XsldbgSettingsModel::ParamSettingType) 
                configSettings.setValue(paramPrefix + it->m_name, it->m_value);
            else
                configSettings.setValue(optionPrefix + it->m_name, it->m_value);
        }
    }
    return result;
}


bool XsldbgSettingsModel::loadSettings (const QSettings & configSettings )
{
    bool result = true;
    init(); // remove any parameters and set settings back to thier defaults
    XsldbgSettingData item;
    QStringList settingsList = configSettings.allKeys();
    QString setting;
    QVariant value;
    int optionPrefixLen = optionPrefix.length();
    int paramPrefixLen = paramPrefix.length();
    // options are anything other than XSLT parameters
    XsldbgSettingsModel::SettingsType optionType = XsldbgSettingsModel::AnySimpleSettingType;
    foreach (setting, settingsList) {
        value = configSettings.value(setting, "");
        if (setting.startsWith(optionPrefix)){
            if (findSetting(setting.mid(optionPrefixLen), optionType,  item))
                updateSetting(item.m_id, value);
        }else if (setting.startsWith(paramPrefix)){
            addParameter(setting.mid(paramPrefixLen), value);
        }
    }
    return result;
}

void XsldbgSettingsModel::lock(bool lockState)
{
    if (lockState)
        mutext.lock();
    else
        mutext.unlock();
}
