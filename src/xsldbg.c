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
 * Original file xslproc.c:
 *
 * See Copyright for the status of this LGPL'd software.
 *
 * daniel@veillard.com
 *
 *
 * New file xsldbg.c: Obtained permission to modify to support debugging
 * This file will eventually be made reduntant when libxslt supports 
 *    debugger "add ons"
 */

#include "xsldbg.h"
#include "options.h"
#include "files.h"
#include <breakpoint/breakpoint.h>
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

int xsldbgInit(void);
void xsldbgFree(void);
int breakPointInit(void);
void breakPointFree(void);
void printTemplates(xsltStylesheetPtr style, xmlDocPtr doc);

/*
 * Internal timing routines to remove the necessity to have unix-specific
 * function calls
 */

#if defined(HAVE_GETTIMEOFDAY)
static struct timeval begin, end;
/*
 * startTimer: call where you want to start timing
 */
static void startTimer(void)
{
    gettimeofday(&begin,NULL);
}
/*
 * endTimer: call where you want to stop timing and to print out a
 *           message about the timing performed; format is a printf
 *           type argument
 */
static void endTimer(const char *format, ...)
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
    vfprintf(stderr,format,ap);
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
static void startTimer(void)
{
    begin=clock();
}
static void endTimer(char *format, ...)
{
    long msec;
    va_list ap;

    end=clock();
    msec = ((end-begin) * 1000) / CLOCKS_PER_SEC;

#ifndef HAVE_STDARG_H
#error "endTimer required stdarg functions"
#endif
    va_start(ap, format);
    vfprintf(stderr,format,ap);
    va_end(ap);
    xsltGenericError(xsltGenericErrorContext, " took %ld ms\n", msec);
}
#else
/*
 * We don't have a gettimeofday or time.h, so we just don't do timing
 */
static void startTimer(void)
{
  /*
   * Do nothing
   */
}
static void endTimer(char *format, ...)
{
  /*
   * We cannot do anything because we don't have a timing function
   */
#ifdef HAVE_STDARG_H
    va_start(ap, format);
    vfprintf(stderr,format,ap);
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
xsltProcess(xmlDocPtr doc, xsltStylesheetPtr cur, const char *filename) {
    xmlDocPtr res;
  const char *params[xslArrayListCount(getParamItemList())*2 + 2];
  int nbparams = 0;
  int index;
  ParameterItemPtr paramItem;

  /* Copy the parameters accross for libxslt*/
  for (index = 0; index < xslArrayListCount(getParamItemList()); index++){
    paramItem = xslArrayListGet(getParamItemList(), index);
    if (paramItem){
      params[nbparams] = paramItem->name;
      params[nbparams + 1] = paramItem->value;
      nbparams+=2;
    }
  }

  params[nbparams] = NULL;

#ifdef LIBXML_XINCLUDE_ENABLED
  if (isOptionEnabled(OPTIONS_XINCLUDE)) {
    if (isOptionEnabled(OPTIONS_TIMING))
	    startTimer();
	xmlXIncludeProcess(doc);
	if (isOptionEnabled(OPTIONS_TIMING)){
	    endTimer("XInclude processing %s", filename);
	}
    }
#endif
    if (isOptionEnabled(OPTIONS_TIMING))
        startTimer();
    if (getStringOption(OPTIONS_OUTPUT_FILE_NAME) == NULL) {
      if (getIntOption(OPTIONS_REPEAT)){
	    int j;

	    for (j = 1; j < getIntOption(OPTIONS_REPEAT); j++) {
		res = xsltApplyStylesheet(cur, doc, params);
		xmlFreeDoc(res);
		doc = loadXmlData();
	    }
	}
      if (isOptionEnabled(OPTIONS_PROFILING)){
	    res = xsltProfileStylesheet(cur, doc, params, stderr);
      } else {
	res = xsltApplyStylesheet(cur, doc, params);
      }
      if (isOptionEnabled(OPTIONS_PROFILING)){
	    if (getIntOption(OPTIONS_REPEAT))
		endTimer("Applying stylesheet %d times", getIntOption(OPTIONS_REPEAT));
	    else
		endTimer("Applying stylesheet");
	}
	if (res == NULL) {
	    xsltGenericError(xsltGenericErrorContext, "no result for %s\n", filename);
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
		xsltSaveResultToFile(stdout, res, cur);
		if (isOptionEnabled(OPTIONS_TIMING))
		    endTimer("Saving result");
	    } else {
		if (xmlStrEqual
		    (cur->method, (const xmlChar *) "xhtml")) {
		    xsltGenericError(xsltGenericErrorContext, "non standard output xhtml\n");
		    if (isOptionEnabled(OPTIONS_TIMING))
			startTimer();
		    xsltSaveResultToFile(stdout, res, cur);
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
	xsltRunStylesheet(cur, doc, params, 
			  getStringOption(OPTIONS_OUTPUT_FILE_NAME), 
			  NULL, NULL);
	if (isOptionEnabled(OPTIONS_TIMING))
	    endTimer("Running stylesheet and saving result");
    }
}


static void usage(const char *name) {
    printf("Usage: %s [options] stylesheet file [file ...]\n", name);
    printf("   Options:\n");
    printf("      --version or -V: show the version of libxml and libxslt used\n");
    printf("      --verbose or -v: show logs of what's happening\n");
    printf("      --output file or -o file: save to a given file\n");
    printf("      --timing: display the time used\n");
    printf("      --repeat: run the transformation 20 times\n");
    printf("      --debug: dump the tree of the result instead\n");
    printf("      --novalid: skip the Dtd loading phase\n");
    printf("      --noout: do not dump the result\n");
    printf("      --maxdepth val : increase the maximum depth\n");
#ifdef LIBXML_HTML_ENABLED
    printf("      --html: the input document is(are) an HTML file(s)\n");
#endif
#ifdef LIBXML_DOCB_ENABLED
    printf("      --docbook: the input document is SGML docbook\n");
#endif
    printf("      --param name value : pass a (parameter,value) pair\n");
    printf("            string values must be quoted like \"'string'\"\n");
    printf("      --nonet refuse to fetch DTDs or entities over network\n");
#ifdef LIBXML_CATALOG_ENABLED
    printf("      --catalogs : use the catalogs from $SGML_CATALOG_FILES\n");
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
    printf("      --xinclude : do XInclude processing on document intput\n");
#endif
    printf("      --profile or --norman : dump profiling informations \n");
    printf("      --cd <PATH>: change to specfied working directory\n");
    printf("      --root <template_name> : specify where to start debugging\n");
    printf("      --shell : start xsldebugger \n");
}

int
main(int argc, char **argv)
{
  int i, result = 1, noFilesFound = 0, retryCount = 0;
  xsltStylesheetPtr cur = NULL;
  xmlDocPtr doc, style;

  xmlInitMemory();

  LIBXML_TEST_VERSION

    xmlLineNumbersDefault(1);

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

    if (argv[i][0] != '-'){
      switch (noFilesFound){
      case 0:
	setStringOption(OPTIONS_SOURCE_FILE_NAME, argv[i]);
	noFilesFound++;
	break;
      case 1:
	setStringOption(OPTIONS_DATA_FILE_NAME, argv[i]);
	noFilesFound++;
	break;

      default:
	printf("Too many file names supplied\n");
	result = 0;
      }
      continue;
    }
#ifdef LIBXML_DEBUG_ENABLED
    if ((!strcmp(argv[i], "-debug")) || (!strcmp(argv[i], "--debug"))) {
      if (result){
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
	setStringOption(OPTIONS_OUTPUT_FILE_NAME, argv[i]);
	strcpy(argv[i], "");
      } else if ((!strcmp(argv[i], "-V")) ||
		 (!strcmp(argv[i], "-version")) ||
		 (!strcmp(argv[i], "--version"))) {
	printf(" xsldbg created by Keith Isdale <k_isdale@tpg.com.au>\n");
	printf(" Version %s, Date created %s\n", VERSION, TIMESTAMP);
	printf("Using libxml %s, libxslt %s and libexslt %s\n",
	       xmlParserVersion, xsltEngineVersion, exsltLibraryVersion);
	printf
	  ("xsldbg was compiled against libxml %d, libxslt %d and libexslt %d\n",
	   LIBXML_VERSION, LIBXSLT_VERSION, LIBEXSLT_VERSION);
	printf("libxslt %d was compiled against libxml %d\n",
	       xsltLibxsltVersion, xsltLibxmlVersion);
	printf("libexslt %d was compiled against libxml %d\n",
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
	if (result){
	  result = enableOption(OPTIONS_NOVALID, 1);	    
	  strcpy(argv[i], "");
	}
      } else if ((!strcmp(argv[i], "-noout")) ||
		 (!strcmp(argv[i], "--noout"))) {
	if (result){
	  result = enableOption(OPTIONS_NOOUT, 1);
	  strcpy(argv[i], "");
	}
#ifdef LIBXML_DOCB_ENABLED
      } else if ((!strcmp(argv[i], "-docbook")) ||
		 (!strcmp(argv[i], "--docbook"))) {
	if (result){
	  result = enableOption(OPTIONS_DOCBOOK, 1);	
	  strcpy(argv[i], "");
	}
#endif
#ifdef LIBXML_HTML_ENABLED
      } else if ((!strcmp(argv[i], "-html")) ||
		 (!strcmp(argv[i], "--html"))) {
	if (result){
	  result = enableOption(OPTIONS_HTML, 1);
	  strcpy(argv[i], "");
	}
#endif
      } else if ((!strcmp(argv[i], "-timing")) ||
		 (!strcmp(argv[i], "--timing"))) {
	if (result){
	  result = enableOption(OPTIONS_TIMING, 1);
	  strcpy(argv[i], "");
	}
      } else if ((!strcmp(argv[i], "-profile")) ||
		 (!strcmp(argv[i], "--profile"))) {
	if (result){
	  result = enableOption(OPTIONS_PROFILING, 1);
	  strcpy(argv[i], "");
	}
      } else if ((!strcmp(argv[i], "-norman")) ||
		 (!strcmp(argv[i], "--norman"))) {
	if (result){
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
	  xsltGenericError(xsltGenericErrorContext, "Variable $SGML_CATALOG_FILES not set\n");
	} else {
	  xmlLoadCatalogs(catalogs);
	}
	strcpy(argv[i], "");
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
      } else if ((!strcmp(argv[i], "-xinclude")) ||
		 (!strcmp(argv[i], "--xinclude"))) {
	if (result){
	  result = enableOption(OPTIONS_XINCLUDE, 1);
	  strcpy(argv[i], "");
	}
	xsltSetXIncludeDefault(1);
#endif
      } else if ((!strcmp(argv[i], "-param")) ||
		 (!strcmp(argv[i], "--param"))) {
	i++;
	xslArrayListAdd(getParamItemList(), 
			paramItemNew(argv[i], argv[i + 1]));
	i++;
	if (xslArrayListCount(getParamItemList()) >= 8) {
	  xsltGenericError(xsltGenericErrorContext, "too many params\n");
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
	if (result){
	  result = enableOption(OPTIONS_SHELL, 1);
	  strcpy(argv[i], "");
	}
      }else if ((!strcmp(argv[i], "-cd")) ||
		(!strcmp(argv[i], "--cd"))){
	strcpy(argv[i], "");
	if (i +1 < argc){
	  i++;
	  result = changeDir(argv[i]);
	  strcpy(argv[i], "");
	}else{
	  printf("Missing path name after --cd option\n");
	}

      } else if ((!strcmp(argv[i], "-root")) ||
		 (!strcmp(argv[i], "--root"))) {
	int parsed_root_ok = 0;
	if ((i + 1)  < argc){
	  i++;
	  result = setStringOption(OPTIONS_ROOT_TEMPLATE_NAME, argv[i]);
	  parsed_root_ok++;
	  strcpy(argv[i], "");
	}
	if (!parsed_root_ok){
	  printf("Missing root template name after --root option\n");
	}
      }

      else {
	xsltGenericError(xsltGenericErrorContext, "Unknown option %s\n", argv[i]);
	result = 0;
      }
  }

  if (!result){
    usage(argv[0]);
    xsldbgFree();
    return(1);
  }


  /*
     * Replace entities with their content.
     */
  xmlSubstituteEntitiesDefault(1);

  /*
     * Register the EXSLT extensions and the test module
     */
  exsltRegisterAll();
  xsltRegisterTestModule();

  /*
   * shell interraction
   */
  if (!isOptionEnabled(OPTIONS_SHELL)){ /* excecute stylesheet (ie no debugging)*/
    xslDebugStatus = DEBUG_RUN;      
  }

  xslDebugGotControl(0);
  while (xslDebugStatus != DEBUG_QUIT){
    if (isOptionEnabled(OPTIONS_SHELL)){
      xsltGenericError(xsltGenericErrorContext, "\nStarting stylesheet\n\n");	
    }
    if (xslDebugStatus != DEBUG_RUN && 
	(getIntOption(OPTIONS_TRACE) == TRACE_OFF))
      xslDebugStatus = DEBUG_CONT;

    if (getStringOption(OPTIONS_SOURCE_FILE_NAME) == NULL){
      xmlDocPtr tempDoc = xmlNewDoc( "1.0");
      xslDebugBreak((xmlNodePtr)tempDoc, (xmlNodePtr)tempDoc, NULL, NULL);
      xmlFreeDoc(tempDoc);
    }
    else{
      loadXmlFile(NULL, FILES_SOURCEFILE_TYPE);
      cur = getStylesheet();
    }

    if ((cur != NULL) && (cur->errors == 0)) {
      if (getStringOption(OPTIONS_DATA_FILE_NAME) == NULL){
	xmlDocPtr tempDoc = xmlNewDoc( "1.0");
	xslDebugBreak((xmlNodePtr)cur->doc, (xmlNodePtr)tempDoc, NULL, NULL);
	xmlFreeDoc(tempDoc);
      }
      else{
	loadXmlFile(NULL, FILES_XMLFILE_TYPE);
	doc = getMainDoc();
	if (doc == NULL)
	  xslDebugStatus = DEBUG_QUIT; /* panic !!*/
	if (xslDebugStatus != DEBUG_QUIT){
	  if (isOptionEnabled(OPTIONS_SHELL)){
	    int result = 0;
	    xmlNodePtr templ = xslFindTemplateNode(cur, 
						   getStringOption(OPTIONS_ROOT_TEMPLATE_NAME));
	    if (templ && templ->doc){
	      if (!xslFindBreakPointByName(getStringOption(OPTIONS_ROOT_TEMPLATE_NAME))){
		result =  xslAddBreakPoint(templ->doc->URL, 
					   xmlGetLineNo(templ), 
					   getStringOption(OPTIONS_ROOT_TEMPLATE_NAME), 
					   DEBUG_BREAK_SOURCE);
	      }else
		result++;
	      xsltProcess(doc, cur, getStringOption(OPTIONS_DATA_FILE_NAME));
	    }
	    if (!result){
	      printf("Root template '%s' not found or unable to set root break point\n" \
		     "Try one of  \n", getStringOption(OPTIONS_ROOT_TEMPLATE_NAME));	    
	      printTemplates(cur, doc);
	    
	      xslDebugStatus = DEBUG_QUIT;
	    }
	  }else{
	    xsltProcess(doc, cur, getStringOption(OPTIONS_DATA_FILE_NAME));
	  }
	}
      }
    }
    if (result)
      result  = freeXmlFile(FILES_SOURCEFILE_TYPE);
    if (result)
      result  = freeXmlFile(FILES_XMLFILE_TYPE);


    if (isOptionEnabled(OPTIONS_SHELL)){
      if( (xslDebugStatus != DEBUG_QUIT) && !xslDebugGotControl(0)){
	  xsltGenericError(xsltGenericErrorContext, "\nDebugger never received control\n" \
			   "Going straight to command shell!\n");
	  if (getStylesheet() && getMainDoc()){
	    xslDebugBreak((xmlNodePtr)getStylesheet()->doc, (xmlNodePtr)getMainDoc(), NULL, NULL);
	  }else{
	    xmlDocPtr tempDoc = xmlNewDoc( "1.0");
	    xslDebugBreak((xmlNodePtr)tempDoc, (xmlNodePtr)tempDoc, NULL, NULL);
	    xmlFreeDoc(tempDoc);
	  }
      }else{
	xsltGenericError(xsltGenericErrorContext, "\nFinished stylesheet\n\n");
	{
	  /* handle trace execution */
	  int trace = getIntOption(OPTIONS_TRACE);
	  switch(trace){
	  case TRACE_OFF:
	    /* no trace of execution*/
	  break;
	  
	  case TRACE_ON:
	    /* tell xsldbg to stop tracing next time we get here*/
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
    }else{
      xslDebugStatus = DEBUG_QUIT;/* request to execute stylesheet only */
    }    
  }

  done:

    xsldbgFree();
    xsltCleanupGlobals();
    xmlCleanupParser();
    xmlMemoryDump();
    return (0);
  }


xsltStylesheetPtr loadStyleSheet(){  
  struct timeval begin, end;
  xsltStylesheetPtr cur = NULL;
  xmlDocPtr doc, style;

  if (isOptionEnabled(OPTIONS_TIMING))
    startTimer();
  style = xmlParseFile((const char *) getStringOption(OPTIONS_SOURCE_FILE_NAME));
  if (isOptionEnabled(OPTIONS_TIMING)) 
    endTimer("Parsing stylesheet %s", getStringOption(OPTIONS_SOURCE_FILE_NAME));
  if (style == NULL) {
    xsltGenericError(xsltGenericErrorContext,  "cannot parse %s\n", getStringOption(OPTIONS_SOURCE_FILE_NAME));
    cur = NULL;
    if (!isOptionEnabled(OPTIONS_SHELL)){
      xsltGenericError(xsltGenericErrorContext, "Aborting debugger!!\n");
      xslDebugStatus = DEBUG_QUIT;
    }else{
      xsltGenericError(xsltGenericErrorContext, "\n");      
      xslDebugStatus = DEBUG_STOP;
    }
  } else {
    cur = xsltLoadStylesheetPI(style);
    if (cur != NULL) {
      /* it is an embedded stylesheet */
      xsltProcess(style, cur,getStringOption(OPTIONS_SOURCE_FILE_NAME));
      xsltFreeStylesheet(cur);
    }else{
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


xmlDocPtr loadXmlData(){
  struct timeval begin, end;
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
    doc = htmlParseFile(getStringOption(OPTIONS_DATA_FILE_NAME), NULL);
  else
#endif
#ifdef LIBXML_DOCB_ENABLED
    if (isOptionEnabled(OPTIONS_DOCBOOK))
      doc = docbParseFile(getStringOption(OPTIONS_DATA_FILE_NAME), NULL);
    else
#endif
      doc = xmlParseFile(getStringOption(OPTIONS_DATA_FILE_NAME));
  if (doc == NULL) {
    xsltGenericError(xsltGenericErrorContext, "unable to parse %s\n", getStringOption(OPTIONS_DATA_FILE_NAME));
    if (!isOptionEnabled(OPTIONS_SHELL)){
      xsltGenericError(xsltGenericErrorContext, "Aborting debugger!!\n");
      xslDebugStatus = DEBUG_QUIT;
    }else{
      xsltGenericError(xsltGenericErrorContext, "\n");      
      xslDebugStatus = DEBUG_STOP;
    }
  }else if (isOptionEnabled(OPTIONS_TIMING))
    endTimer("Parsing document %s", getStringOption(OPTIONS_DATA_FILE_NAME));
	    
  return doc;
}

/* 
 * Random load of a xml file
*/
xmlDocPtr loadXmlTemporay(const char *path){
  struct timeval begin, end;
  xmlDocPtr doc = NULL;
  /*
   * disable CDATA from being built in the document tree
   */
  xmlDefaultSAXHandlerInit();
  xmlDefaultSAXHandler.cdataBlock = NULL;


  doc = NULL;
  if (isOptionEnabled(OPTIONS_TIMING))
    gettimeofday(&begin, NULL);
#ifdef LIBXML_HTML_ENABLED
  if (isOptionEnabled(OPTIONS_HTML))
    doc = htmlParseFile(path, NULL);
  else
#endif
#ifdef LIBXML_DOCB_ENABLED
    if (isOptionEnabled(OPTIONS_DOCBOOK))
      doc = docbParseFile(path, NULL);
    else
#endif
      doc = xmlParseFile(path);
  if (doc == NULL) {
    xsltGenericError(xsltGenericErrorContext, "unable to parse %s\n", 
	    path);
  }
    
  if (isOptionEnabled(OPTIONS_TIMING) && 
      (xslDebugStatus != DEBUG_QUIT)) {
    long msec;
      
    gettimeofday(&end, NULL);
    msec = end.tv_sec - begin.tv_sec;
    msec *= 1000;
    msec += (end.tv_usec - begin.tv_usec) / 1000;
    xsltGenericError(xsltGenericErrorContext, "Parsing document %s took %ld ms\n",
	    path, msec);
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
void printTemplates(xsltStylesheetPtr style, xmlDocPtr doc){
  xsltTransformContextPtr ctxt = xsltNewTransformContext(style, doc);
  if (ctxt){
    /* don't be verbose when printing out template names */
    xslDbgPrintTemplateNames(ctxt, NULL, NULL, 0);
  }else{
    printf("Unable to create context : print templates\n");
  }
}

void catchSigInt(int value){
  if (xslDebugStatus != DEBUG_STOP){
    /* stop running/walking imediately !!*/
    xslDebugStatus = DEBUG_STOP;
    setIntOption(OPTIONS_WALK_SPEED, WALKSPEED_STOP);
  }else{
    /* empty, don't want to kill program, 
       user will use {exit| bye|quit} command to quit program*/
  }
}

typedef void (*sighandler_t)(int);   
sighandler_t oldHandler;

int xsldbgInit(){
  static int initialized = 0;  
  int result = 0;
  if (!initialized){
    xslDebugInit();
    result = filesInit();
    if (result)
      result = optionsInit();
    if (result)
    result = breakPointInit();

    /* catch SIGINT */
    oldHandler = signal(SIGINT, catchSigInt);

    initialized = 1;
  }
  return result;
}

void xsldbgFree(){
  xslDebugFree();
  filesFree();
  optionsFree();
  breakPointFree(); 

  if (oldHandler != SIG_ERR)
    signal(SIGINT, oldHandler);
}
