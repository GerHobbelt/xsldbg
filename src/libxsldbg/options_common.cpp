/**
 *
 *  This file is part of the kdewebdev package
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

#include <libxsldbg/options.h>
#include <libxsldbg/utils.h>
#include <QStringList>
#include <QFileInfo>
#include <QCoreApplication>

static XsldbgSettingsModel *settingsModel = NULL;
// the model that will apply after the run command
static XsldbgSettingsModel *newSettingsModel = NULL;


/* the names for our options
   Items that start with *_ are options that CANNOT be used by the user
  Once you set an option you need to give a run command to activate
  new settings */
const char *optionNames[] = {
    "xinclude",                 /* Use xinclude during xml parsing */
    "docbook",                  /* Use of docbook sgml parsing */
    "timing",                   /* Use of timing */
    "profile",                  /* Use of profiling */
    "valid",                    /* Enable file validation */
    "net",                      /* Enable the network entity loader */
    "out",                      /* Enable output to stdout */
    "html",                     /* Enable the use of html parsing */
    "debug",                    /* Enable the use of xml tree debugging */
    "shell",                    /* Enable the use of debugger shell */
    "preferhtml",               /* Prefer html output for search results */
    "autoencode",               /* Try to use the encoding from the stylesheet */
    "utf8input",                /* All input from "user" will be in UTF-8 */
    "stdout",                   /* Print all error messages to  stdout, 
				 * normally error messages go to stderr */
    "autorestart",		/* When finishing the debug of a XSLT script 
				   automatically restart at the beginning */
    "catalogs",                 /* do we use catalogs in SGML_CATALOG_FILES */
    "autoloadconfig",           /* automatically load configuration */
    "verbose",                  /* Be verbose with messages */
    "gdb",                      /* Run in gdb modem prints more messages */
    "repeat",                   /* The number of times to repeat */
    "*_trace_*",                /* Trace execution */
    "*_walkspeed_*",            /* How fast do we walk through code */
    "output",                   /* what is the output file name */
    "source",                   /* The stylesheet source to use */
    "data",                     /* The xml data file to use */
    "docspath",                 /* Path of xsldbg's documentation */
    "catalognames",             /* The names of the catalogs to use when the catalogs option is active */
    "encoding",                 /* What encoding to use for standard output */
    "searchresultspath",        /* Where do we store the results of search */
    "cwd",                      /* what directory was changed into during excecution */
    "comment",                  /* the comment related to the current session */
    NULL                        /* indicate end of list */
};


QString optionsGetOptionName(OptionTypeEnum ID)
{
    QString result;
    if ( (ID >= OPTIONS_FIRST_OPTIONID) && (ID <= OPTIONS_LAST_OPTIONID)){
	/* An option ID is always valid at the moment */
	result = optionNames[ID - OPTIONS_FIRST_OPTIONID];
    }

    return result;
}


int optionsGetOptionID(QString optionName)
{
    int result = -1;
    int optID = lookupName(optionName, optionNames);

    if (optID >= 0) {
        result = optID + OPTIONS_FIRST_OPTIONID;
    }

    return result;
}


// find the help documentation directory

QString langLookupDir( const QString &fname )
{
    QStringList search;

    // assemble the local search paths

	// first try looking relative to the binaries install directory
	if (QCoreApplication::libraryPaths().count()){
		  QString newPath =  QCoreApplication::libraryPaths().first() + "/../docs/en";
  	      search.append(newPath);
	}

#ifdef DOCS_PATH
    search.append(DOCS_PATH);
#endif
    QString docsEnv = getenv("DOCS_PATH");
    if (!docsEnv.isEmpty()){
	search.append(docsEnv);
	search.append(docsEnv + "/docs/en");
    }
 
    // also look in each of the KDEDIR path
    docsEnv = getenv("KDEDIR");
    if (!docsEnv.isEmpty()){
	search.append(docsEnv);
	search.append(docsEnv + "/docs/en");
    }

	// lastly try in QTDIR
    docsEnv = getenv("QTDIR");
    if (!docsEnv.isEmpty()){
	search.append(docsEnv);
	search.append(docsEnv + "/docs/en");
    }


    // try to locate the file
	QStringList::Iterator it;
    for (it = search.begin(); it != search.end(); ++it)
    {	
	QString baseDir = (*it);
	QFileInfo info(baseDir + '/' + fname);
	if (info.exists() && info.isFile() && info.isReadable())
	  return baseDir;
    }

    return QString();
}


void optionsSetDataModel(XsldbgSettingsModel *newModel)
{
    newSettingsModel = newModel;
}

void optionsApplyNewDataModel()
{
    if (newSettingsModel) {
        settingsModel = newSettingsModel;
        newSettingsModel = 0;
    }
}

XsldbgSettingsModel * optionDataModel()
{
    if (!settingsModel)
        qWarning("No XsldbgSettingsModel set");

    Q_ASSERT(settingsModel);
    return settingsModel;
}

static bool aConfig = true;
void optionSetAutoConfig(bool value)
{
    aConfig = value;
}


bool optionsAutoConfig()
{
    return aConfig;
}

