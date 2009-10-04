
/**
 *
 *  This file is part of the kdewebdev package
 *  Copyright (c) 2001 Keith Isdale <keith@kdewebdev.org>
 *
 *  This library is free software; you can redistribute it and/or 
 *  modify it under the terms of the GNU General Public License as 
 *  published by the Free Software Foundation; either version 2 of 
 *  the License, or (at your option) any later version.
 *
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


#include "xsldbg.h"
#include "utils.h"
#include "options.h"
#include "xsldbgmsg.h"
#include "xsldbgthread.h"
#include "debugXSL.h"
#include <QDebug>


int xslDbgShellSetOption(xmlChar * arg)
{
    int result = 0;

    if (!arg) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
#endif
        return result;
    }

    if (xmlStrLen(arg) > 0) {
        xmlChar *opts[2];
        long optValue;
        long optID;
        int optionCount;
        bool pasedBoolOption = false;

        optionCount = splitString(arg, 2, opts);

        if ((optionCount == 1) || (optionCount == 2)) {
            bool invertOption = false;
            optID = optionsGetOptionID((const char*)opts[0]);
            if ((optID == -1) && (opts[0][0] == 'n') &&  (opts[0][1] == 'o')){
                optID = optionsGetOptionID((const char*)&opts[0][2]);
                if (optID != -1){
                    // invert the value of users choice, ie turn off this option
                    invertOption = true;     
                    pasedBoolOption = true;
                }
            }

            if (optID >= OPTIONS_FIRST_INT_OPTIONID) {
                if (optID <= OPTIONS_LAST_INT_OPTIONID) {
                    if (optionCount == 1){
                        // treat this as though the user had provided a value of 1
                        pasedBoolOption = true;
                        optValue=1;
                    }else if (xmlStrCmp(opts[1], "true") == 0) {
                        pasedBoolOption = true;
                        optValue = 1;
                    }else if (xmlStrCmp(opts[1], "false") == 0) {
                        pasedBoolOption = true;
                        optValue = 0;
                    }
                    /* handle setting integer option */
                    if (!pasedBoolOption && !invertOption &&
                            ((optionCount == 2) && !sscanf((char *) opts[1], "%ld", &optValue))) {
                        xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as an option value.\n").arg(xsldbgText(opts[1])));
                        result = false;
                    } else {
                        if (invertOption)
                            optValue = !optValue;
                        result = optionsSetIntOption(OptionTypeEnum(optID), optValue);
                    }

                    switch (optID) {

                        case OPTIONS_TRACE:
                        case OPTIONS_WALK_SPEED:
                        case OPTIONS_GDB:
                            break;

                        default:
                            xsldbgGenericErrorFunc(QObject::tr("Information: The new value for option \'%1\' will not be applied until the run command is used.\n").arg(xsldbgText(opts[0])));
                            break;
                    }
                } else {
                    /* handle setting a string option */
                    result = optionsSetStringOption(OptionTypeEnum(optID), xsldbgText(opts[1]));

                }
            } else {
                xsldbgGenericErrorFunc(QObject::tr("Error: Unknown option name %1.\n").arg(xsldbgText(opts[0])));
            }
        } else {
            xsldbgGenericErrorFunc(QObject::tr("Error: Missing arguments for the command %1.\n").arg(QString("setoption")));
        }
    } else {
	xsldbgGenericErrorFunc(QObject::tr("Error: Missing arguments for the command %1.\n").arg(QString("setoption")));
    }

    return result;
}



int xslDbgShellOptions(void)
{
    int result = 1;
    XsldbgSettingData item;

    /* Print out the integer options and thier values */
    if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN) {
        XsldbgSettingsModel::SettingsType optionType = XsldbgSettingsModel::AnySimpleSettingType;
        // retrieve a list of options sorted by name
        QStringList optionList = optionDataModel()->settingsList(optionType,
                XsldbgSettingsModel::SortByName);
        QString optionName;
        foreach (optionName, optionList){
            if (optionDataModel()->findSetting(optionName, optionType, item)) {
                switch (item.m_type) {
                    case XsldbgSettingsModel::IntSettingType:
                    case XsldbgSettingsModel::BoolSettingType:
                        xsldbgGenericErrorFunc(QObject::tr("Option %1 = %2\n").arg(item.m_name).arg(item.m_value.toString()));
                        break;

                    case XsldbgSettingsModel::StringSettingType:
                        if (item.m_value.isValid()){
                            xsldbgGenericErrorFunc(QObject::tr("Option %1 = \"%2\"\n").arg(item.m_name).arg(item.m_value.toString()));
                        } else {
                            xsldbgGenericErrorFunc(QObject::tr("Option %1 = \"\"\n").arg(item.m_name));
                        }
                        break;

                    default:
                        if (!(item.m_type & XsldbgSettingsModel::HiddenSettingType)){
                            // not a hidden option, and not handled in cases above
                            qWarning("Option name %s has unexpected option type %d",
                                    item.m_name.toUtf8().constData(), item.m_type);
                            result = 0;
                        }
                }
            }else {
                qWarning("xslDbgShellOptions option not found %s",
                        optionName.toLatin1().constData());
            }
        }
        xsldbgGenericErrorFunc("\n");
    } else {
        // nothing to do here as the thread will be monitoring the model
    }

    return result;
}


int xslDbgShellShowWatches(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctx,int showWarnings)
{
  int result = 0, counter;  
  xmlChar* watchExpression;  
  if ((showWarnings == 1) && (arrayListCount(optionsGetWatchList()) == 0)){
    xsldbgGenericErrorFunc(QObject::tr("\tNo expression watches set.\n"));
  }
  for ( counter = 0; 
	counter < arrayListCount(optionsGetWatchList()); 
	counter++){
    watchExpression = (xmlChar*)arrayListGet(optionsGetWatchList(), counter);
    if (watchExpression){
      xsldbgGenericErrorFunc(QObject::tr(" WatchExpression %1 ").arg( counter + 1));
      result = xslDbgShellCat(styleCtxt, ctx, watchExpression);
    }else
      break;
  }

  return result;
}


int xslDbgShellAddWatch(xmlChar* arg)
{
  int result = 0;
  if (arg){
    trimString(arg);
    result = optionsAddWatch(arg);
    if (!result)
      xsldbgGenericErrorFunc(QObject::tr("Error: Unable to add watch expression \"%1\". It already has been added or it cannot be watched.\n").arg(xsldbgText(arg)));
  }
  return result;
}


int xslDbgShellDeleteWatch(xmlChar* arg)
{
  int result = 0;
  long watchID;
  if (arg){
    trimString(arg);
    if (arg[0] == '*') {
      arrayListEmpty(optionsGetWatchList());
    }else if ((xmlStrlen(arg) == 0) || 
	      !sscanf((char *) arg, "%ld", &watchID)) {
      xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse %1 as a watchID.\n").arg(xsldbgText(arg)));
      return result;
    } else {
      result = optionsRemoveWatch(watchID);
      if (!result)
	xsldbgGenericErrorFunc(QObject::tr("Error: Watch expression %1 does not exist.\n").arg(watchID));
    }
  }
  return result;
}



int xsldbgWriteConfig(xmlChar * arg)
{
    Q_UNUSED(arg);
    int result = 0;
    QSettings settings;
    if (optionsWriteConfig(settings)){
        result = 1;
    }
    return result;
}


int xsldbgReadConfig(xmlChar * arg)
{
    Q_UNUSED(arg);
    int result = 0;
    QSettings settings;
    if (optionsReadConfig(settings)){
        result = 1;
    }
    return result;
}
