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

/* needed by printTemplateNames */
#include "libxslt/transform.h"

#include "libxslt/xslt.h" /*libxslt.h"*/
#include "libexslt/exslt.h"
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
#endif /* _MS_VER */
#else /* WIN32 */
#include <sys/time.h>
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

void printTemplates(xsltStylesheetPtr style, xmlDocPtr doc);

static int shell = 0;
char root_template_name[100] ={"/"};
static int debug = 0;
static int repeat = 0;
static int timing = 0;
static int novalid = 0;
static int noout = 0;
#ifdef LIBXML_DOCB_ENABLED
static int docbook = 0;
#endif
#ifdef LIBXML_HTML_ENABLED
static int html = 0;
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
static int xinclude = 0;
#endif
static int profile = 0;

static struct timeval begin, end;
static const char *params[16 + 1];
static int nbparams = 0;
static const char *output = NULL;

static void
xsltProcess(xmlDocPtr doc, xsltStylesheetPtr cur, const char *filename) {
  xmlDocPtr res;

#ifdef LIBXML_XINCLUDE_ENABLED
  if (xinclude) {
    if (timing)
      gettimeofday(&begin, NULL);
    xmlXIncludeProcess(doc);
    if (timing) {
      long msec;

      gettimeofday(&end, NULL);
      msec = end.tv_sec - begin.tv_sec;
      msec *= 1000;
      msec += (end.tv_usec - begin.tv_usec) / 1000;
      fprintf(stderr, "XInclude processing %s took %ld ms\n",
	      filename, msec);
    }
  }
#endif
  if (timing)
    gettimeofday(&begin, NULL);
  if (output == NULL) {
    if (repeat) {
      int j;

      for (j = 1; j < repeat; j++) {
	res = xsltApplyStylesheet(cur, doc, params);
	xmlFreeDoc(res);
	xmlFreeDoc(doc);
#ifdef LIBXML_HTML_ENABLED
	if (html)
	  doc = htmlParseFile(filename, NULL);
	else
#endif
#ifdef LIBXML_DOCB_ENABLED
	  if (docbook)
	    doc = docbParseFile(filename, NULL);
	  else
#endif
	    doc = xmlParseFile(filename);
      }
    }
    if (profile) {
      res = xsltProfileStylesheet(cur, doc, params, stderr);
    } else {
      res = xsltApplyStylesheet(cur, doc, params);
    }
    if (timing) {
      long msec;

      gettimeofday(&end, NULL);
      msec = end.tv_sec - begin.tv_sec;
      msec *= 1000;
      msec += (end.tv_usec - begin.tv_usec) / 1000;
      if (repeat)
	fprintf(stderr,
		"Applying stylesheet %d times took %ld ms\n",
		repeat, msec);
      else
	fprintf(stderr,
		"Applying stylesheet took %ld ms\n", msec);
    }
    xmlFreeDoc(doc);
    if (res == NULL) {
      fprintf(stderr, "no result for %s\n", filename);
      return;
    }
    if (noout) {
      xmlFreeDoc(res);
      return;
    }
#ifdef LIBXML_DEBUG_ENABLED
    if (debug)
      xmlDebugDumpDocument(stdout, res);
    else {
#endif
      if (cur->methodURI == NULL) {
	if (timing)
	  gettimeofday(&begin, NULL);
	xsltSaveResultToFile(stdout, res, cur);
	if (timing) {
	  long msec;

	  gettimeofday(&end, NULL);
	  msec = end.tv_sec - begin.tv_sec;
	  msec *= 1000;
	  msec += (end.tv_usec - begin.tv_usec) / 1000;
	  fprintf(stderr, "Saving result took %ld ms\n",
		  msec);
	}
      } else {
	if (xmlStrEqual
	    (cur->method, (const xmlChar *) "xhtml")) {
	  fprintf(stderr, "non standard output xhtml\n");
	  if (timing)
	    gettimeofday(&begin, NULL);
	  xsltSaveResultToFile(stdout, res, cur);
	  if (timing) {
	    long msec;

	    gettimeofday(&end, NULL);
	    msec = end.tv_sec - begin.tv_sec;
	    msec *= 1000;
	    msec +=
	      (end.tv_usec - begin.tv_usec) / 1000;
	    fprintf(stderr,
		    "Saving result took %ld ms\n",
		    msec);
	  }
	} else {
	  fprintf(stderr,
		  "Unsupported non standard output %s\n",
		  cur->method);
	}
      }
#ifdef LIBXML_DEBUG_ENABLED
    }
#endif

    xmlFreeDoc(res);
  } else {
    xsltRunStylesheet(cur, doc, params, output, NULL, NULL);
    if (timing) {
      long msec;

      gettimeofday(&end, NULL);
      msec = end.tv_sec - begin.tv_sec;
      msec *= 1000;
      msec += (end.tv_usec - begin.tv_usec) / 1000;
      fprintf(stderr,
	      "Running stylesheet and saving result took %ld ms\n",
	      msec);
    }
    xmlFreeDoc(doc);
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
    int i;
    xsltStylesheetPtr cur = NULL;
    xmlDocPtr doc, style;

    if (argc <= 1) {
        usage(argv[0]);
        return (1);
    }

    xmlInitMemory();

    LIBXML_TEST_VERSION

    xmlLineNumbersDefault(1);

    xslDebugInit();

    if (novalid == 0)
        xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    else
        xmlLoadExtDtdDefaultValue = 0;
    for (i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-"))
            break;

        if (argv[i][0] != '-')
            continue;
#ifdef LIBXML_DEBUG_ENABLED
        if ((!strcmp(argv[i], "-debug")) || (!strcmp(argv[i], "--debug"))) {
            debug++;
        } else
#endif
        if ((!strcmp(argv[i], "-v")) ||
                (!strcmp(argv[i], "-verbose")) ||
                (!strcmp(argv[i], "--verbose"))) {
            xsltSetGenericDebugFunc(stderr, NULL);
        } else if ((!strcmp(argv[i], "-o")) ||
                   (!strcmp(argv[i], "-output")) ||
                   (!strcmp(argv[i], "--output"))) {
            i++;
            output = argv[i++];
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
        } else if ((!strcmp(argv[i], "-repeat"))
                   || (!strcmp(argv[i], "--repeat"))) {
            if (repeat == 0)
                repeat = 20;
            else
                repeat = 100;
        } else if ((!strcmp(argv[i], "-novalid")) ||
                   (!strcmp(argv[i], "--novalid"))) {
            novalid++;
        } else if ((!strcmp(argv[i], "-noout")) ||
                   (!strcmp(argv[i], "--noout"))) {
            noout++;
#ifdef LIBXML_DOCB_ENABLED
        } else if ((!strcmp(argv[i], "-docbook")) ||
                   (!strcmp(argv[i], "--docbook"))) {
            docbook++;
#endif
#ifdef LIBXML_HTML_ENABLED
        } else if ((!strcmp(argv[i], "-html")) ||
                   (!strcmp(argv[i], "--html"))) {
            html++;
#endif
        } else if ((!strcmp(argv[i], "-timing")) ||
                   (!strcmp(argv[i], "--timing"))) {
            timing++;
        } else if ((!strcmp(argv[i], "-profile")) ||
                   (!strcmp(argv[i], "--profile"))) {
            profile++;
        } else if ((!strcmp(argv[i], "-norman")) ||
                   (!strcmp(argv[i], "--norman"))) {
            profile++;
        } else if ((!strcmp(argv[i], "-nonet")) ||
                   (!strcmp(argv[i], "--nonet"))) {
            xmlSetExternalEntityLoader(xmlNoNetExternalEntityLoader);
#ifdef LIBXML_CATALOG_ENABLED
        } else if ((!strcmp(argv[i], "-catalogs")) ||
                   (!strcmp(argv[i], "--catalogs"))) {
            const char *catalogs;

            catalogs = getenv("SGML_CATALOG_FILES");
            if (catalogs == NULL) {
                fprintf(stderr, "Variable $SGML_CATALOG_FILES not set\n");
            } else {
                xmlLoadCatalogs(catalogs);
            }
#endif
#ifdef LIBXML_XINCLUDE_ENABLED
        } else if ((!strcmp(argv[i], "-xinclude")) ||
                   (!strcmp(argv[i], "--xinclude"))) {
            xinclude++;
            xsltSetXIncludeDefault(1);
#endif
        } else if ((!strcmp(argv[i], "-param")) ||
                   (!strcmp(argv[i], "--param"))) {
            i++;
            params[nbparams++] = argv[i++];
            params[nbparams++] = argv[i];
            if (nbparams >= 16) {
                fprintf(stderr, "too many params\n");
                return (1);
            }
        } else if ((!strcmp(argv[i], "-maxdepth")) ||
                   (!strcmp(argv[i], "--maxdepth"))) {
            int value;

            i++;
            if (sscanf(argv[i], "%d", &value) == 1) {
                if (value > 0)
                    xsltMaxDepth = value;
            }

	    /*---------------------------------------- */
	    /*     Handle xsldbg specific options      */
	    /*---------------------------------------- */

	} else if ((!strcmp(argv[i], "-shell")) ||
                   (!strcmp(argv[i], "--shell"))) {
            shell++;
	}else if ((!strcmp(argv[i], "-cd")) ||
		  (!strcmp(argv[i], "--cd"))){
	  char *workingDir ="<None specified>";
	  int parsed_dir_ok = 0;
	  if (i +1 < argc){
	    i++;
	    workingDir = argv[i];
	    if (chdir(workingDir) == 0){
	      parsed_dir_ok ++;
	    }
	  }
	  if (!parsed_dir_ok)
	    printf("Unable to change to directory %s\n", workingDir); 
	  else
	    printf("Change to directory %s\n", workingDir); 

	} else if ((!strcmp(argv[i], "-root")) ||
		   (!strcmp(argv[i], "--root"))) {
	  int parsed_root_ok = 0;
	  if ((i + 1)  < argc){
	    i++;
	    strcpy(root_template_name, argv[i]);
	    parsed_root_ok++;
	  }
	  if (!parsed_root_ok){
	    printf("Missing root template name after --root option\n");
	  }
	}

	else {
	  fprintf(stderr, "Unknown option %s\n", argv[i]);
	  usage(argv[0]);
	  return (1);
	}
    }
    params[nbparams] = NULL;

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
    if (!shell){ /* excecute stylesheet (ie no debugging)*/
      xslDebugStatus = DEBUG_RUN;      
    }

    xslDebugGotControl(0);
  while (xslDebugStatus != DEBUG_QUIT){
    if (shell)
      fprintf(stderr, "Starting stylesheet\n\n");
    if (xslDebugStatus != DEBUG_RUN)
      xslDebugStatus = DEBUG_CONT;
    for (i = 1; i < argc; i++) {
      if ((!strcmp(argv[i], "-maxdepth")) ||
	  (!strcmp(argv[i], "--maxdepth"))) {
	i++;
	continue;
      } else if ((!strcmp(argv[i], "-o")) ||
		 (!strcmp(argv[i], "-output")) ||
		 (!strcmp(argv[i], "--output"))) {
	i++;
	continue;
      }
      if ((!strcmp(argv[i], "-param")) || (!strcmp(argv[i], "--param"))) {
	i += 2;
	continue;
      }
      if ((!strcmp(argv[i], "-cd")) || 
	      (!strcmp(argv[i], "--cd"))) {
	i++;
	continue;
      }
      if ((!strcmp(argv[i], "-root")) || 
	      (!strcmp(argv[i], "--root"))) {    
	i++;
	continue;
      }

      if ((argv[i][0] != '-') || (strcmp(argv[i], "-") == 0)) {
	if (timing && !shell)
	  gettimeofday(&begin, NULL);
	style = xmlParseFile((const char *) argv[i]);
	if (timing && !shell) {
	  long msec;

	  gettimeofday(&end, NULL);
	  msec = end.tv_sec - begin.tv_sec;
	  msec *= 1000;
	  msec += (end.tv_usec - begin.tv_usec) / 1000;
	  fprintf(stderr, "Parsing stylesheet %s took %ld ms\n",
		  argv[i], msec);
	}
	if (style == NULL) {
	  fprintf(stderr,  "cannot parse %s\n", argv[i]);
	  if (shell)
	    fprintf(stderr, "Aborting debugger!!\n");
	  else
	    fprintf(stderr, "\n");
	  cur = NULL;
	  xslDebugStatus = DEBUG_QUIT;
	  goto done;
	} else {
	  cur = xsltLoadStylesheetPI(style);
	  if (cur != NULL) {
	    /* it is an embedded stylesheet */
	    xsltProcess(style, cur, argv[i]);
	    xsltFreeStylesheet(cur);
	    goto done;
	  }
	  cur = xsltParseStylesheetDoc(style);
	  if (cur != NULL) {
	    if (cur->indent == 1)
	      xmlIndentTreeOutput = 1;
	    else
	      xmlIndentTreeOutput = 0;
	    i++;
	  } else {
	    xmlFreeDoc(style);
	    goto done;
	  }
	}
	break;

      }
    }

    /*
     * disable CDATA from being built in the document tree
     */
    xmlDefaultSAXHandlerInit();
    xmlDefaultSAXHandler.cdataBlock = NULL;

    if ((cur != NULL) && (cur->errors == 0)) {
      for (; i < argc; i++) {
	doc = NULL;
	if (timing && !shell)
	  gettimeofday(&begin, NULL);
#ifdef LIBXML_HTML_ENABLED
	if (html)
	  doc = htmlParseFile(argv[i], NULL);
	else
#endif
#ifdef LIBXML_DOCB_ENABLED
	  if (docbook)
	    doc = docbParseFile(argv[i], NULL);
	  else
#endif
	    doc = xmlParseFile(argv[i]);
	if (doc == NULL) {
	  fprintf(stderr, "unable to parse %s\n", argv[i]);
	  if (shell)
	    fprintf(stderr, "Aborting debugger!!\n");
	  else
	    fprintf(stderr, "\n");

	  xslDebugStatus = DEBUG_QUIT;
	  break;
	}

	if (timing && !shell) {
	  long msec;

	  gettimeofday(&end, NULL);
	  msec = end.tv_sec - begin.tv_sec;
	  msec *= 1000;
	  msec += (end.tv_usec - begin.tv_usec) / 1000;
	  fprintf(stderr, "Parsing document %s took %ld ms\n",
		  argv[i], msec);
	}

	if (shell){
	  int result = 0;
	  xmlNodePtr templ = xslFindTemplateNode(cur, root_template_name);
	  if (templ && templ->doc){
	    if (!xslFindBreakPointByName(root_template_name)){
	      result =  xslAddBreakPoint(templ->doc->URL, xmlGetLineNo(templ), root_template_name, DEBUG_BREAK_SOURCE);
	    }else
	      result++;
	    xsltProcess(doc, cur, argv[i]);
	  }
	  if (!result){
	    printf("Root template '%s' not found or unable to set root break point\n" \
		   "Try one of  \n", root_template_name);	    
	    printTemplates(cur, doc);
	    
	    xslDebugStatus = DEBUG_QUIT;
	  }
	}else{
	  xsltProcess(doc, cur, argv[i]);
	}
      }
    }
    if (cur != NULL)
      xsltFreeStylesheet(cur);

  if (shell){
    if( (xslDebugStatus != DEBUG_QUIT) && !xslDebugGotControl(0) ){
      fprintf(stderr, "Debugger never received control setting breakpoint on all" \
	      " template names\n");
      strcpy(root_template_name, "*");
    }
    fprintf(stderr, "Finished stylesheet\n\n");
    }else{
      break; /* request to execute stylesheet only */
    }
  }
 done:

    xslDebugFree();

    xsltCleanupGlobals();
    xmlCleanupParser();
    xmlMemoryDump();
    return (0);
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


