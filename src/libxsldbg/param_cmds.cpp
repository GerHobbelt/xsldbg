
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
#include "debugXSL.h"
#include "options.h"

#include "xsldbgmsg.h"
#include "xsldbgthread.h"       /* for get thread status */


/* -----------------------------------------

   libxslt parameter related commands

  ------------------------------------------- */

int xslDbgShellAddParam(xmlChar * arg)
{
    int result = 0;
    static const char *errorPrompt = I18N_NOOP("Failed to add parameter");
    xmlChar *opts[2];

    if (!arg) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for the command %1.\n").arg(QString("addparam")));
    }else{
	if ((xmlStrLen(arg) > 1) && splitString(arg, 2, opts) == 2) {
            if (optionDataModel()) {
                QString name = xsldbgText(opts[0]);
                QVariant value = xsldbgText(opts[1]);
                result = optionDataModel()->addParameter(name, value);
            }
	} else {
	    xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for the command %1.\n").arg(QString("addparam")));
	}
    }
    if (!result)
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
    else {
	xsldbgGenericErrorFunc("\n");
    }
    return result;
}


int xslDbgShellDelParam(xmlChar * arg)
{
    int result = 0;
    static const char *errorPrompt = I18N_NOOP("Failed to delete parameter");
    xmlChar *opts[2];

    if (!arg) {
	xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for the command %1.\n").arg(QString("delparam")));
    }else{
	if (xmlStrLen(arg) > 0) {
	    if (splitString(arg, 1, opts) == 1) {
                QString name = xsldbgText(opts[0]);
                if (optionDataModel()) {
                    result = optionDataModel()->removeParameter(name);
                    if (!result)
                        xsldbgGenericErrorFunc(QObject::tr("Error: Unable to find parameter %1.\n").arg(name));
		}
	    } else {
		xsldbgGenericErrorFunc(QObject::tr("Error: Invalid arguments for the command %1.\n").arg(QString("delparam")));
	    }
	} else {
            if (optionDataModel()) {
                optionDataModel()->removeAllParameters();
	        result = 1;
            }
	}
    }
    if (!result)
        xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
    else
         xsldbgGenericErrorFunc("\n");

    return result;
}


int xslDbgShellShowParam(xmlChar * arg)
{
    Q_UNUSED(arg);
    int result = 0;
    static const char *errorPrompt = I18N_NOOP("Unable to print parameters");

    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        // nothing to do here as the thread should be monitoring the settings model
        result = 1;
    } else {
        QStringList paramsList =
            optionDataModel()->settingsList(XsldbgSettingsModel::ParamSettingType,
                    XsldbgSettingsModel::SortById);
        QString param;
        XsldbgSettingData item;
        if (!paramsList.isEmpty()){
            xsldbgGenericErrorFunc(QObject::tr("\n"));
            foreach (param, paramsList){
                if (optionDataModel()->findSetting(param, XsldbgSettingsModel::ParamSettingType,  item))
                        xsldbgGenericErrorFunc(QObject::tr(" Parameter %1 %2=\"%3\"\n").arg(item.m_id).arg(item.m_name).arg(item.m_value.toString()));
            }
            result = 1;
        }else {
            xsldbgGenericErrorFunc(QObject::tr("\nNo parameters present.\n"));
            result = 1;
        }


        if (result != 1)
            xsldbgGenericErrorFunc(QString("Error: %1\n").arg(QObject::tr(errorPrompt)));
        xsldbgGenericErrorFunc("\n");
    }
    return result;
}
