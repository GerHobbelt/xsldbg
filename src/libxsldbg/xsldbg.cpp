
/***************************************************************************
              xsldbg.cpp  - entry point to main xsldbg features
                             -------------------
    begin                : Mon May 2 2016
    copyright            : (C) 2016 by Keith Isdale
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


/*
 * Based on file xsltproc.c
 *
 * by  Daniel Veillard
 *     daniel@veillard.com
 *
 *  xsltproc.c is part of libxslt
 *
 *
 */

#define WITH_XSLDBG_DEBUG_PROCESS
/* Justin's port version - do not merge into core!!! */
#define RISCOS_PORT_VERSION "2.01"

#include "xsldbg.h"
#include <QCoreApplication>
#include <QDateTime>
#include <QUrl>
#include <QStringList>
#include "debug.h"
#include "options.h"
#include "utils.h"
#include "files.h"
#include "breakpoint.h"
#include "debugXSL.h"
//##TODO
//#include "version.h"

#include <libxml/xmlerror.h>
#include "xsldbgmsg.h"
#include "xsldbgthread.h"       /* for getThreadStatus */
#ifdef HAVE_READLINE
#  include <readline/readline.h>
#  ifdef HAVE_HISTORY
#     include <readline/history.h>
#   endif
#endif

/* need to setup catch of SIGINT */
#include <signal.h>


/* needed by printTemplateNames */
#include <libxslt/transform.h>
#include <libxml/uri.h>

/* standard includes from xsltproc*/
#include <libxslt/xslt.h>
#include <libexslt/exslt.h>

#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/xmlerror.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#ifdef LIBXML_XINCLUDE_ENABLED
#include <libxml/xinclude.h>
#endif

#include <libxml/catalog.h>

#include <libxslt/xslt.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxslt/extensions.h>
#include <libexslt/exsltconfig.h>

#include <QFile>

#ifdef __riscos

/* Global definition of our program name on invocation.
   This is required such that we can invoke ourselves for processing
   search or help messages where our executable does not exist on the
   current run path */
char *xsldbgCommand = NULL;
#endif

static xmlChar *output = NULL;
xmlSAXHandler mySAXhdlr;

FILE *errorFile = NULL; /* we'll set this just before starting debugger */

xmlParserInputPtr xmlNoNetExternalEntityLoader(const char *URL,
                                               const char *ID,
                                               xmlParserCtxtPtr ctxt);

/* -----------------------------------------
   Private function declarations for xsldbg.c
 -------------------------------------------*/

/**
 * Initialize xsldbg:
 *
 * @returns 1 if able to allocate memory needed by xsldbg
 *         0 otherwise
 */
int xsldbgInit(void);


/**
 * Free memory used by xsldbg
 */
void xsldbgFree(void);


/**
 * Recover from a signal(SIGINT), exit if needed
 *
 * @param value : not used
 */
void catchSigInt(int value);


/**
 * Clean up and exit
 *
 * @param value : not used
 */
void   catchSigTerm(int value);

/**
 * Handles print output from xsldbg and passes it to the application if
 *  running as a thread otherwise send to stderr
 *
 * @param ctx:  Is Valid
 * @param msg:  Is valid
 * ...:  other parameters to use
 *
 */
void   xsldbgGenericErrorFunc(void *ctx, const char *msg, ...);

xmlEntityPtr (*oldGetEntity)( void * user_data, const xmlChar * name);

static xmlEntityPtr xsldbgGetEntity( void * user_data, const xmlChar * name)
{
    xmlEntityPtr ent = NULL;
    if (oldGetEntity){
        ent =  (oldGetEntity)(user_data, name);
        if (ent)
            filesEntityRef(ent, ent->children, ent->last);
    }
    return ent;
}

/* ------------------------------------- 
   End private functions
   ---------------------------------------*/


/*
 * Internal timing routines to remove the necessity to have unix-specific
 * function calls
 */


static QTime startTime;


/*
 * startTimer: call where you want to start timing
 */
static void startTimer(void)
{
    startTime = QDateTime::currentDateTime().time();
}

/*
 * endTimer: call where you want to stop timing and to print out a
 *           message about the timing performed; format is a printf
 *           type argument
 */
static void endTimer(const QString& message)
{
    QTime endTime = QDateTime::currentDateTime().time();
    int msec = startTime.msecsTo(endTime);

    /* Display the time taken to complete this task */
    xsldbgGenericErrorFunc(QObject::tr("%1 took %2 ms to complete.\n").arg(message).arg(msec));
}


static void xsltProcess(xmlDocPtr doc, xsltStylesheetPtr cur)
{

    xmlDocPtr res;
    const char *params[(MAXPARAM_COUNT +1) * 2];
    int bytesWritten = -1;
    int nbparams = 0;
    QByteArray paramArray[(MAXPARAM_COUNT +1) *2];

    Q_ASSERT(optionDataModel());
    /* Copy the parameters across for libxslt */
    QStringList paramsList = optionDataModel()->settingsList(XsldbgSettingsModel::ParamSettingType);
    QString param;
    XsldbgSettingData item;
    foreach (param, paramsList) {
        if (nbparams >= MAXPARAM_COUNT){
            xsldbgGenericErrorFunc(QObject::tr("Warning: Too many libxslt parameters provided.\n"));
            break;
        }
        if (optionDataModel()->findSetting(param, XsldbgSettingsModel::ParamSettingType,  item)){
            paramArray[nbparams].append(item.m_name.toUtf8().constData());
            paramArray[nbparams + 1].append(item.m_value.toString().toUtf8().constData());
            params[nbparams] = paramArray[nbparams].constData();
            params[nbparams + 1] = paramArray[nbparams +1].constData();
            nbparams += 2;
        }
    }
    params[nbparams] = NULL;

#ifdef LIBXML_XINCLUDE_ENABLED
    if (optionsGetIntOption(OPTIONS_XINCLUDE)) {
        if (optionsGetIntOption(OPTIONS_TIMING))
            startTimer();
        xmlXIncludeProcess(doc);
        if (optionsGetIntOption(OPTIONS_TIMING)) {
            /* Display the time taken to do XInclude processing */
            endTimer(QObject::tr("XInclude processing %1.").arg(optionsGetStringOption(OPTIONS_DATA_FILE_NAME)));
        }
    }
#endif
    QString outFileName(optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME));
    QByteArray outFile;
    if (outFileName.indexOf(":/") != -1) {
        //try to convert URI to local path
        QUrl outputUrl(outFileName);
        outFile = outputUrl.toLocalFile().toUtf8().constData();
    } else {
        outFile = outFileName.toUtf8().constData();
    }

    if (optionsGetIntOption(OPTIONS_TIMING) ||
            optionsGetIntOption(OPTIONS_PROFILING))
        startTimer();
    if ((optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME).isEmpty())
            || optionsGetIntOption(OPTIONS_SHELL)) {
        if (optionsGetIntOption(OPTIONS_REPEAT)) {
            int j;

            for (j = 1; j < optionsGetIntOption(OPTIONS_REPEAT); j++) {
                res = xsltApplyStylesheet(cur, doc, params);
                xmlFreeDoc(res);
                doc = xsldbgLoadXmlData();
            }
        }
        if (optionsGetIntOption(OPTIONS_PROFILING)) {
            if (terminalIO != NULL)
                res = xsltProfileStylesheet(cur, doc, params, terminalIO);
            else if ((optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME).isEmpty())
                     || (getThreadStatus() != XSLDBG_MSG_THREAD_RUN)
                     || (filesTempFileName(1).isEmpty()))
                res = xsltProfileStylesheet(cur, doc, params, stderr);
            else {
                /* We now have to output to using notify using
                 * temp file #1 */
                FILE *tempFile = fopen(filesTempFileName(1), "w");

                if (tempFile != NULL) {
                    res =
                            xsltProfileStylesheet(cur, doc, params, tempFile);
                    fclose(tempFile);
                    /* send the data to application */
                    notifyXsldbgApp(XSLDBG_MSG_FILEOUT,
                                    filesTempFileName(1));
                } else {
                    xsldbgGenericErrorFunc(QObject::tr("Error: Unable to write temporary results to %1.\n").arg(filesTempFileName(1).constData()));
                    res = xsltProfileStylesheet(cur, doc, params, stderr);
                }
            }
        } else {
            res = xsltApplyStylesheet(cur, doc, params);
        }
        if (optionsGetIntOption(OPTIONS_PROFILING)) {
            if (optionsGetIntOption(OPTIONS_REPEAT))
                /* Display how long it took to apply stylesheet */
                endTimer(QObject::tr("Applying stylesheet %1 times").arg(optionsGetIntOption(OPTIONS_REPEAT)));
            else
                /* Display how long it took to apply stylesheet */
                endTimer(QObject::tr("Applying stylesheet"));
        }
        if (res == NULL) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
            xsltGenericError(xsltGenericErrorContext, "Error: Transformation did not complete writing to file %s\n",
                             outFile.constData());
#endif
            return;
        }
        if (!optionsGetIntOption(OPTIONS_OUT)) {
            xmlFreeDoc(res);
            return;
        }
#ifdef LIBXML_DEBUG_ENABLED
        if (optionsGetIntOption(OPTIONS_DEBUG)) {
            if (xslDebugStatus != DEBUG_RUN_RESTART){
                if (terminalIO != NULL)
                    xmlDebugDumpDocument(terminalIO, res);
                else if ((optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME).isEmpty())
                         || (getThreadStatus() != XSLDBG_MSG_THREAD_RUN)
                         || (filesTempFileName(1).isEmpty()))
                    xmlDebugDumpDocument(stdout, res);
                else {
                    FILE *tempFile = fopen(filesTempFileName(1), "w");

                    if (tempFile) {
                        bytesWritten = 0; // flag that we have writen at least zero bytes
                        xmlDebugDumpDocument(tempFile, res);
                        fclose(tempFile);
                        /* send the data to application */
                        notifyXsldbgApp(XSLDBG_MSG_FILEOUT,
                                        filesTempFileName(1));
                    } else {
                        xsldbgGenericErrorFunc(QObject::tr("Error: Unable to write temporary results to %1.\n").arg(filesTempFileName(1).constData()));
                        xmlDebugDumpDocument(stdout, res);
                    }

                }
            }
        } else {
#endif
            if (xslDebugStatus != DEBUG_RUN_RESTART){
                if (cur->methodURI == NULL) {
                    if (optionsGetIntOption(OPTIONS_TIMING))
                        startTimer();
                    if (xslDebugStatus != DEBUG_QUIT) {
                        if (terminalIO != NULL)
                            bytesWritten = xsltSaveResultToFile(terminalIO, res, cur);
                        else if (optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME).isEmpty())
                            bytesWritten = xsltSaveResultToFile(stdout, res, cur);
                        else{
                            bytesWritten =
                                    xsltSaveResultToFilename(outFile.constData(), res, cur, 0);
                        }
                    }
                    if (optionsGetIntOption(OPTIONS_TIMING))
                        /* Indicate how long it took to save to file */
                        endTimer(QObject::tr("Saving result"));
                } else {
                    if (xmlStrEqual(cur->method, (const xmlChar *) "xhtml")) {
                        xsldbgGenericErrorFunc(QObject::tr("Warning: Generating non-standard output XHTML.\n"));
                        if (optionsGetIntOption(OPTIONS_TIMING))
                            startTimer();
                        if (terminalIO != NULL){
                            bytesWritten = xsltSaveResultToFile(terminalIO, res, cur);
                        }else if (optionsGetStringOption(OPTIONS_OUTPUT_FILE_NAME).isEmpty()){
                            bytesWritten = xsltSaveResultToFile(stdout, res, cur);
                        }else{
                            bytesWritten = xsltSaveResultToFilename(outFile.constData(), res, cur, 0);
                        }
                        if (optionsGetIntOption(OPTIONS_TIMING))
                            /* Indicate how long it took to save to file */
                            endTimer(QObject::tr("Saving result"));
                    } else {
                        xsldbgGenericErrorFunc(QObject::tr("Warning: Unsupported, non-standard output method %1.\n").arg(xsldbgText(cur->method)));
                    }
                }
            }
#ifdef LIBXML_DEBUG_ENABLED
        }
#endif

        xmlFreeDoc(res);
    } else {
        xsltTransformContextPtr userCtxt = xsltNewTransformContext(cur, doc);
        if (userCtxt){
            bytesWritten = xsltRunStylesheetUser(cur, doc, params, outFile.constData(),
                                                 NULL, NULL, NULL, userCtxt);
            if (optionsGetIntOption(OPTIONS_TIMING))
                endTimer(QObject::tr("Running stylesheet and saving result"));
            xsltFreeTransformContext(userCtxt);
        }else{
            xsldbgGenericErrorFunc(QObject::tr("Error: Out of memory.\n"));
        }
    }
    if (((xslDebugStatus != DEBUG_RUN_RESTART) && (xslDebugStatus != DEBUG_QUIT)) && (bytesWritten == -1))
        xsldbgGenericErrorFunc(QObject::tr("Error: Unable to save results of transformation to file %1.\n").arg(outFile.constData()));

}

static void
usage(const char *name)
{
    xsltGenericError(xsltGenericErrorContext,
                     "Usage: %s [options] stylesheet file. See documenation of source and data commands\n", name);
    xsltGenericError(xsltGenericErrorContext,
                     "Without any parameters xsldbg starts in command mode, ready "
                     "for the source and data to be selected\n");
    xsltGenericError(xsltGenericErrorContext, "   Options:\n");
    /* Options help format is --<option>: <description>
     * or --<option> <para>: <description> */
    xsltGenericError(xsltGenericErrorContext,
                     "      --output file or -o file: Save to a given file. " \
                     "See output command documentatation\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --version or -V : Show the version of libxml and libxslt used\n");
    xsltGenericError(xsltGenericErrorContext,
                     "For documentation on the folowing \"flags\" " \
                     "see the documentation of the setoption command\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --verbose or -v : Show logs of what's happening\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --timing: Display the time used\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --repeat : Run the transformation 20 times\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --debug: Dump the tree of the result instead\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --novalid : Skip the DTD loading phase\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --noout : Do not dump the result\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --maxdepth val : Increase the maximum depth\n");
#ifdef LIBXML_HTML_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --html : The input document is(are) an HTML file(s)\n");
#endif
    xsltGenericError(xsltGenericErrorContext,
                     "      --param name value : Pass a (parameter,value) pair\n");
    xsltGenericError(xsltGenericErrorContext,
                     "                           string values must be quoted like \"'string'\"\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --param name:value : Pass a (parameter,value) pair\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --nonet : Refuse to fetch DTDs or entities over network\n");
#ifdef LIBXML_CATALOG_ENABLED
#ifdef __riscos
    xsltGenericError(xsltGenericErrorContext,
                     "      --catalogs : Use the catalogs from SGML$CatalogFiles\n");
#else
    xsltGenericError(xsltGenericErrorContext,
                     "      --catalogs : Use the catalogs from $SGML_CATALOG_FILES\n");
#endif
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --xinclude : do XInclude processing on document intput\n");
#endif
    xsltGenericError(xsltGenericErrorContext,
                     "      --profile or --norman : dump profiling informations \n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --shell|--noshell : Always|never go to the xsldbg prompt, to start debugging\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --noautoloadconfig : Disable automatic loading of last save xsldbg settings\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --gdb : Run in gdb mode printing out more information\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --autoencode : Detect and use encodings in the stylesheet\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --utf8input : Treat command line input as encoded in UTF-8\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --preferhtml : Use html output when generating search reports.\n"
                     "                     See search command\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --stdout : Print all error messages to stdout\n" \
                     "                 normally error messages go to stderr\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --cd <PATH> : Change to specfied working directory\n");


}

int xsldbgMain(int argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);
    int i=0, result = 1, noFilesFound = 0;
    xsltStylesheetPtr cur = NULL;
    QString expandedName;      /* contains file name with path expansion if any */

    /* in some cases we always want to bring up a command prompt */
    int showPrompt;

    /* the xml document we're processing */
    xmlDocPtr doc;
    
    errorFile = stderr;

#ifdef __riscos
    /* Remember our invocation command such that we may call ourselves */
    xsldbgCommand = argv[0];
#endif

    xmlInitMemory();


    LIBXML_TEST_VERSION xmlLineNumbersDefault(1);

    if (!xsldbgInit()) {
        qWarning("Init failed");
        xsldbgGenericErrorFunc(QObject::tr("Fatal error: Aborting debugger due to an unrecoverable error.\n"));
        xsldbgFree();
        xsltCleanupGlobals();
        xmlCleanupParser();
        xmlMemoryDump();
        return (1);
    }


    if (argc >1){
        // Do a quick scan to see if -*autoloadconfig is supplied
        for (i = 1; i < argc; i++) {
            if ((argv[i][0] == '-') && (argv[i][1] == '-'))
                argv[i]++;          /* treat --<OPTION_NAME> as -<OPTION_NAME> */

            if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-noautoloadconfig"))
                optionSetAutoConfig(false);
            else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-autoloadconfig"))
                optionSetAutoConfig(true);
        }
    }
    if (optionsAutoConfig()){
        xmlChar *profile=0;
        xsldbgReadConfig(profile);
    }
    if (argc >1){
        for (i = 1; i < argc; i++) {
            if (!result)
                break;

            if (argv[i][0] != '-') {
                expandedName = filesExpandName(argv[i]);
                if (expandedName.isEmpty()) {
                    xsldbgGenericErrorFunc(QObject::tr("Error: Unable to determine expanded file name for %1\n").arg(argv[i]));
                    result = 0;
                    break;
                }
                switch (noFilesFound) {
                case 0:
                    optionsSetStringOption(OPTIONS_SOURCE_FILE_NAME, expandedName);
                    noFilesFound++;
                    break;
                case 1:
                    optionsSetStringOption(OPTIONS_DATA_FILE_NAME, expandedName);
                    noFilesFound++;
                    break;

                default:
                    xsldbgGenericErrorFunc(QObject::tr("Error: Too many file names supplied via command line looking at option '%1'.\n").arg(argv[i]));
                    result = 0;
                }
                continue;
            }

#ifdef LIBXML_DEBUG_ENABLED
            if (!xmlStrCmp(argv[i], "-debug")) {
                if (result) {
                    result = optionsSetIntOption(OPTIONS_DEBUG, 1);
                    argv[i] = NULL;
                }
            } else
#endif
                if ((!xmlStrCmp(argv[i], "-v")) || (!xmlStrCmp(argv[i], "-verbose"))) {
                    xsltSetGenericDebugFunc(stderr, NULL);
                } else if ((xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-o")) ||
                           (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-output"))) {
                    argv[i] = NULL;
                    i++;
#if defined(WIN32) || defined (__CYGWIN__)
                    output = xmlCanonicPath((xmlChar*)argv[i]);
                    if (output){
                        result = xslDbgShellOutput(output);
                        xmlFree(output);
                    }
                    else
#endif
                        result = xslDbgShellOutput((xmlChar*)argv[i]);
                    argv[i] = NULL;
                } else if ((xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-V")) ||
                           (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-version"))) {
                    xsltGenericError(xsltGenericErrorContext,
                                     " xsldbg created by Keith Isdale <keithisdale@gmail.com\n");
                    xsltGenericError(xsltGenericErrorContext,
                                     " Version %s, Date created %s\n", XSLDBG_VERSION,
                                     TIMESTAMP);
                    xsltGenericError(xsltGenericErrorContext,
                                     "Using libxml %s, libxslt %s and libexslt %s\n",
                                     xmlParserVersion, xsltEngineVersion,
                                     exsltLibraryVersion);
                    xsltGenericError(xsltGenericErrorContext,
                                     "xsldbg was compiled against libxml %d, libxslt %d and libexslt %d\n",
                                     LIBXML_VERSION, LIBXSLT_VERSION,
                                     LIBEXSLT_VERSION);
                    xsltGenericError(xsltGenericErrorContext,
                                     "libxslt %d was compiled against libxml %d\n",
                                     xsltLibxsltVersion, xsltLibxmlVersion);
                    xsltGenericError(xsltGenericErrorContext,
                                     "libexslt %d was compiled against libxml %d\n",
                                     exsltLibexsltVersion, exsltLibxmlVersion);
                    argv[i] = NULL;
                } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-norman")) {
                    if (result) {
                        result = optionsSetIntOption(OPTIONS_PROFILING, 1);
                        argv[i] = NULL;
                    }
                } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-nonet")) {
                    xmlSetExternalEntityLoader(xmlNoNetExternalEntityLoader);
                } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-param")) {
                    argv[i] = NULL;
                    i++;
                    // support name:value as well
                    QRegExp reg("^(\\w+|[_]+):(.+)");
                    if ((argv[i][0] == '-') || (i >= argc)){
                        xsltGenericError(xsltGenericErrorContext,
                                         "Missing value to -param option");
                        return 1;
                    }
                    if (reg.exactMatch(argv[i]) && (reg.capturedTexts().count() == 3)){
                        optionDataModel()->addParameter(reg.capturedTexts()[1], reg.capturedTexts()[2]);
                        argv[i] = NULL;
                    }else{
                        if ((i + 1 < argc) && (argv[i +1][0] != '-')){
                            optionDataModel()->addParameter(argv[i], argv[i + 1]);
                            argv[i] = NULL;
                            argv[i+1] = NULL;
                            i++;
                        }else{
                            xsltGenericError(xsltGenericErrorContext,
                                             "Missing value to -param option");
                            return 1;
                        }
                    }
                    //##TODO check if too many parameters were added
                    /*
                if (arrayListCount(optionsGetParamItemList()) >= 32) {
                    xsltGenericError(xsltGenericErrorContext,
                            "Too many params\n");
                    return (1);
                }
                */
                } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-maxdepth")) {
                    int value;

                    argv[i] = NULL;
                    i++;
                    if (sscanf(argv[i], "%d", &value) == 1) {
                        if (value > 0)
                            xsltMaxDepth = value;
                    }
                    argv[i] = NULL;

                } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-repeat")) {
                    if (optionsGetIntOption(OPTIONS_REPEAT) == 0)
                        optionsSetIntOption(OPTIONS_REPEAT, 20);
                    else
                        optionsSetIntOption(OPTIONS_REPEAT, 100);
                    argv[i] = NULL;
                } else if (xmlStrEqual((xmlChar*)argv[i], (xmlChar*)"-cd")) {
                    argv[i] = NULL;
                    if (i + 1 < argc) {
                        i++;
                        result = changeDir(argv[i]);
                        argv[i] = NULL;
                    } else {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Missing path name after -cd option\n");
                    }

                } else {
                    /* From here we're only dealing with integer options */
                    /* ignore any non-user option */
                    if (result && (argv[i][2] != '*') ) {
                        // allow the user to turn off an option by going --no<IntOptionName>
                        bool disableOption = false;
                        if ((argv[i][1] == 'n') && (argv[i][2]=='o')){
                            disableOption = true;
                            argv[i]+=2;
                        }
                        int optID = optionsGetOptionID((argv[i]+1));

                        /* the user might have entered a string option so reject it if so */
                        if ((optID >= OPTIONS_FIRST_INT_OPTIONID)
                                && (optID <= OPTIONS_LAST_INT_OPTIONID)) {
                            if (!disableOption)
                                result = optionsSetIntOption(OptionTypeEnum(optID), optionsGetIntOption(OptionTypeEnum(optID)) + 1);
                            else
                                result = optionsSetIntOption(OptionTypeEnum(optID), 0);
                            argv[i] = NULL;
                        } else {
                            xsltGenericError(xsltGenericErrorContext,
                                             "Error: Unknown option %s, or unknown "
                                             " integer option. See setoption commmand in "
                                             "xsldbg documentation for full description of "
                                             "integer and string options\n",
                                             argv[i]);
                            result = 0;
                        }
                    }
                }
        }



        if (!result) {
            usage(argv[0]);
            xsldbgFree();
            return (1);
        }

        // No extra arguments go straight to the shell
        if (argc <= 1)
            result = optionsSetIntOption(OPTIONS_SHELL, 1);
    }else {
        // nothing to do here as caller of xsldbgMain must setup the options required
        // go straight to the shell
        result = optionsSetIntOption(OPTIONS_SHELL, 1);
    }

    if (getThreadStatus() != XSLDBG_MSG_THREAD_NOTUSED){
        result = optionsSetIntOption(OPTIONS_SHELL, 1);
    }
    /* copy the volitile options over to xsldbg */
    optionsCopyVolitleOptions();
    
    /*
     * shell interraction
     */
    if (!optionsGetIntOption(OPTIONS_SHELL)) {  /* excecute stylesheet (ie no debugging) */
        xslDebugStatus = DEBUG_NONE;
    } else {
        xslDebugStatus = DEBUG_STOP;
        xsltGenericError(xsltGenericErrorContext, "XSLDBG %s\n", XSLDBG_VERSION);
    }

    if (optionsGetIntOption(OPTIONS_VALID))
        xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    else
        xmlLoadExtDtdDefaultValue = 0;


    debugGotControl(0);
    while (xslDebugStatus != DEBUG_QUIT) {
        xsldbgReachedFirstTemplate = false;
        /* don't force xsldbg to show command prompt */
        showPrompt = 0;
        cur = NULL;
        doc = NULL;
        arrayListEmpty(filesEntityList());
        /* copy the volitile options over to xsldbg */
        optionsCopyVolitleOptions();

        // apply the chosen working directory to this session
        changeDir(optionsGetStringOption(OPTIONS_CWD));

        // use xinclude and net entity loader selection
        xsltSetXIncludeDefault(optionsGetIntOption(OPTIONS_XINCLUDE));
        /* enable/disable the no net entity loader as required */
        if (optionsGetIntOption(OPTIONS_NET))
            xmlSetExternalEntityLoader(xmlNoNetExternalEntityLoader);
        else
            xmlSetExternalEntityLoader(xsldbgDefaultEntLoader);

        /* choose where error messages/xsldbg output get sent to */
        if (optionsGetIntOption(OPTIONS_STDOUT))
            errorFile = stdout;
        else
            errorFile = stderr;

        /* enable/disable the no net entity loader as required */
        if (optionsGetIntOption(OPTIONS_NET))
            xmlSetExternalEntityLoader(xmlNoNetExternalEntityLoader);
        else
            xmlSetExternalEntityLoader(xsldbgDefaultEntLoader);

        filesLoadCatalogs();

        if (optionsGetIntOption(OPTIONS_SHELL)) {
            debugGotControl(0);

            xsldbgGenericErrorFunc(QObject::tr("\nStarting stylesheet\n\n"));
            if (optionsGetIntOption(OPTIONS_TRACE) == TRACE_OFF)
                xslDebugStatus = DEBUG_STOP;    /* stop as soon as possible */
        }

        if (optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME).isEmpty() ||
                optionsGetStringOption(OPTIONS_DATA_FILE_NAME).isEmpty()) {
            /* at least on file name has not been set */
            /*goto a xsldbg command prompt */
            showPrompt = 1;
            if (optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME).isEmpty())
                xsldbgGenericErrorFunc(QObject::tr("Error: No XSLT source file supplied.\n"));

            if (optionsGetStringOption(OPTIONS_DATA_FILE_NAME).isEmpty()) {
                xsldbgGenericErrorFunc(QObject::tr("Error: No XML data file supplied.\n"));
            }

        } else {
            filesLoadXmlFile(NULL, FILES_SOURCEFILE_TYPE);
            cur = filesGetStylesheet();
            if ((cur == NULL) || (cur->errors != 0)) {
                /*goto a xsldbg command prompt */
                showPrompt = 1;
                if (xslDebugStatus == DEBUG_NONE) {
                    xslDebugStatus = DEBUG_QUIT;        /* panic !! */
                    result = 0;
                }
            }
        }

        if (showPrompt == 0) {
            filesLoadXmlFile(NULL, FILES_XMLFILE_TYPE);
            doc = filesGetMainDoc();
            if (doc == NULL) {
                if (xslDebugStatus == DEBUG_NONE) {
                    xslDebugStatus = DEBUG_QUIT;        /* panic !! */
                    result = 0;
                } else {
                    /*goto a xsldbg command prompt */
                    showPrompt = 1;
                }
            } else {
                if (xslDebugStatus != DEBUG_QUIT) {
                    xsltProcess(doc, cur);
                    result = 1;
                }
            }

            if (optionsGetIntOption(OPTIONS_SHELL) && (showPrompt == 0)) {
                if ((xslDebugStatus != DEBUG_QUIT)
                        && !debugGotControl(-1)) {
                    xsldbgGenericErrorFunc(QObject::tr("\nDebugger never received control.\n"));
                    /*goto a xsldbg command prompt */
                    showPrompt = 1;
                    xslDebugStatus = DEBUG_STOP;
                    notifyXsldbgApp(XSLDBG_MSG_COMPLETED_TRANSFORMATION, NULL);
                } else {
                    xsldbgGenericErrorFunc(QObject::tr("\nFinished stylesheet\n\n"));
                    bool walking = optionsGetIntOption(OPTIONS_WALK_SPEED) != WALKSPEED_STOP;
                    bool tracing = optionsGetIntOption(OPTIONS_TRACE) != TRACE_OFF;
                    optionsSetIntOption(OPTIONS_WALK_SPEED, WALKSPEED_STOP);
                    optionsSetIntOption(OPTIONS_TRACE, TRACE_OFF);
                    notifyXsldbgApp(XSLDBG_MSG_COMPLETED_TRANSFORMATION, NULL);
                    if (!(walking || tracing) && !optionsGetIntOption(OPTIONS_AUTORESTART) && (xslDebugStatus != DEBUG_RUN_RESTART)){
                        /* pass control to user they won't be able to do much
               other than add breakpoints, quit, run, continue */
                        debugXSLBreak((xmlNodePtr) cur->doc, (xmlNodePtr) doc,
                                      NULL, NULL);
                    }
                }
            } else {
                /* request to execute stylesheet only  so we're done */
                xslDebugStatus = DEBUG_QUIT;
            }
        } else {
            /* Some sort of problem loading source file has occurred. Quit? */
            if (xslDebugStatus == DEBUG_NONE) {
                xslDebugStatus = DEBUG_QUIT;    /* Panic!! */
                result = 0;
            } else {
                /*goto a xsldbg command prompt */
                showPrompt = 1;
            }
        }

        if (showPrompt && optionsGetIntOption(OPTIONS_SHELL)) {
            xmlDocPtr tempDoc = xmlNewDoc((xmlChar *) "1.0");
            xmlNodePtr tempNode =
                    xmlNewNode(NULL, (xmlChar *) "xsldbg_default_node");
            if (!tempDoc || !tempNode) {
                xsldbgFree();
                return (1);
            }
            xmlAddChild((xmlNodePtr) tempDoc, tempNode);

            xsldbgGenericErrorFunc(QObject::tr("Going to the command shell; not all xsldbg commands will work as not all needed have been loaded.\n"));
            xslDebugStatus = DEBUG_STOP;
            if ((cur == NULL) && (doc == NULL)) {
                /*no doc's loaded */
                debugXSLBreak(tempNode, tempNode, NULL, NULL);
            } else if ((cur != NULL) && (doc == NULL)) {
                /* stylesheet is loaded */
                debugXSLBreak((xmlNodePtr) cur->doc, tempNode, NULL, NULL);
            } else if ((cur == NULL) && (doc != NULL)) {
                /* xml doc is loaded */
                debugXSLBreak(tempNode, (xmlNodePtr) doc, NULL, NULL);
            } else {
                /* unexpected problem, both docs are loaded */
                debugXSLBreak((xmlNodePtr) cur->doc, (xmlNodePtr) doc,
                              NULL, NULL);
            }
            xmlFreeDoc(tempDoc);
        } else if (showPrompt && !optionsGetIntOption(OPTIONS_SHELL)) {
            xslDebugStatus = DEBUG_QUIT;
            result = 0;         /* panic */
        }

        if (optionsGetIntOption(OPTIONS_SHELL)) {
            /* force a refesh of both stlesheet and xml data */
            filesFreeXmlFile(FILES_SOURCEFILE_TYPE);
            filesFreeXmlFile(FILES_XMLFILE_TYPE);
        }
    }

    if (!result) {
        xsldbgGenericErrorFunc(QObject::tr("Fatal error: Aborting debugger due to an unrecoverable error.\n"));
    }
    xsldbgFree();
    xsltCleanupGlobals();
    xmlCleanupParser();
    xmlMemoryDump();
    return !result;
}

xsltStylesheetPtr xsldbgLoadStylesheet()
{
    xsltStylesheetPtr cur = NULL;
    xmlDocPtr style;
    QByteArray sourceFile(optionsGetStringOption(OPTIONS_SOURCE_FILE_NAME).toUtf8().constData());

    if (optionsGetIntOption(OPTIONS_TIMING))
        startTimer();
    style = xmlParseFile(sourceFile.constData());
    if (optionsGetIntOption(OPTIONS_TIMING))
        endTimer(QObject::tr("Parsing stylesheet %1").arg(sourceFile.constData()));
    if (style == NULL) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Cannot parse file %1.\n").arg(xsldbgUrl(sourceFile)));
        cur = NULL;
        if (!optionsGetIntOption(OPTIONS_SHELL)) {
            xsldbgGenericErrorFunc(QObject::tr("Fatal error: Aborting debugger due to an unrecoverable error.\n"));
            xslDebugStatus = DEBUG_QUIT;
        } else {
            xsltGenericError(xsltGenericErrorContext, "\n");
            xslDebugStatus = DEBUG_STOP;
        }
    } else {
        cur = xsltLoadStylesheetPI(style);
        if (cur != NULL) {
            /* it is an embedded stylesheet */
            xsltProcess(style, cur);
            xsltFreeStylesheet(cur);
        } else {
            cur = xsltParseStylesheetDoc(style);
            if (cur != NULL) {
                if (cur->indent == 1)
                    xmlIndentTreeOutput = 1;
                else
                    xmlIndentTreeOutput = 0;
            } else {
                xmlFreeDoc(style);
            }
        }
    }
    return cur;
}



xmlDocPtr xsldbgLoadXmlData(void)
{
    xmlDocPtr doc = NULL;
    xmlSAXHandler mySAXHandler;
    QByteArray dataFile(optionsGetStringOption(OPTIONS_DATA_FILE_NAME).toUtf8().constData());
    doc = NULL;

    xmlSAXVersion(&mySAXHandler,2);
    oldGetEntity = mySAXHandler.getEntity;
    mySAXHandler.getEntity = xsldbgGetEntity;

    if (optionsGetIntOption(OPTIONS_TIMING))
        startTimer();
#ifdef LIBXML_HTML_ENABLED
    if (optionsGetIntOption(OPTIONS_HTML))
        doc = htmlParseFile(dataFile.constData(), NULL);
    else
#endif
#if LIBXML_VERSION >= 20600
        doc = xmlSAXParseFile(&mySAXHandler, dataFile.constData(), 0);
#else
        doc = xmlParseFile(dataFile.constData());
#endif
    if (doc == NULL) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse file %1.\n").arg(xsldbgUrl(dataFile)));
        if (!optionsGetIntOption(OPTIONS_SHELL)) {
            xsldbgGenericErrorFunc(QObject::tr("Fatal error: Aborting debugger due to an unrecoverable error.\n"));
            xslDebugStatus = DEBUG_QUIT;
        } else {
            xsltGenericError(xsltGenericErrorContext, "\n");
            xslDebugStatus = DEBUG_STOP;
        }
    } else if (optionsGetIntOption(OPTIONS_TIMING))
        endTimer(QString("Parsing document %1").arg(xsldbgUrl(dataFile)).toUtf8().constData());

    return doc;
}


xmlDocPtr xsldbgLoadXmlTemporary(const xmlChar * path)
{
    xmlDocPtr doc = NULL;
    doc = NULL;

    if (optionsGetIntOption(OPTIONS_TIMING))
        startTimer();
#ifdef LIBXML_HTML_ENABLED
    if (optionsGetIntOption(OPTIONS_HTML))
        doc = htmlParseFile((char *) path, NULL);
    else
#endif
        doc = xmlSAXParseFile(&mySAXhdlr, (char *) path, 0);
    if (doc == NULL) {
        xsldbgGenericErrorFunc(QObject::tr("Error: Unable to parse file %1.\n").arg(xsldbgUrl(path)));
    }

    if (optionsGetIntOption(OPTIONS_TIMING)
            && (xslDebugStatus != DEBUG_QUIT)) {
        endTimer(QString("Parsing document %1").arg(xsldbgUrl(path)));
    }
    return doc;
}

#ifdef WIN32

/* For the windows world we capture the control event */
BOOL WINAPI
handler_routine(DWORD dwCtrlType)
{

    switch (dwCtrlType) {
    case CTRL_C_EVENT:
    case CTRL_BREAK_EVENT:
    case CTRL_CLOSE_EVENT:
        catchSigInt(SIGINT);
        break;

    case CTRL_LOGOFF_EVENT:
    case CTRL_SHUTDOWN_EVENT:
        xsldbgFree();
        exit(1);
        break;

    default:
        printf("Error: Unknown control event\n");
        break;
    }

    return (true);
}

#endif

#if LIBXML_VERSION >= 2006000
/* libxml/ handlers */
void xsldbgStructErrorHandler(void * userData, xmlErrorPtr error)
{
    if (error && error->message && (error->level >= 0) && (error->level <= 4)){
        if (getThreadStatus() != XSLDBG_MSG_THREAD_RUN){
            static const char *msgPrefix[4 + 1] = {"", "warning :", "error:", "fatal:"};
            if (error->file)
                xsltGenericError(xsltGenericErrorContext, "%s%s in file \"%s\" line %d", msgPrefix[error->level], error->message, error->file, error->line);
            else
                xsltGenericError(xsltGenericErrorContext, "%s%s", msgPrefix[error->level], error->message);

        }else{
            xsltGenericError(xsltGenericErrorContext,"Struct error handler");
            notifyXsldbgApp(XSLDBG_MSG_ERROR_MESSAGE, error);
        }
    }
}

void xsldbgSAXErrorHandler(void * ctx, const char * msg, ...)
{
    if (ctx)
        xsldbgStructErrorHandler(0, ((xmlParserCtxtPtr)ctx)->lastError);
}

void xsldbgSAXWarningHandler(void * ctx, const char * msg, ...)
{
    if (ctx)
        xsldbgStructErrorHandler(0, ((xmlParserCtxtPtr)ctx)->lastError);
}

#endif

void catchSigInt(int value)
{
    Q_UNUSED(value);
    if ((xslDebugStatus == DEBUG_NONE) || (xsldbgStop == 1) || (xslDebugStatus == DEBUG_STOP)) {
        xsldbgFree();
        exit(1);
    }
#ifdef __riscos
    /* re-catch SIGINT - RISC OS resets the handler when the interrupt occurs */
    signal(SIGINT, catchSigInt);
#endif

    if (xslDebugStatus != DEBUG_STOP) {
        /* stop running/walking imediately !! */
        xsldbgStop = 1;
    }
}


void catchSigTerm(int value)
{
    Q_UNUSED(value);
    xsldbgFree();
    exit(1);
}



typedef void (*sighandler_t) (int);
static sighandler_t oldHandler;

static int initialized = 0;

int xsldbgInit()
{
    int result = 0;
    int xmlVer = 0;

    if (!initialized) {
        sscanf(xmlParserVersion, "%d", &xmlVer);
        if (!debugInit()) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
            xsltGenericError(xsltGenericErrorContext,
                             "Fatal error: Init of debug module failed\n");
#endif
            return result;
        }
        if (!filesInit()) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
            xsltGenericError(xsltGenericErrorContext,
                             "Fatal error: Init of files module failed\n");
#endif
            return result;
        }

        if (!optionsInit()) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
            xsltGenericError(xsltGenericErrorContext,
                             "Fatal error: Init of options module failed\n");
#endif
            return result;
        }

        if (!searchInit()) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
            xsltGenericError(xsltGenericErrorContext,
                             "Fatal error: Init of search module failed\n");
#endif
            return result;
        }



        /* set up the parser */
        xmlInitParser();
#if 0
#if LIBXML_VERSION >= 20600
        xmlSetGenericErrorFunc(NULL,  NULL);
        xmlSetStructuredErrorFunc(NULL , (xmlStructuredErrorFunc)xsldbgStructErrorHandler);
#else
        xmlSetGenericErrorFunc(0, xsldbgGenericErrorFunc);
        xsltSetGenericErrorFunc(0, xsldbgGenericErrorFunc);
#endif    
#else
        xmlSetGenericErrorFunc(0, xsldbgGenericErrorFunc);
        xsltSetGenericErrorFunc(0, xsldbgGenericErrorFunc);
#endif

        /*
     * * Replace entities with their content.
     */
        xmlSubstituteEntitiesDefault(1);

        /*
     * * Register the EXSLT extensions and the test module
     */
        exsltRegisterAll();
        xsltRegisterTestModule();

        /*
     * disable CDATA from being built in the document tree
     */
        xmlDefaultSAXHandlerInit();
        xmlDefaultSAXHandler.cdataBlock = NULL;

        if (getThreadStatus() != XSLDBG_MSG_THREAD_NOTUSED) {
            initialized = 1;
            return 1;           /* this is all we need to do when running as a thread */
        }
#ifndef WIN32
        /* catch SIGINT */
        oldHandler = signal(SIGINT, catchSigInt);
#else
        if (SetConsoleCtrlHandler(handler_routine, true) != TRUE)
            return result;
#endif

#ifndef WIN32
        /* catch SIGTIN tty input available fro child */
        signal(SIGTERM, catchSigTerm);
#endif
        initialized = 1;
    }
    return 1;
}

void xsldbgFree()
{
    debugFree();
    filesFree();
    optionsFree();
    searchFree();
#ifndef WIN32
    if (oldHandler != SIG_ERR)
        signal(SIGINT, oldHandler);
#else
    SetConsoleCtrlHandler(handler_routine, false);
#endif
    initialized = 0;

#ifdef HAVE_READLINE
    /*  rl_free_line_state ();
      rl_cleanup_after_signal(); */
#   ifdef HAVE_HISTORY
    clear_history();
#   endif    
#endif

}


char msgBuffer[4000];

void xsldbgGenericErrorFunc(void *ctx, const char *msg, ...)
{
    va_list args;
    Q_UNUSED(ctx);

    va_start(args, msg);
    if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
        vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);

        notifyTextXsldbgApp(XSLDBG_MSG_TEXTOUT, msgBuffer);
    } else {
        xmlChar *encodeResult = NULL;

        vsnprintf(msgBuffer, sizeof(msgBuffer), msg, args);
        encodeResult = filesEncode((xmlChar *) msgBuffer);
        if (encodeResult) {
            fprintf(errorFile, "%s", encodeResult);
            xmlFree(encodeResult);
        } else
            fprintf(errorFile, "%s", msgBuffer);
    }
    va_end(args);
}

void xsldbgGenericErrorFunc(QString const &text)
{
    xsldbgGenericErrorFunc(0, "%s", text.toUtf8().constData());
}


QString xsldbgUrl(const char *utf8fUrl)
{
    return filesExpandName(xsldbgText(utf8fUrl));
}

QString xsldbgUrl(const xmlChar *utf8Url)
{
    return xsldbgUrl((const char*)(utf8Url));
}

QString xsldbgText(const char *utf8Text)
{
    return QString::fromUtf8(utf8Text);
}

QString xsldbgText(const xmlChar *utf8Text)
{
    return QString::fromUtf8((const char *)utf8Text);
}

