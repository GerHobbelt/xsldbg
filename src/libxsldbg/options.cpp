
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


#include "options.h"
#include "xsldbg.h"
#include "xsldbgthread.h"
#include "arraylist.h"
#include "debugXSL.h"
#include "xsldbgmsg.h"
#include "utils.h"
#include "files.h"
#include <QDebug>

/* make use that use of options are safe by only copying
   critical values from intVolitleOptions just before stylesheet is started
 */
int intVolitileOptions[OPTIONS_LAST_INT_OPTIONID - OPTIONS_FIRST_INT_OPTIONID + 1];

xmlExternalEntityLoader xsldbgDefaultEntLoader;

/* what are the expressions to be printed out when xsldbg stops */
static arrayListPtr watchExpressionList;


int optionsInit(void)
{
    xsldbgDefaultEntLoader = xmlGetExternalEntityLoader(); 
    int optionId;

    for (optionId = 0;
         optionId <= OPTIONS_LAST_INT_OPTIONID - OPTIONS_FIRST_INT_OPTIONID; optionId++) {
        intVolitileOptions[optionId] = 0;
    }

    optionsSetDataModel(new XsldbgSettingsModel);
    optionsApplyNewDataModel();

    /* init our list of expressions to watch which are only a list of 
     strings ie xmlChar*'s  */
    watchExpressionList = arrayListNew(10, (freeItemFunc) xmlFree);


    return (optionDataModel() && watchExpressionList);
}


void optionsFree(void)
{
    if (optionsAutoConfig() && optionsGetIntOption(OPTIONS_AUTOLOADCONFIG)){
        xmlChar *profile=0;
        xsldbgWriteConfig(profile);
    }
    if (optionDataModel()) {
        delete optionDataModel();
        optionsSetDataModel(0);
    }

    /* Free up memory used by parameters and watches*/
    arrayListFree(watchExpressionList);
    watchExpressionList = NULL;
}



int optionsSetIntOption(OptionTypeEnum optionType, int value)
{
    int type = optionType, result = 1;

    if ((type >= OPTIONS_FIRST_INT_OPTIONID) && (type <= OPTIONS_LAST_INT_OPTIONID)) {
        /* make sure that use of options are safe by only copying
         * critical values from intVolitleOptions just before 
         * stylesheet is started
         */
        QVariant qValue = value;
        if (!optionDataModel()->updateSetting(type, qValue)){
            qWarning("Unable to update setting %d", type);
            return 0;
        }

        /* the following types must be activated imediately */
        switch (type) {

            case OPTIONS_TRACE:
            case OPTIONS_WALK_SPEED:
            case OPTIONS_GDB:
                intVolitileOptions[type - OPTIONS_FIRST_INT_OPTIONID] = value;
                break;

            default:
                break;
        }
    } else {
	if ((type >= OPTIONS_FIRST_OPTIONID) && (type <= OPTIONS_LAST_OPTIONID)){
	     xsldbgGenericErrorFunc(QObject::tr("Error: Option %1 is not a valid boolean/integer option.\n").arg(type));
	}else{
#ifdef WITH_XSLDBG_DEBUG_PROCESS
	    xsldbgGenericErrorFunc(QString("Error: Invalid arguments for the command %1.\n").arg("setoption"));
#endif
	}
        result = 0;
    }
    return result;
}


int optionsGetIntOption(OptionTypeEnum optionType)
{
    int type = optionType, result = 0;

    if ((type >= OPTIONS_FIRST_INT_OPTIONID) && (type <= OPTIONS_LAST_INT_OPTIONID)) {
        XsldbgSettingData item;
        if (optionDataModel()->findSetting(type, item)){
            result = item.m_value.toInt();
        }else {
            qWarning("Unable to get setting %d", type);
        }
    } else {
	if ((type >= OPTIONS_FIRST_OPTIONID) && (type <= OPTIONS_LAST_OPTIONID)){
	    xsldbgGenericErrorFunc(QObject::tr("Error: Option %1 is not a valid boolean/integer option.\n").arg(type));
	}else{
#ifdef WITH_XSLDBG_DEBUG_PROCESS
	    xsldbgGenericErrorFunc(QString("Error: Invalid arguments for the command %1.\n").arg("options"));
#endif
	}
    }
    return result;
}



int optionsSetStringOption(OptionTypeEnum optionType, QString value)
{
    int type = optionType, result = 0;

    if ((type >= OPTIONS_FIRST_STRING_OPTIONID) &&
        (type <= OPTIONS_LAST_STRING_OPTIONID)) {
        QVariant qValue(value);
        if (!optionDataModel()->updateSetting(type, qValue))
            qWarning("Unable to update setting %d", type);
    } else{
	if ((type >= OPTIONS_FIRST_OPTIONID) && (type <= OPTIONS_LAST_OPTIONID)){
	    xsldbgGenericErrorFunc(QObject::tr("Error: Option %1 is not a valid string xsldbg option.\n").arg(type));
	}else{
#ifdef WITH_XSLDBG_DEBUG_PROCESS
	    xsldbgGenericErrorFunc(QString("Error: Invalid arguments for the command %1.\n").arg("setoption"));
#endif
	}
    }
    return result;
}


const QString optionsGetStringOption(OptionTypeEnum optionType)
{
    int type = optionType;
    QString result;

    if ((type >= OPTIONS_FIRST_STRING_OPTIONID) &&
        (type <= OPTIONS_LAST_STRING_OPTIONID)) {
        XsldbgSettingData item;
        if (optionDataModel()->findSetting(type, item)){
            result = item.m_value.toString();
        }else {
            qWarning("Unable to get setting %d", type);
        }
    } else{
	if ((type >= OPTIONS_FIRST_OPTIONID) && (type <= OPTIONS_LAST_OPTIONID)){
	    xsldbgGenericErrorFunc(QObject::tr("Error: Option %1 is not a valid string xsldbg option.\n").arg(type));
	}else{
#ifdef WITH_XSLDBG_DEBUG_PROCESS
	    xsldbgGenericErrorFunc(QString("Error: Invalid arguments for the command %1.\n").arg("options"));
#endif
	}
    }
    return result;
}


void optionsCopyVolitleOptions(void)
{
    int optionId;
    XsldbgSettingData item;

    optionsApplyNewDataModel();
    optionDataModel();

    for (optionId = 0;
         optionId <= OPTIONS_LAST_INT_OPTIONID - OPTIONS_FIRST_INT_OPTIONID; optionId++) {
        if (optionDataModel()->findSetting(optionId + OPTIONS_FIRST_INT_OPTIONID, item)) {
            intVolitileOptions[optionId] = item.m_value.toInt();
        }else {
            qWarning("optionsCopyVolitleOptions option not found %d", optionId);
        }
    }
}


int optionsAddWatch(const xmlChar* xPath)
{
  int result = 0;
  if (!xPath || (xmlStrlen(xPath) == 0)){
#ifdef WITH_XSLDBG_DEBUG_PROCESS
           xsltGenericError(xsltGenericErrorContext,
			    "Error: Invalid XPath supplied\n");
#endif
  } else{
    if (optionsGetWatchID(xPath) == 0){
      xmlChar * nameCopy = xmlStrdup(xPath);
      if (nameCopy){
	arrayListAdd(watchExpressionList, nameCopy);
	result = 1;
      }
    }
  }

  return result;
}


int optionsGetWatchID(const xmlChar* xPath)
{
  int result = 0, counter;
  xmlChar* watchExpression;
  if (!xPath){
#ifdef WITH_XSLDBG_DEBUG_PROCESS
           xsltGenericError(xsltGenericErrorContext,
		       "Error: Invalid xPath supplied\n");
#endif
  } else{
    for ( counter = 0; 
	  counter < arrayListCount(watchExpressionList); 
				   counter++){
      watchExpression = (xmlChar*)arrayListGet(watchExpressionList, counter);
      if (watchExpression){
	if (xmlStrEqual(xPath, watchExpression)){
	  result = counter + 1;
	  break;
	}
      }else{
	break;
      }
    }	    
  }
 
 return result;
}

int optionsRemoveWatch(int watchID)
{
  return arrayListDelete(watchExpressionList, watchID - 1);
}

  arrayListPtr optionsGetWatchList()
{
  return watchExpressionList;
}

bool optionsReadConfig(const QSettings &config)
{
    bool result = false;
/*
    if (optionDataModel())
        result = optionDataModel()->loadSettings(config);
*/

    return result;
}

// assume that the correct grup has already been selected
bool optionsWriteConfig(QSettings &config)
{
    bool result = false;
/*
    if (optionDataModel())
        result = optionDataModel()->saveSettings(config);
*/

    return result;
}
