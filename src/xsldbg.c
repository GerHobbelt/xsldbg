
/***************************************************************************
                          xsldbg.c  -  description
                             -------------------
    begin                : Sun Sep 16 2001
    copyright            : (C) 2001 by Keith Isdale
    email                : k_isdale@tpg.com.au
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
 * Base on Original file xslproc.c:
 *
 * by  Daniel Veillard 
 *     daniel@veillard.com
 *
 *
 */

#ifdef VERSION
#undef VERSION
#endif

#include "xsldbg.h"
#include "options.h"
#include "files.h"
#include "breakpointInternals.h"
#include "debugXSL.h"

/* need to setup catch of SIGINT */
#include <signal.h>

/* needed by printTemplateNames */
#include <libxslt/transform.h>

/* standard includes from xsltproc*/
#include <libxslt/xslt.h>
#include <libexslt/exslt.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STDARG_H
#include <stdarg.h>
#endif
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#ifdef LIBXML_DOCB_ENABLED
#include <libxml/DOCBparser.h>
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
#include <libxml/xinclude.h>
#endif
#ifdef LIBXML_CATALOG_ENABLED
#include <libxml/catalog.h>
#endif
#include <libxml/parserInternals.h>

#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <libxslt/extensions.h>

#include <libexslt/exsltconfig.h>

#ifdef WIN32
#ifdef _MSC_VER
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define gettimeofday(p1,p2)
#define HAVE_TIME_H
#include <time.h>
#define HAVE_STDARG_H
#include <stdarg.h>
#endif /* _MS_VER */
#else /* WIN32 */
#if defined(HAVE_SYS_TIME_H)
#include <sys/time.h>
#elif defined(HAVE_TIME_H)
#include <time.h>
#endif
#endif /* WIN32 */

#ifndef HAVE_STAT
#  ifdef HAVE__STAT
     /* MS C library seems to define stat and _stat. The definition
      *         is identical. Still, mapping them to each other causes a warning. */
#    ifndef _MSC_VER
#      define stat(x,y) _stat(x,y)
#    endif
#    define HAVE_STAT
#  endif
#endif


xmlParserInputPtr xmlNoNetExternalEntityLoader(const char *URL,
                                               const char *ID,
                                               xmlParserCtxtPtr ctxt);

/* -----------------------------------------
   Private function declarations for xsldbg.c
 -------------------------------------------*/

/**
 * xsldbgInit:
 * 
 * Returns 1 if able to allocate memory needed by xsldbg
 *         0 otherwise
 */
int xsldbgInit(void);


/**
 * xsldbgFree:
 *
 * Free memory used by xsldbg
 */
void xsldbgFree(void);


/**
 * printTemplates:
 * @style : valid as parsed my xsldbg
 * @doc :    "    "   "     "    "
 *  
 * print out list of template names
 */
void printTemplates(xsltStylesheetPtr style, xmlDocPtr doc);


/**
 * catchSigInt:
 * @value : not used
 *
 * Recover from a signal(SIGINT), exit if needed
 */
void catchSigInt(int value ATTRIBUTE_UNUSED);


/**
 * catchSigTerm:
 * @value : not used
 *
 * Clean up and exit
 */
void
  catchSigTerm(int value ATTRIBUTE_UNUSED);


/* ------------------------------------- 
    End private functions
---------------------------------------*/


/*
 * Internal timing routines to remove the necessity to have unix-specific
 * function calls
 */

#if defined(HAVE_GETTIMEOFDAY)
static struct timeval begin, end;

/*
 * startTimer: call where you want to start timing
 */
static void
startTimer(void)
{
    gettimeofday(&begin, NULL);
}

/*
 * endTimer: call where you want to stop timing and to print out a
 *           message about the timing performed; format is a printf
 *           type argument
 */
static void
endTimer(const char *format, ...)
{
    long msec;
    va_list ap;

    gettimeofday(&end, NULL);
    msec = end.tv_sec - begin.tv_sec;
    msec *= 1000;
    msec += (end.tv_usec - begin.tv_usec) / 1000;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);

    xsltGenericError(xsltGenericErrorContext, " took %ld ms\n", msec);
}
#elif defined(HAVE_TIME_H)

/*
 * No gettimeofday function, so we have to make do with calling clock.
 * This is obviously less accurate, but there's little we can do about
 * that.
 */

clock_t begin, end;
static void
startTimer(void)
{
    begin = clock();
}
static void
endTimer(char *format, ...)
{
    long msec;
    va_list ap;

    end = clock();
    msec = ((end - begin) * 1000) / CLOCKS_PER_SEC;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    xsltGenericError(xsltGenericErrorContext, " took %ld ms\n", msec);
}
#else

/*
 * We don't have a gettimeofday or time.h, so we just don't do timing
 */
static void
startTimer(void)
{
    /*
     * Do nothing
     */
}
static void
endTimer(char *format, ...)
{
    /*
     * We cannot do anything because we don't have a timing function
     */
#ifdef HAVE_STDARG_H
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
    xsltGenericError(xsltGenericErrorContext, " was not timed\n", msec);
#else
    /* We don't have gettimeofday, time or stdarg.h, what crazy world is
     * this ?!
     */
#endif
}
#endif

static void
xsltProcess(xmlDocPtr doc, xsltStylesheetPtr cur, const char *filename)
{
    xmlDocPtr res;
    const char *params[8 * 2 + 2];
    int nbparams = 0;
    int paramIndex;
    ParameterItemPtr paramItem;

    /* Copy the parameters accross for libxslt */
    for (paramIndex = 0;
         paramIndex < arrayListCount(getParamItemList());
         paramIndex++) {
        paramItem = arrayListGet(getParamItemList(), paramIndex);
        if (paramItem) {
            params[nbparams] = (char *) paramItem->name;
            params[nbparams + 1] = (char *) paramItem->value;
            nbparams += 2;
        }
    }

    params[nbparams] = NULL;

#ifdef LIBXML_XINCLUDE_ENABLED
    if (isOptionEnabled(OPTIONS_XINCLUDE)) {
        if (isOptionEnabled(OPTIONS_TIMING))
            startTimer();
        xmlXIncludeProcess(doc);
        if (isOptionEnabled(OPTIONS_TIMING)) {
            endTimer("XInclude processing %s", filename);
        }
    }
#endif
    if (isOptionEnabled(OPTIONS_TIMING))
        startTimer();
    if (getStringOption(OPTIONS_OUTPUT_FILE_NAME) == NULL) {
        if (getIntOption(OPTIONS_REPEAT)) {
            int j;

            for (j = 1; j < getIntOption(OPTIONS_REPEAT); j++) {
                res = xsltApplyStylesheet(cur, doc, params);
                xmlFreeDoc(res);
                doc = loadXmlData();
            }
        }
        if (isOptionEnabled(OPTIONS_PROFILING)) {
            res = xsltProfileStylesheet(cur, doc, params, stderr);
        } else {
            res = xsltApplyStylesheet(cur, doc, params);
        }
        if (isOptionEnabled(OPTIONS_PROFILING)) {
            if (getIntOption(OPTIONS_REPEAT))
                endTimer("Applying stylesheet %d times",
                         getIntOption(OPTIONS_REPEAT));
            else
                endTimer("Applying stylesheet");
        }
        if (res == NULL) {
            xsltGenericError(xsltGenericErrorContext, "no result for %s\n",
                             filename);
            return;
        }
        if (isOptionEnabled(OPTIONS_NOOUT)) {
            xmlFreeDoc(res);
            return;
        }
#ifdef LIBXML_DEBUG_ENABLED
        if (isOptionEnabled(OPTIONS_DEBUG))
            xmlDebugDumpDocument(stdout, res);
        else {
#endif
            if (cur->methodURI == NULL) {
                if (isOptionEnabled(OPTIONS_TIMING))
                    startTimer();
		if (terminalIO == NULL)
		  xsltSaveResultToFile(stdout, res, cur);
		else
		  xsltSaveResultToFile(terminalIO, res, cur);
                if (isOptionEnabled(OPTIONS_TIMING))
                    endTimer("Saving result");
            } else {
                if (xmlStrEqual(cur->method, (const xmlChar *) "xhtml")) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "non standard output xhtml\n");
                    if (isOptionEnabled(OPTIONS_TIMING))
                        startTimer();
		    if (terminalIO == NULL)
		      xsltSaveResultToFile(stdout, res, cur);
		    else
		      xsltSaveResultToFile(terminalIO, res, cur);
                    if (isOptionEnabled(OPTIONS_TIMING))
                        endTimer("Saving result");
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Unsupported non standard output %s\n",
                                     cur->method);
                }
            }
#ifdef LIBXML_DEBUG_ENABLED
        }
#endif

        xmlFreeDoc(res);
    } else {
        xsltRunStylesheet(cur, doc, params, (char *)
                          getStringOption(OPTIONS_OUTPUT_FILE_NAME), NULL,
                          NULL);
        if (isOptionEnabled(OPTIONS_TIMING))
            endTimer("Running stylesheet and saving result");
    }
}


static void
usage(const char *name)
{
    xsltGenericError(xsltGenericErrorContext,
                     "Usage: %s [options] stylesheet file\n", name);
    xsltGenericError(xsltGenericErrorContext,
                     "Without any parameters xsldbg starts in command mode, ready"
                     "for the source and data to be select\n");
    xsltGenericError(xsltGenericErrorContext, "   Options:\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --version or -V: show the version of libxml and libxslt used\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --verbose or -v: show logs of what's happening\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --output file or -o file: save to a given file\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --timing: display the time used\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --repeat: run the transformation 20 times\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --debug: dump the tree of the result instead\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --novalid: skip the Dtd loading phase\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --noout: do not dump the result\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --maxdepth val : increase the maximum depth\n");
#ifdef LIBXML_HTML_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --html: the input document is(are) an HTML file(s)\n");
#endif
#ifdef LIBXML_DOCB_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --docbook: the input document is SGML docbook\n");
#endif
    xsltGenericError(xsltGenericErrorContext,
                     "      --param name value : pass a (parameter,value) pair\n");
    xsltGenericError(xsltGenericErrorContext,
                     "            string values must be quoted like \"'string'\"\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --nonet refuse to fetch DTDs or entities over network\n");
#ifdef LIBXML_CATALOG_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --catalogs : use the catalogs from $SGML_CATALOG_FILES\n");
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
    xsltGenericError(xsltGenericErrorContext,
                     "      --xinclude : do XInclude processing on document intput\n");
#endif
    xsltGenericError(xsltGenericErrorContext,
                     "      --profile or --norman : dump profiling informations \n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --cd <PATH>: change to specfied working directory\n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --shell : start xsldebugger \n");
    xsltGenericError(xsltGenericErrorContext,
                     "      --gdb : run in gdb mode printing out more information");
}

int
main(int argc, char **argv)
{
    int i, result = 1, noFilesFound = 0;
    xsltStylesheetPtr cur = NULL;

    /* in some cases we always want to bring up a command prompt */
    int showPrompt;
    xmlDocPtr doc;

    xmlInitMemory();

    LIBXML_TEST_VERSION xmlLineNumbersDefault(1);

    xsldbgInit();

    if (argc == 1)
        result = enableOption(OPTIONS_SHELL, 1);

    if (isOptionEnabled(OPTIONS_NOVALID) == 0)
        xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    else
        xmlLoadExtDtdDefaultValue = 0;
    for (i = 1; i < argc; i++) {
        if (!result)
            break;
        if (argv[i][0] != '-') {
            switch (noFilesFound) {
                case 0:
                    setStringOption(OPTIONS_SOURCE_FILE_NAME,
                                    (xmlChar *) argv[i]);
                    noFilesFound++;
                    break;
                case 1:
                    setStringOption(OPTIONS_DATA_FILE_NAME,
                                    (xmlChar *) argv[i]);
                    noFilesFound++;
                    break;

                default:
                    xsltGenericError(xsltGenericErrorContext,
                                     "Too many file names supplied\n");
                    result = 0;
            }
            continue;
        }
#ifdef LIBXML_DEBUG_ENABLED
        if ((!strcmp(argv[i], "-debug")) || (!strcmp(argv[i], "--debug"))) {
            if (result) {
                result = enableOption(OPTIONS_DEBUG, 1);
                strcpy(argv[i], "");
            }
        } else
#endif
        if ((!strcmp(argv[i], "-v")) ||
                (!strcmp(argv[i], "-verbose")) ||
                (!strcmp(argv[i], "--verbose"))) {
            xsltSetGenericDebugFunc(stderr, NULL);
        } else if ((!strcmp(argv[i], "-o")) ||
                   (!strcmp(argv[i], "-output")) ||
                   (!strcmp(argv[i], "--output"))) {
            strcpy(argv[i], "");
            i++;
            setStringOption(OPTIONS_OUTPUT_FILE_NAME, (xmlChar *) argv[i]);
            strcpy(argv[i], "");
        } else if ((!strcmp(argv[i], "-V")) ||
                   (!strcmp(argv[i], "-version")) ||
                   (!strcmp(argv[i], "--version"))) {
            xsltGenericError(xsltGenericErrorContext,
                             " xsldbg created by Keith Isdale <k_isdale@tpg.com.au>\n");
            xsltGenericError(xsltGenericErrorContext,
                             " Version %s, Date created %s\n", VERSION,
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
            strcpy(argv[i], "");
        } else if ((!strcmp(argv[i], "-repeat"))
                   || (!strcmp(argv[i], "--repeat"))) {
            if (getIntOption(OPTIONS_REPEAT) == 0)
                setIntOption(OPTIONS_REPEAT, 20);
            else
                setIntOption(OPTIONS_REPEAT, 100);
        } else if ((!strcmp(argv[i], "-novalid")) ||
                   (!strcmp(argv[i], "--novalid"))) {
            if (result) {
                result = enableOption(OPTIONS_NOVALID, 1);
                strcpy(argv[i], "");
            }
        } else if ((!strcmp(argv[i], "-noout")) ||
                   (!strcmp(argv[i], "--noout"))) {
            if (result) {
                result = enableOption(OPTIONS_NOOUT, 1);
                strcpy(argv[i], "");
            }
#ifdef LIBXML_DOCB_ENABLED
        } else if ((!strcmp(argv[i], "-docbook")) ||
                   (!strcmp(argv[i], "--docbook"))) {
            if (result) {
                result = enableOption(OPTIONS_DOCBOOK, 1);
                strcpy(argv[i], "");
            }
#endif
#ifdef LIBXML_HTML_ENABLED
        } else if ((!strcmp(argv[i], "-html")) ||
                   (!strcmp(argv[i], "--html"))) {
            if (result) {
                result = enableOption(OPTIONS_HTML, 1);
                strcpy(argv[i], "");
            }
#endif
        } else if ((!strcmp(argv[i], "-timing")) ||
                   (!strcmp(argv[i], "--timing"))) {
            if (result) {
                result = enableOption(OPTIONS_TIMING, 1);
                strcpy(argv[i], "");
            }
        } else if ((!strcmp(argv[i], "-profile")) ||
                   (!strcmp(argv[i], "--profile"))) {
            if (result) {
                result = enableOption(OPTIONS_PROFILING, 1);
                strcpy(argv[i], "");
            }
        } else if ((!strcmp(argv[i], "-norman")) ||
                   (!strcmp(argv[i], "--norman"))) {
            if (result) {
                result = enableOption(OPTIONS_PROFILING, 1);
                strcpy(argv[i], "");
            }
        } else if ((!strcmp(argv[i], "-nonet")) ||
                   (!strcmp(argv[i], "--nonet"))) {
            xmlSetExternalEntityLoader(xmlNoNetExternalEntityLoader);
#ifdef LIBXML_CATALOG_ENABLED
        } else if ((!strcmp(argv[i], "-catalogs")) ||
                   (!strcmp(argv[i], "--catalogs"))) {
            const char *catalogs;

            catalogs = getenv("SGML_CATALOG_FILES");
            if (catalogs == NULL) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Variable $SGML_CATALOG_FILES not set\n");
            } else {
                xmlLoadCatalogs(catalogs);
            }
            strcpy(argv[i], "");
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
        } else if ((!strcmp(argv[i], "-xinclude")) ||
                   (!strcmp(argv[i], "--xinclude"))) {
            if (result) {
                result = enableOption(OPTIONS_XINCLUDE, 1);
                strcpy(argv[i], "");
            }
            xsltSetXIncludeDefault(1);
#endif
        } else if ((!strcmp(argv[i], "-param")) ||
                   (!strcmp(argv[i], "--param"))) {
            i++;
            arrayListAdd(getParamItemList(),
                            paramItemNew((xmlChar *) argv[i],
                                         (xmlChar *) argv[i + 1]));
            i++;
            if (arrayListCount(getParamItemList()) >= 8) {
                xsltGenericError(xsltGenericErrorContext,
                                 "too many params\n");
                return (1);
            }
        } else if ((!strcmp(argv[i], "-maxdepth")) ||
                   (!strcmp(argv[i], "--maxdepth"))) {
            int value;

            strcpy(argv[i], "");
            i++;
            if (sscanf(argv[i], "%d", &value) == 1) {
                if (value > 0)
                    xsltMaxDepth = value;
            }
            strcpy(argv[i], "");

        /*---------------------------------------- */
            /*     Handle xsldbg specific options      */

        /*---------------------------------------- */

        } else if ((!strcmp(argv[i], "-shell")) ||
                   (!strcmp(argv[i], "--shell"))) {
            if (result) {
                result = enableOption(OPTIONS_SHELL, 1);
                strcpy(argv[i], "");
            }
        } else if ((!strcmp(argv[i], "-cd")) || (!strcmp(argv[i], "--cd"))) {
            strcpy(argv[i], "");
            if (i + 1 < argc) {
                i++;
                result = changeDir((xmlChar *) argv[i]);
                strcpy(argv[i], "");
            } else {
                xsltGenericError(xsltGenericErrorContext,
                                 "Missing path name after --cd option\n");
            }

        } else if ((!strcmp(argv[i], "-gdb"))
                   || (!strcmp(argv[i], "--gdb"))) {
            /* run in gdb mode printing out more information after each command */
            if (result) {
                result = enableOption(OPTIONS_GDB, 1);
                strcpy(argv[i], "");
            }
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Unknown option %s\n", argv[i]);
            result = 0;
        }
    }

    if (!result) {
        usage(argv[0]);
        xsldbgFree();
        return (1);
    }


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
     * shell interraction
     */
    if (!isOptionEnabled(OPTIONS_SHELL)) {      /* excecute stylesheet (ie no debugging) */
        xslDebugStatus = DEBUG_NONE;
    } else {
        xsltGenericError(xsltGenericErrorContext, "XSLDBG %s\n", VERSION);
    }


    debugGotControl(0);
    while (xslDebugStatus != DEBUG_QUIT) {
        /* don't force xsldbg to show command prompt */
        showPrompt = 0;
        cur = NULL;
        doc = NULL;
        if (isOptionEnabled(OPTIONS_SHELL)) {
	    debugGotControl(0);
            xsltGenericError(xsltGenericErrorContext,
                             "\nStarting stylesheet\n\n");
	    if (getIntOption(OPTIONS_TRACE) == TRACE_OFF)
	      xslDebugStatus = DEBUG_STOP;        /* stop as soon as possible */
	}

        if ((getStringOption(OPTIONS_SOURCE_FILE_NAME) == NULL) ||
            (getStringOption(OPTIONS_DATA_FILE_NAME) == NULL)) {
            /* at least on file name has not been set */
            /*goto a xsldbg command prompt */
            showPrompt = 1;
        } else {
            loadXmlFile(NULL, FILES_SOURCEFILE_TYPE);
            cur = getStylesheet();
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
            loadXmlFile(NULL, FILES_XMLFILE_TYPE);
            doc = getMainDoc();
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
                    xsltProcess(doc, cur, (char *)
                                getStringOption(OPTIONS_DATA_FILE_NAME));
                    result++;
                }
            }

            if (isOptionEnabled(OPTIONS_SHELL) && (showPrompt == 0)) {
                if ((xslDebugStatus != DEBUG_QUIT)
                    && !debugGotControl(-1)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "\nDebugger never received control\n");
                    /*goto a xsldbg command prompt */
                    showPrompt = 1;
                    xslDebugStatus = DEBUG_STOP;
                } else {
                    xsltGenericError(xsltGenericErrorContext,
                                     "\nFinished stylesheet\n\n");
                    {
                        /* handle trace execution */
                        int trace = getIntOption(OPTIONS_TRACE);

                        switch (trace) {
                            case TRACE_OFF:
                                /* no trace of execution */
                                break;

                            case TRACE_ON:
                                /* tell xsldbg to stop tracing next time we get here */
                                setIntOption(OPTIONS_TRACE, TRACE_RUNNING);
                                xslDebugStatus = DEBUG_TRACE;
                                break;

                            case TRACE_RUNNING:
                                /* turn off tracing */
                                xslDebugStatus = DEBUG_CONT;
                                setIntOption(OPTIONS_TRACE, TRACE_OFF);
                                break;
                        }
                    }
                }
            } else {
                /* request to execute stylesheet only  so we're done */
                xslDebugStatus = DEBUG_QUIT;
            }
        } else {
            /* Some sort of problem loading source file has occured. Quit? */
            if (xslDebugStatus == DEBUG_NONE) {
                xslDebugStatus = DEBUG_QUIT;    /* Panic!! */
                result = 0;
            } else {
                /*goto a xsldbg command prompt */
                showPrompt = 1;
            }
        }

        if (showPrompt && isOptionEnabled(OPTIONS_SHELL)) {
            xmlDocPtr tempDoc = xmlNewDoc((xmlChar *) "1.0");
            xmlNodePtr tempNode =
                xmlNewNode(NULL, (xmlChar *) "xsldbg_default_node");
            if (!tempDoc || !tempNode) {
                xsldbgFree();
                exit(1);
            }
            xmlAddChild((xmlNodePtr) tempDoc, tempNode);

            xsltGenericError(xsltGenericErrorContext,
                             "Going straight to "
                             "command shell! Not all xsldbg commands will "
                             "work as not all needed have been loaded \n");
            if ((cur == NULL) && (doc == NULL)) {
                /*no doc's loaded */
                debugBreak(tempNode, tempNode, NULL, NULL);
            } else if ((cur != NULL) && (doc == NULL)) {
                /* stylesheet is loaded */
                debugBreak((xmlNodePtr) cur->doc, tempNode, NULL, NULL);
            } else if ((cur == NULL) && (doc != NULL)) {
                /* xml doc is loaded */
                debugBreak(tempNode, (xmlNodePtr) doc, NULL, NULL);
            } else {
                /* unexpected problem, both docs are loaded */
                debugBreak((xmlNodePtr) cur->doc, (xmlNodePtr) doc,
                              NULL, NULL);
            }
            xmlFreeDoc(tempDoc);
        } else if (showPrompt && !isOptionEnabled(OPTIONS_SHELL)) {
            xslDebugStatus = DEBUG_QUIT;
            result = 0;         /* panic */
        }

        if (isOptionEnabled(OPTIONS_SHELL)) {
            /* force a refesh of both stlesheet and xml data */
            freeXmlFile(FILES_SOURCEFILE_TYPE);
            freeXmlFile(FILES_XMLFILE_TYPE);
        }
    }

    if (!result) {
        xsltGenericError(xsltGenericErrorContext,
                         "xsldbg is aborting due to unrecoverable errors!\n");
    }
    xsldbgFree();
    xsltCleanupGlobals();
    xmlCleanupParser();
    xmlMemoryDump();
    return !result;
}


/**
 * loadStylesheet:
 *
 * Returns the stylesheet after reloading it if successful
 *         NULL otherwise
 */
xsltStylesheetPtr
loadStylesheet(void)
{
    xsltStylesheetPtr cur = NULL;
    xmlDocPtr style;

    if (isOptionEnabled(OPTIONS_TIMING))
        startTimer();
    style = xmlParseFile((const char *)
                         getStringOption(OPTIONS_SOURCE_FILE_NAME));
    if (isOptionEnabled(OPTIONS_TIMING))
        endTimer("Parsing stylesheet %s",
                 getStringOption(OPTIONS_SOURCE_FILE_NAME));
    if (style == NULL) {
        xsltGenericError(xsltGenericErrorContext, "cannot parse %s\n",
                         getStringOption(OPTIONS_SOURCE_FILE_NAME));
        cur = NULL;
        if (!isOptionEnabled(OPTIONS_SHELL)) {
            xsltGenericError(xsltGenericErrorContext,
                             "Aborting debugger!!\n");
            xslDebugStatus = DEBUG_QUIT;
        } else {
            xsltGenericError(xsltGenericErrorContext, "\n");
            xslDebugStatus = DEBUG_STOP;
        }
    } else {
        cur = xsltLoadStylesheetPI(style);
        if (cur != NULL) {
            /* it is an embedded stylesheet */
            xsltProcess(style, cur, (char *)
                        getStringOption(OPTIONS_SOURCE_FILE_NAME));
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


/**
 * loadXmlData:
 *
 * Returns the data file after reloading it if successful
 *         NULL otherwise
 */
xmlDocPtr
loadXmlData(void)
{
    xmlDocPtr doc = NULL;

    /*
     * disable CDATA from being built in the document tree
     */
    xmlDefaultSAXHandlerInit();
    xmlDefaultSAXHandler.cdataBlock = NULL;


    doc = NULL;

    if (isOptionEnabled(OPTIONS_TIMING))
        startTimer();
#ifdef LIBXML_HTML_ENABLED
    if (isOptionEnabled(OPTIONS_HTML))
        doc =
            htmlParseFile((char *) getStringOption(OPTIONS_DATA_FILE_NAME),
                          NULL);
    else
#endif
#ifdef LIBXML_DOCB_ENABLED
    if (isOptionEnabled(OPTIONS_DOCBOOK))
        doc =
            docbParseFile((char *) getStringOption(OPTIONS_DATA_FILE_NAME),
                          NULL);
    else
#endif
        doc =
            xmlParseFile((char *) getStringOption(OPTIONS_DATA_FILE_NAME));
    if (doc == NULL) {
        xsltGenericError(xsltGenericErrorContext,
                         "unable to parse %s\n",
                         getStringOption(OPTIONS_DATA_FILE_NAME));
        if (!isOptionEnabled(OPTIONS_SHELL)) {
            xsltGenericError(xsltGenericErrorContext,
                             "Aborting debugger!!\n");
            xslDebugStatus = DEBUG_QUIT;
        } else {
            xsltGenericError(xsltGenericErrorContext, "\n");
            xslDebugStatus = DEBUG_STOP;
        }
    } else if (isOptionEnabled(OPTIONS_TIMING))
        endTimer("Parsing document %s",
                 getStringOption(OPTIONS_DATA_FILE_NAME));

    return doc;
}

/**
 * loadXmlTemporary:
 *
 * Returns the temporary file after reloading it if successful,
 *         NULL otherwise
 */
xmlDocPtr
loadXmlTemporary(const xmlChar * path)
{
    xmlDocPtr doc = NULL;

    /*
     * disable CDATA from being built in the document tree
     */
    xmlDefaultSAXHandlerInit();
    xmlDefaultSAXHandler.cdataBlock = NULL;


    doc = NULL;
    if (isOptionEnabled(OPTIONS_TIMING))
        startTimer();
#ifdef LIBXML_HTML_ENABLED
    if (isOptionEnabled(OPTIONS_HTML))
        doc = htmlParseFile((char *) path, NULL);
    else
#endif
#ifdef LIBXML_DOCB_ENABLED
    if (isOptionEnabled(OPTIONS_DOCBOOK))
        doc = docbParseFile((char *) path, NULL);
    else
#endif
        doc = xmlParseFile((char *) path);
    if (doc == NULL) {
        xsltGenericError(xsltGenericErrorContext, "unable to parse %s\n",
                         path);
    }

    if (isOptionEnabled(OPTIONS_TIMING) && (xslDebugStatus != DEBUG_QUIT)) {
        endTimer("Parsing document %s", path);
    }
    return doc;
}

/**
 * printTemplates:
 * @style : valid as parsed my xsldbg
 * @doc :    "    "   "     "    "
 *  
 * print out list of template names
 */
void
printTemplates(xsltStylesheetPtr style, xmlDocPtr doc)
{
    xsltTransformContextPtr ctxt = xsltNewTransformContext(style, doc);

    if (ctxt) {
        /* don't be verbose when printing out template names */
        xslDbgPrintTemplateNames(ctxt, NULL, NULL, 0, 0);
    } else {
        xsltGenericError(xsltGenericErrorContext,
                         "Unable to create context : print templates\n");
    }
}



/**
 * catchSigInt:
 * @value : not used
 *
 * Recover from a signal(SIGINT), exit if needed
 */
void
catchSigInt(int value ATTRIBUTE_UNUSED)
{
    if (xslDebugStatus == DEBUG_RUN) {
        xsldbgFree();
        exit(1);
    }

    if (xslDebugStatus != DEBUG_STOP) {
        /* stop running/walking imediately !! */
        xslDebugStatus = DEBUG_STOP;
        setIntOption(OPTIONS_WALK_SPEED, WALKSPEED_STOP);
    }
}


/**
 * catchSigTerm:
 * @value : not used
 *
 * Clean up and exit
 */
void
catchSigTerm(int value ATTRIBUTE_UNUSED)
{
    xsldbgFree();
    exit(1);
}



typedef void (*sighandler_t) (int);
sighandler_t oldHandler;

/**
 * xsldbgInit:
 * 
 * Returns 1 if able to allocate memory needed by xsldbg
 *         0 otherwise
 */
int
xsldbgInit()
{
    static int initialized = 0;
    int result = 0;

    if (!initialized) {
        debugInit();
        result = filesInit();
        if (result)
            result = optionsInit();
	/* must start with tracing off */
	setIntOption(OPTIONS_TRACE,TRACE_OFF);
        if (result)
            result = breakPointInit();
        if (result)
            result = searchInit();

        /* catch SIGINT */
        oldHandler = signal(SIGINT, catchSigInt);
        /* catch SIGTIN tty input available fro child */
        signal(SIGTERM, catchSigTerm);
        initialized = 1;
    }
    return result;
}

/**
 * xsldbgFree:
 *
 * Free memory used by xsldbg
 */
void
xsldbgFree()
{
    debugFree();
    filesFree();
    optionsFree();
    breakPointFree();
    searchFree();

    if (oldHandler != SIG_ERR)
        signal(SIGINT, oldHandler);
}
