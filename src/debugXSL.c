/***************************************************************************
                          debugXSL.c  -  description
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
 * Orinal file : debugXML.c : This is a set of routines used for debugging the tree
 *              produced by the XML parser.
 * New file : debugXSL.c : Debug support version
 *
 * See Copyright for the status of this software.
 *
 * Daniel Veillard <daniel@veillard.com>
 * 
 * Permission abtained to modify the LGPL'd code and extend to include breakpoints, inspections of
 * stylesheet source, xml data, stylesheet variables
 */

/* --- Start specificaly xsldbg related code by KPI -- */
#include "xsldbg.h"
#include "files.h"
#include "debugXSL.h"
#include <breakpoint/breakpoint.h>
#include "help.h"
#include <ctype.h>

#define DEBUG_BUFFER_SIZE 500 /*used by xslDbgShell */

/* current template being processed */
xsltTemplatePtr rootCopy;
 
xmlChar *commandNames[] = \
  {"help",  /* valid commands of xslDbgShell*/
			   
   "bye",
   "exit",
   "quit",
		      
   "step", 
   "stepup", 
   "stepdown", 
   "continue", 
   "run",
		      
   "templates", 
   "where", 
		      
   "break", 
   "showbreak", 
   "delete",
   "enable",
   "disable",

   "ls",
   "dir", 
   "du", 
   "cat",
   "print",
   "pwd",
   "dump",
   "base",

   "globals", 
   "locals", 
   /* "cat", already listed */
   "source", 
   "data", 
   "cd",

   "validate", 
   "load", 
   "save", 
   "write", 
   "free",
   "chdir",

   NULL /* Indicate the end of list*/ 
} ; 

xmlChar *shortCommandNames[] = \
  {"h",  /* valid commands of xslShell*/
			   
   "bye",
   "exit", 
   "q", /*quit */
		      
   "s", /* step*/
   "up",   /*stepup */ 
   "down", /* stepdown*/ 
   "c", /* cont*/ 
   "r", /* run*/
		      
   "t", /* templates*/
   "w",  /* where */
		      
   "b", /* break*/
   "show", 
   "d", /* delete*/
   "e", /* enabled*/
   "disable",

   "ls",
   "dir", 
   "du", 
   "cat",
   "print",
   "pwd",
   "dump",
   "base",

   "globals", 
   "locals", 
   /* "cat", already listed */
   "source", 
   "data", 
   "cd",

   "validate", 
   "load", 
   "save", 
   "write", 
   "free",
   "chdir",

   NULL /* Indicate the end of list*/ 
} ; 

enum { DEBUG_HELP_CMD = 100, /* id's for commands of xslDbgShell*/
		      
       DEBUG_BYE_CMD,
       DEBUG_EXIT_CMD,
       DEBUG_QUIT_CMD,
		      
       DEBUG_STEP_CMD, 
       DEBUG_STEPUP_CMD, 
       DEBUG_STEPDOWN_CMD, 
       DEBUG_CONT_CMD, 
       DEBUG_RUN_CMD,
		      
       DEBUG_TEMPLATES_CMD, 
       DEBUG_WHERE_CMD, 
		      
       DEBUG_BREAK_CMD, 
       DEBUG_SHOWBREAK_CMD, 
       DEBUG_DELETE_CMD,
       DEBUG_ENABLE_CMD,
       DEBUG_DISABLE_CMD,

       DEBUG_LS_CMD,
       DEBUG_DIR_CMD, 
       DEBUG_DU_CMD, 
       DEBUG_CAT_CMD,
       DEBUG_PRINT_CMD, /* cat alternative */
       DEBUG_PWD_CMD,
       DEBUG_DUMP_CMD,
       DEBUG_BASE_CMD,

       DEBUG_GLOBALS_CMD, 
       DEBUG_LOCALS_CMD, 
       /*     DEBUG_CAT_CMD, already listed */
       DEBUG_SOURCE_CMD, 
       DEBUG_DATA_CMD, 
       DEBUG_CD_CMD,

       DEBUG_VALIDATE_CMD, 
       DEBUG_LOAD_CMD, 
       DEBUG_SAVE_CMD, 
       DEBUG_WRITE_CMD, 
       DEBUG_FREE_CMD,
       DEBUG_CHDIR_CMD,

       /*NULL */} ;


/* some convenient short cuts when using  cd command*/
xmlChar *cdShortCuts[] = \
  {"<<", 
   ">>",
   "<-",
   "->",
   NULL /* indicate end of list*/
  };

/* what to replace shortcuts with */
xmlChar *cdAlternative[] = \
  { "preceding-sibling::node()",
    "following-sibling::node()",
    "ancestor::node()",
    "descendant::node()"
  };

/* what enum to use for shortcuts */
enum {DEBUG_PREV_SIBLING = 200,
      DEBUG_NEXT_SIBLING,
      DEBUG_ANCESTOR_NODE,
      DEBUG_DESCENDANT_NODE
} ;

#include <libxml/xpathInternals.h>

#include <libxslt/extra.h>
#include <string.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif

/* Added readline and history support KPI */
#ifdef HAVE_READLINE
#include <readline/readline.h>
#ifdef HAVE_HISTORY
#include <readline/history.h>
#endif
#endif

#include <libxml/xmlmemory.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <libxml/valid.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/HTMLparser.h>
#include <libxml/xmlerror.h>


#define IS_BLANK(c)							\
  (((c) == '\n') || ((c) == '\r') || ((c) == '\t') || ((c) == ' '))

#define INPUT_BUFFER_SIZE 501 


/**
 * xslDbgShellReadline:
 * @prompt:  the prompt value
 *
 * Read a string
 * 
 * Returns a pointer to it or NULL on EOF the caller is expected to
 *     free the returned string.
 */

char *
xslDbgShellReadline(char *prompt) {
  static char last_read[INPUT_BUFFER_SIZE] = {'\0'};
#ifdef HAVE_READLINE
    char *line_read;

    /* Get a line from the user. */
    line_read = readline (prompt);

    /* If the line has any text in it, save it on the history. */
    if (line_read && *line_read){
      add_history (line_read);
      strncpy(last_read, line_read, INPUT_BUFFER_SIZE -1 );
    }else{
      /* if only <Enter>is pressed then try last saved command line */
      line_read = strdup(last_read);
    }
    return (line_read);
#else
    char line_read[INPUT_BUFFER_SIZE];

    if (prompt != NULL)
	fprintf(stdout, "%s", prompt);
    if (!fgets(line_read, INPUT_BUFFER_SIZE -1, stdin))
        return(NULL);
    line_read[INPUT_BUFFER_SIZE -1] = 0;
    /* if only <Enter>is pressed then try last saved command line */
    if ((strlen(line_read) == 0) || (line_read[0] =='\n')){
      strcpy(line_read, last_read);
    }else{
      strcpy(last_read, line_read);
    }
    return(strdup(line_read));
#endif
}


/****************************************************************
 *								*
 *	 	The XSL shell related functions			*
 *								*
 ****************************************************************/

/**
 * xslFrameBreak:
 *@arg : non-null 
 *@stepup : if != 1 then we step up, otherwise step down
 *
 * Set a "frame" breakpoint either up or down from here
 */
int
xslDbgShellFrameBreak(xmlChar *arg, int stepup){
  int result = 0;
  // how many frames to go up/down
  int noOfFrames;
  static char *errorPrompt =  "Failed to set frame break point\n";
  if (!arg){
    return result;
  }

  if (strlen(arg) >0){
    if (!sscanf(arg, "%d", &noOfFrames)){
      fprintf(stderr, "%s\tUnable to read number of frames \n", errorPrompt);
      return result;
    }
  }else{
    noOfFrames = 0;
  }

  if (stepup){
    result = xslStepupToDepth(xslCallDepth() - noOfFrames);
  }else{
    result = xslStepdownToDepth(xslCallDepth() + noOfFrames); 
  }
  return result;
}

/**
 * xslDbgShellBreak:
 * @arg : non-null
 * 
 * Add break point specified by arg
 */
int
xslDbgShellBreak(xmlChar *arg, xsltStylesheetPtr style){
  int result = 0;
  long lineNo;
  static char *errorPrompt =  "Failed to add break point\n";
  if (!arg || !style){
    fprintf(stderr, "Debuger has no files loaded, try reloading files\n");
    return result;
  }
  if (arg[0] == '-'){
    xmlChar *opts[2];
    if ( (strlen(arg) > 1) && (arg[1] == 'l') ){
      if (splitString(&arg[2], 2, opts) == 2){
	if (!sscanf(opts[1], "%ld", &lineNo)){
	  fprintf(stderr, "%s\tUnable to read line number \n", errorPrompt);
	  return result;
	}else{
	  int type; 
	  /* try to guess whether we are looking for source or data 
	     break point
	   */
	  if(strstr(opts[0],".xsl"))
	    type = DEBUG_BREAK_SOURCE;
	  else
	    type = DEBUG_BREAK_DATA;
	  if (!xslAddBreakPoint(opts[0], lineNo, NULL, type))
	    fprintf(stderr, "%s", errorPrompt);
	  else
	    result++;
	}
      }else
	fprintf(stderr, "%s\tMissing arguments to break command\n", 
		errorPrompt);
    }
  }else if (strcmp(arg, "*") != 0){
    xmlNodePtr templNode = xslFindTemplateNode(style, arg);
    if (templNode && templNode->doc){
      if (!xslAddBreakPoint(templNode->doc->URL, xmlGetLineNo(templNode), arg, DEBUG_BREAK_SOURCE))
	fprintf(stderr,  "%s\tBreak point to template '%s' exists \n", 
		errorPrompt, arg);
      else
	result++;
    }else
      fprintf(stderr,  "%s\tUnable to find template '%s' \n", 
	      errorPrompt, arg);	  
  }else{
    /* add all template names*/
    const xmlChar *name, *defaultUrl = "<n/a>";
    const xmlChar *url;
    int templateCount = 0;    
    xsltTemplatePtr templ;
    while (style){
      templ = style->templates;
      while (templ){        
	if (templ->elem && templ->elem->doc && templ->elem->doc->URL){
	  url = templ->elem->doc->URL;
	}else{
	  url = defaultUrl;
	}
	if (templ->match)
	  name = templ->match;
	else 
	  name = templ->name;
      
	if (name){
	  if (!xslAddBreakPoint(url, xmlGetLineNo(templ->elem), 
				name, DEBUG_BREAK_SOURCE))
	    fprintf(stderr,  "%s\tBreak point to template '%s' exists \n", 
		    errorPrompt, name);
	  else
	    templateCount++;
	}
	templ = templ->next;
      }
      if (style->next)
	style = style->next;
      else
	style = style->imports;
    }
    if (templateCount == 0){
      fprintf(stderr, "No templates found or unable to add any break points\n ");    
    }
  }
  
  return result;
}


/**
 * xslDbgShellDelete:
 * @arg : non-null
 * 
 * Delete break point specified by arg
 */
int
xslDbgShellDelete(xmlChar *arg){
  int result = 0, breakPointNumber, breakPointId;
  long lineNo;
  xmlChar *fileName;
  static char *errorPrompt =  "Failed to delete break point\n";
  if (!arg)
    return result;

  if (arg[0] == '-'){
    xmlChar *opts[2];
    if ( (strlen(arg) > 1) && (arg[1] == 'l')){
      if (splitString(&arg[2], 2, opts) == 2){
	if (!sscanf(opts[1], "%ld", &lineNo)){
	  fprintf(stderr, "\n%s\tUnable to read line number \n", errorPrompt);
	}else{
	  breakPointNumber = xslFindBreakPointByLineNo(opts[0], lineNo);
	  if (!breakPointNumber ||  !xslDeleteBreakPoint(breakPointNumber))
	    fprintf(stderr, "\n%s\tBreak point to '%s' doesn't exist\n", 
		  errorPrompt, arg);
	  else
	    result++;
	}
      }else
	fprintf(stderr,  "\n%s\tMissing arguments to delete command\n", 
		errorPrompt);
    }
  }else  if (!strcmp("*", arg)){
    result = 1;
       /*remove all from breakpoints */
      while(xslBreakPointCount()){
	if (!xslDeleteBreakPoint(1)){
	  result = 0;
	  break;
	}
      }
  }else if (sscanf(arg, "%d", &breakPointId)){
    breakPointNumber =  xslFindBreakPointById(breakPointId);
    if (breakPointNumber){
      result =  xslDeleteBreakPoint(breakPointNumber);
      if (!result){
	fprintf(stderr, "\nUnable to delete breakpoint %d\n" , breakPointNumber);
      }
    }else{
      fprintf(stderr, "\nBreakpoint %d doesn't exist\n",breakPointId);
    }
  }else {	
      breakPointNumber = xslFindBreakPointByName(arg);
      if (breakPointNumber){
	result = xslDeleteBreakPoint(breakPointNumber);
	if (!result){
	  fprintf(stderr,"\nDelete breakpoint to template %s failed\n", arg);
	}
      }else
	fprintf(stderr, "%s\tBreakpoint to template '%s' doesn't exist\n", 
		errorPrompt, arg);
    }
  return result;
}

/**
 * xslDbgShellEnable:
 * @arg : non-null
 * 
 * Enable/disable break point specified by arg
 */
int
xslDbgShellEnable(xmlChar *arg, int enableType){
  int result = 0, breakPointNumber, breakPointId;
  long lineNo;
  xmlChar *fileName;
  static char *errorPrompt =  "Failed to enable/disable break point\n";
  if (!arg)
    return result;

  if (arg[0] == '-'){
    xmlChar *opts[2];
    if ( (strlen(arg) > 1) && (arg[1] == 'l')){
      if (splitString(&arg[2], 2, opts) == 2){
	if (!sscanf(opts[1], "%ld", &lineNo)){
	  fprintf(stderr, "\n%s\tUnable to read line number \n", errorPrompt);
	}else{
	  breakPointNumber = xslFindBreakPointByLineNo(opts[0], lineNo);
	  if (!breakPointNumber)
	    fprintf(stderr, "\n%s", errorPrompt);
	  else{
	    result = xslEnableBreakPoint(breakPointNumber, enableType);
	  }
	}
      }else
	fprintf(stderr,  "\n%s\tMissing arguments to enable command\n",
		errorPrompt);
    }
  }else if (!strcmp("*", arg)){
    int index;
    result = 1;
      /*enable/disable all from breakpoints */
      for (index = 1; index <= xslBreakPointCount(); index++){
	if (!xslEnableBreakPoint(index, enableType)){
	  result = 0;
	  break;
	}
      }
  }else if (sscanf(arg, "%d", &breakPointId)){
    breakPointNumber =  xslFindBreakPointById(breakPointId);
    if (breakPointNumber){
      result =  xslEnableBreakPoint(breakPointNumber, enableType);
      if (!result){
	fprintf(stderr, "\nUnable to enable breakpoint %d\n" , breakPointNumber);
      }
    }else{
      fprintf(stderr, "\nBreakpoint %d doesn't exist\n", breakPointId);
    }
  }else{
      breakPointNumber = xslFindBreakPointByName(arg);
      if (breakPointNumber){
	result =xslEnableBreakPoint(breakPointNumber, enableType);
      }else
	fprintf(stderr, "\n%s\tBreakpoint to template '%s' doesn't exist\n", 
		errorPrompt, arg);      
  }	    
  return result;
}


/* print list of nodes in either ls or dir format
 * @ctxt : the current shell context
 * @arg : what path to display
 * @dir : print in dir mode? 
 */
void 
xslDbgPrintList(xmlShellCtxtPtr ctxt, xmlChar *arg, int dir){
  xmlXPathObjectPtr list;
  if (!ctxt || !arg){
    fprintf(stderr, "Debuger has no files loaded, try reloading files\n");
    return;
  }

  if (arg[0] == 0) {
    if (dir)
      xmlShellDir(ctxt, NULL, ctxt->node, NULL);
    else
      xmlShellList(ctxt, NULL, ctxt->node, NULL);
  } else {
    ctxt->pctxt->node = ctxt->node;
    ctxt->pctxt->node = ctxt->node;
    if (!xmlXPathNsLookup(ctxt->pctxt, "xsl"))
      xmlXPathRegisterNs(ctxt->pctxt, "xsl", XSLT_NAMESPACE);
    list = xmlXPathEval((xmlChar *) arg, ctxt->pctxt);
    if (list != NULL) {
      switch (list->type) {
      case XPATH_NODESET: {
	int indx;

	for (indx = 0;indx < list->nodesetval->nodeNr;
	     indx++) {
	  if (dir)
	    xmlShellDir(ctxt, NULL,
			list->nodesetval->nodeTab[indx], NULL);
	  else
	    xmlShellList(ctxt, NULL,
			 list->nodesetval->nodeTab[indx], NULL);
	}
	break;
      }
      default:
	xmlShellPrintXPathError(list->type, arg);
      }
      xmlXPathFreeObject(list);
    } else {
      xmlGenericError(xmlGenericErrorContext,
		      "%s: no such node\n", arg);
    }
    ctxt->pctxt->node = NULL;
  }  
}



/* xslDbgCat :
 * print the result of an xpath expression. This can include variables
 * if styleCtxt is not NULL
 * @styleCtxt : current stylesheet context
 * @ctxt : current shell context
 * @name : path to change to 
 */
void xslDbgCat(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,\
	      xmlChar *arg){
  xmlXPathObjectPtr list;
  int i = 0;
  if (!ctxt){
    fprintf(stderr, "Debuger has no files loaded, try reloading files\n");
    return;
  }
  if (arg == NULL)
    arg = "";
  if (arg[0] == 0) {
    xmlShellCat(ctxt, NULL, ctxt->node, NULL);
  } else {
    ctxt->pctxt->node = ctxt->node;
    if (!styleCtxt){
      list = xmlXPathEval((xmlChar *) arg, ctxt->pctxt);
    } else{
      xmlNodePtr savenode = styleCtxt->xpathCtxt->node;
      ctxt->pctxt->node = ctxt->node;
      styleCtxt->xpathCtxt->node = ctxt->node;
      if (!xmlXPathNsLookup(styleCtxt->xpathCtxt, "xsl"))
	xmlXPathRegisterNs(styleCtxt->xpathCtxt, "xsl", XSLT_NAMESPACE);
      list = xmlXPathEval((xmlChar *) arg, styleCtxt->xpathCtxt);
      styleCtxt->xpathCtxt->node = savenode;
    }
    if (list != NULL) {
      switch (list->type) {
      case XPATH_NODESET: {
	int indx;
	if (list->nodesetval){
	  for (indx = 0;indx < list->nodesetval->nodeNr;
	       indx++) {
	    if (i > 0) fprintf(stderr, " -------\n");
	    xmlShellCat(ctxt, NULL,
		      list->nodesetval->nodeTab[indx], NULL);
	  }
	}else{
	  xmlGenericError(xmlGenericErrorContext,
			"xpath %s: results an in empty set\n", arg);
	}
	break;
      }

      case XPATH_BOOLEAN:
	xmlGenericError(xmlGenericErrorContext,
			"%s is a Boolean:%s\n", arg,
			xmlBoolToText(list->boolval));
	break;
      case XPATH_NUMBER:
	xmlGenericError(xmlGenericErrorContext,
			"%s is a number:%0g\n", arg, list->floatval);
	break;
      case XPATH_STRING:
	xmlGenericError(xmlGenericErrorContext,
			"%s is a string:%s\n", arg, list->stringval);
	break;

      default:
	xmlShellPrintXPathError(list->type, arg);
      }
      xmlXPathFreeObject(list);
    } else {
      xmlGenericError(xmlGenericErrorContext,
		      "%s: no such node\n", arg);
    }
    ctxt->pctxt->node = NULL;
  }
}

int varCount;

/*
 * xslDbgPrintNames:
 * Print a name of variable found by scanning variable table
 * It is used by print_variable function.
 * @payload : 
 * @data : 
 * @name : the variable name 
 */
static void 
*xslDbgPrintNames(void *payload, void *data, xmlChar *name){
  if (varCount)
    fprintf(stderr, ", %s", name);
  else
    fprintf(stderr, "%s", name);    
  varCount++;
  return NULL;
}



/* xslDbgPrintVariable:
 *   Print the value variable specified by args
 * @styleCtxt : the current stylesheet context 
 * @arg : the name of variable to look for
 */
static void 
xslDbgPrintVariable(xsltTransformContextPtr styleCtxt, xmlChar *arg, 
		    int type){
  varCount= 0;
  if (!styleCtxt) {
    fprintf(stderr, "Debuger has no files loaded, try reloading files\n");
    return;
  }

  if (arg[0] == 0){
    /* list variables of type requested */
    if (type == DEBUG_PRINT_GLOBAL_VAR ){
      /* list global variables */
      fprintf(stderr, "\nGlobal variables found: ");
      xmlHashScan(styleCtxt->globalVars, xslDbgPrintNames, NULL);
    }else{
      /* list local variables */
      xsltStackElemPtr item = styleCtxt->varsTab[styleCtxt->varsBase];
      fprintf(stderr, "\nLocal variables found: ");
      while (item ){
	fprintf(stderr, "%s ", item->name);
	item = item->next;
      }
    }
    fprintf(stderr, "\n");    
  }else{
    /* Display the value of variable */
    if (arg[0] =='$')
     xmlShellPrintXPathResult(xmlXPathEval(arg, styleCtxt->xpathCtxt));
    else{
      char tempbuff[100];
      strcpy(tempbuff, "$");
      strcat(tempbuff, arg);
      xmlShellPrintXPathResult(xmlXPathEval(tempbuff, styleCtxt->xpathCtxt)); 
    }    
  }
} 



/* xslDbgCd :
 * Change directories
 * @styleCtxt : current stylesheet context
 * @ctxt : current shell context
 * @name : path to change to 
 */
void xslDbgCd(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,\
	      xmlChar *arg, xmlNodePtr source){
  xmlXPathObjectPtr list = NULL;
  int index = 2; /* in some cases I'm only interested after first two chars  */
  if (!ctxt){
    fprintf(stderr, "Debuger has no files loaded, try reloading files\n");
    return;
  }
  if (arg == NULL)
    arg = "";
  if (arg[0] == 0) {
    ctxt->node = (xmlNodePtr) ctxt->doc;
  } else {
    if ((arg[0] =='-')  && (strlen(arg) >2) ) {
      if (styleCtxt){ 
	if (arg[1] =='t'){
	  xmlNodePtr templateNode;
	  /* quickly find a template*/

	  /* skip any white spaces*/
	  while (isspace(arg[index]))
	    index++;

	  templateNode =xslFindTemplateNode(styleCtxt->style, &arg[index]);
	  if (!templateNode){
	    fprintf(stderr, "Template '%s' not found\n", &arg[index]);
	    return;
	  }else{
	    fprintf(stderr, " template :\"%s\"\n", &arg[index]);	
	    ctxt->pctxt->node;
	    return;
	  }
	}else if (arg[1] =='s'){
	  /*quickly switch to another stylesheet node */
	  xmlXPathContextPtr pctxt = xmlXPathNewContext(source->doc);
	  if (pctxt == NULL) {
	    xmlFree(ctxt);
	    xslDebugStatus = DEBUG_QUIT;
	    return;
	  }
	  if (!xmlXPathNsLookup(pctxt, "xsl"))
	    xmlXPathRegisterNs(pctxt, "xsl", XSLT_NAMESPACE);
	  list = xmlXPathEval((xmlChar *) &arg[index], pctxt); 
	  if (pctxt){
	    xmlFree(pctxt);
	  }
	}else{
	  printf("Unknown option to cd\n");
	}
      }else
	printf("Unable to cd, No stylesheet properly parsed\n");
    } else {
      xmlNodePtr savenode;
      if (styleCtxt){
	savenode= styleCtxt->xpathCtxt->node;
	ctxt->pctxt->node = ctxt->node;
	styleCtxt->xpathCtxt->node = ctxt->node;
	if (!xmlXPathNsLookup(ctxt->pctxt, "xsl"))
	  xmlXPathRegisterNs(ctxt->pctxt, "xsl", XSLT_NAMESPACE);
	list = xmlXPathEval((xmlChar *) arg, styleCtxt->xpathCtxt); 	
	styleCtxt->xpathCtxt->node = savenode;
      }else if (ctxt->pctxt){
	if (!xmlXPathNsLookup(ctxt->pctxt, "xsl"))
	  xmlXPathRegisterNs(ctxt->pctxt, "xsl", XSLT_NAMESPACE);
	 list = xmlXPathEval((xmlChar *) arg, ctxt->pctxt);
      }else{
	printf("Invalid parameters to xslDbgCd\n");	
      }
    }

    if (list != NULL) {
      switch (list->type) {
      case XPATH_NODESET:
	if (list->nodesetval){
	  if (list->nodesetval->nodeNr == 1) {
	    ctxt->node = list->nodesetval->nodeTab[0];
	  } else 
	    xmlGenericError(xmlGenericErrorContext,
			  "%s is a %d Node Set\n",
			  arg, list->nodesetval->nodeNr);
	}else{
	    xmlGenericError(xmlGenericErrorContext,
			    "%s is a empty Node Set\n ", arg);
	}
	break;
	
      default:
	xmlShellPrintXPathError(list->type, arg);
      }
      xmlXPathFreeObject(list);
    } else {
      xmlGenericError(xmlGenericErrorContext,
		      "%s: no such node\n", arg);
    }
    ctxt->pctxt->node = NULL;
  }
}

/* invert the order of printin template names */
int xslDbgPrintTemplateHelper(xsltTemplatePtr templ, int verbose, int templateCount){
  const xmlChar *name, *defaultUrl = "<n/a>";
  const xmlChar *url;
    if (templ){
      templateCount = xslDbgPrintTemplateHelper(templ->next, verbose, templateCount + 1);
      if (templ->elem && templ->elem->doc && templ->elem->doc->URL){
	url = templ->elem->doc->URL;
      }else{
	url = defaultUrl;
      }
      if (templ->match)
	name = templ->match;
      else 
	name = templ->name;
      
      if (name){
	if (verbose)
	  fprintf(stderr, " template :\"%s\" in file %s : line %ld\n", 
		name, url, xmlGetLineNo(templ->elem));
	else
	  fprintf(stderr, "\"%s\" ", name);
      }
      templ = templ->next;
    }
    return templateCount;
}

/* 
 * @verbose : if true then print out line info
 */
void xslDbgPrintTemplateNames(xsltTransformContextPtr styleCtxt, xmlShellCtxtPtr ctxt,\
	      xmlChar *arg, int verbose, int allFiles){
  int templateCount = 0;
  xsltStylesheetPtr curStyle;
  xsltTemplatePtr templ;
  if (!styleCtxt){
      xmlGenericError(xmlGenericErrorContext,
		      "Null styleCtxt supplied to xslDbgPrintTemplateNames\n");
      return;
  }
    
  if (allFiles)
    curStyle = styleCtxt->style;
  else{
    /* try to find files in the current stylesheet */
    if (rootCopy)
      curStyle = rootCopy->style;
    else
      curStyle = NULL;
  }
    
  while (curStyle){
    templ = curStyle->templates;
    /* print them out in the order their in the file */
    templateCount = xslDbgPrintTemplateHelper(templ, verbose, templateCount);
    fprintf(stderr, "\n");    
    if (curStyle->next)
      curStyle = curStyle->next;
    else
      curStyle = curStyle->imports;

  }
  if (templateCount == 0){
     fprintf(stderr, "No templates found\n ");    
  }
}

void 
xslDbgPrintCallStack(){
  int index;
  xmlNodePtr node;
  xmlChar *name;
  xmlChar buff[300];
  xslCallPointPtr callPoint;
  for (index = 1; index <= xslCallDepth(); index++){
    callPoint = xslGetCall(index); 
    if (callPoint && callPoint->info){
      if (index == 0)
	fprintf(stderr, "Call stack contains:\n");
      fprintf(stderr, " template :\"%s\"", callPoint->info->templateName);
      if (callPoint->info->url)
	fprintf(stderr, " in file %s : line %ld \n", callPoint->info->url,
		callPoint->lineNo);
      else
	fprintf(stderr, "\n");
    }else{
#ifdef WITH_XSLT_DEBUG_PROCESS
      xsltGenericError(xsltGenericErrorContext,
		       "Call stack item not found at depth %d :" \
		       " xslDbgPrintCallStack\n", index);
#endif
      break;
    }
  }
  if (xslCallDepth() == 0)
    fprintf(stderr, "No items on call stack\n");
  else
    fprintf(stderr, "\n");
}


/* Look up a name in a list  
   Stop looking when you reach a null entry in matchList
*/
int lookupName(xmlChar *name, xmlChar **matchList){
  int result = -1, index;
  for (index = 0; matchList[index]; index++){
    if (!strcmp(name, matchList[index])){
      result = index;
      break;
    }
  }
    
  return result;
}


/**
 * xslShellReadline:
 * @prompt:  the prompt value
 *
 * Read a string
 *
 * Returns a pointer to it or NULL on EOF the caller is expected to
 *     free the returned string.
 */

char *
xslShellReadline(char *prompt) {
  static char last_read[INPUT_BUFFER_SIZE] = {'\0'};
#ifdef HAVE_READLINE
    char *line_read;

    /* Get a line from the user. */
    line_read = readline (prompt);

    /* If the line has any text in it, save it on the history. */
    if (line_read && *line_read){
      add_history (line_read);
      strncpy(last_read, line_read, INPUT_BUFFER_SIZE -1 );
    }else{
      /* if only <Enter>is pressed then try last saved command line */
      line_read = strdup(last_read);
    }
    return (line_read);
#else
    char line_read[INPUT_BUFFER_SIZE];

    if (prompt != NULL)
	fprintf(stdout, "%s", prompt);
    if (!fgets(line_read, INPUT_BUFFER_SIZE -1, stdin))
        return(NULL);
    line_read[INPUT_BUFFER_SIZE -1] = 0;
    /* if only <Enter>is pressed then try last saved command line */
    if ((strlen(line_read) == 0) || (line_read[0] =='\n')){
      strcpy(line_read, last_read);
    }else{
      strcpy(last_read, line_read);
    }
    return(strdup(line_read));
#endif
}

int splitString(xmlChar *textIn, int maxStrings, xmlChar**out){
  int result = 0;
  while((*textIn != '\0') && (result < maxStrings)){
    /*skip the first spaces ?*/
    while(isspace(*textIn))
      textIn++;
    out[result] = textIn;

    /* no word found only spaces ?*/
    if (strlen(textIn) == 0)
      return result;

    /* look for end of word */
    while(!isspace(*textIn) && (*textIn != '\0'))
      textIn++;      
    *textIn = '\0';
    textIn++;
    result++;
  }
  return result;
} 
/*
 * A break point has been found so pass control to user
 * @templ : The source node being executed
 * @node : The data node being processed
 * @root : The template being applide to "node"
 * @ctxt :
 */
void xslDebugBreak(xmlNodePtr templ, xmlNodePtr node,  xsltTemplatePtr root, \
		   xsltTransformContextPtr ctxt){
  rootCopy = root;
     if (root){
      if (root->match)
	fprintf(stderr, "\nReached template :\"%s\"\n\n", root->match);
      else if (root->name)
	fprintf(stderr, "\nReached template :\"%s\"\n\n", root->name);      
     }else{
       fprintf(stderr, "\n");
     } 
     if (ctxt && ctxt->node && ctxt->node && ctxt->node->doc && ctxt->node->doc->URL){
       if (xslActiveBreakPoint() > 0){
	 fprintf(stderr, "Breakpoint %d ", xslActiveBreakPoint());
       }else{
	 fprintf(stderr, "Breakpoint ");
       }
     }

     xslDbgShell(templ, node, "index.xsl", xslDbgShellReadline,stdout, ctxt);

}

/* Highly modified function based on xmlShell */
/**
 * xslDbgShell
 * @doc:  the initial document
 * @filename:  the output buffer
 * @input:  the line reading function
 * @output:  the output FILE*
 *
 * Implements the XSL shell 
 * Work with stylsheet and data in a similar way to Unix command shell 
 */
void
xslDbgShell(xmlNodePtr source, xmlNodePtr doc, char *filename, \
	 xmlShellReadlineFunc input, FILE *output, \
	 xsltTransformContextPtr styleCtxt) {
    char prompt[DEBUG_BUFFER_SIZE] = "/ > ";
    char *cmdline = NULL, *cur;
    int nbargs = 0;
    int commandId = -1; /* stores what was the last command id entered by user */
    char command[DEBUG_BUFFER_SIZE]; /* holds the command user entered */
    char arg[DEBUG_BUFFER_SIZE]; /* holds any extra arguments to command entered */
    char dir[DEBUG_BUFFER_SIZE]; /* temporary buffer used by where and pwd commands */
    int shortCutId = -1; /* used by cd command*/
    int i;
    long lineNo;
    static int showSource = 1; /* Do we first show source or data ?*/
    /* for convenience keep track of which node was last 
     selected of source and doc*/
    xmlNodePtr lastSourceNode, lastDocNode;


    xmlShellCtxtPtr ctxt;
    xmlXPathObjectPtr list;
    int exitShell = 0; /* Indicate when to exit xslShell*/    

    if (source ==NULL)
      return;
    if (doc == NULL)
        return;
    if (filename == NULL)
        return;
    if (input == NULL)
        return;
    if (output == NULL)
        return;
    ctxt = (xmlShellCtxtPtr) xmlMalloc(sizeof(xmlShellCtxt));
    if (ctxt == NULL) 
        return;

    /* flag that we've received control */
    xslDebugGotControl(1);
    ctxt->loaded = 0;
    lastSourceNode = source;
    lastDocNode = doc;
    /* show the doc or source first?*/
    if (showSource){
      ctxt->doc = source->doc;
      ctxt->node = source;
    }else{
      ctxt->doc = doc->doc;
      ctxt->node = (xmlNodePtr) doc;
    }
    ctxt->input = input;
    ctxt->output = output;
    ctxt->filename = (char *) xmlStrdup((xmlChar *) filename);
 
    if (ctxt->node && ctxt->node && ctxt->node->doc && ctxt->node->doc->URL)
      if (xslActiveBreakPoint() > 0){
	xslBreakPointPtr breakPtr = xslGetBreakPoint(xslActiveBreakPoint());
	fprintf(stderr, "in file %s : line %ld \n", breakPtr->url, breakPtr->lineNo);
      }else{
	if (xmlGetLineNo(ctxt->node) != -1)
	  fprintf(stderr, "at file %s : line %ld \n", ctxt->node->doc->URL, 
		  xmlGetLineNo(ctxt->node));
	else
	  fprintf(stderr, "@ text node in file %s\n", ctxt->node->doc->URL); 
      }
    else
      fprintf(stderr, "\n");


    ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
    if (ctxt->pctxt == NULL) {
	xmlFree(ctxt);
	return;
    }

    while (!exitShell) {
        if (ctxt->node == (xmlNodePtr) ctxt->doc)
	    snprintf(prompt,DEBUG_BUFFER_SIZE -1, "(xsldbg) %s > ", "/");
	else if ((ctxt->node->name) && (ctxt->node->ns))
	    snprintf(prompt,DEBUG_BUFFER_SIZE -1, "(xsldbg) %s:%s > ", ctxt->node->ns->prefix, ctxt->node->name);
	else if (ctxt->node->name)
	    snprintf(prompt, DEBUG_BUFFER_SIZE -1,"(xsldbg) %s > ", ctxt->node->name);

        else
	    snprintf(prompt, DEBUG_BUFFER_SIZE -1, "(xsldbg) ? > ");
        prompt[sizeof(prompt) - 1] = 0;

	/*
	 * Get a new command line
	 */
        cmdline = ctxt->input(prompt);
        if (cmdline == NULL) break;

	/*
	 * Parse the command itself
	 */
	cur = cmdline;
	nbargs = 0;
	while ((*cur == ' ') || (*cur == '\t')) cur++;
	i = 0;
	while ((*cur != ' ') && (*cur != '\t') &&
	       (*cur != '\n') && (*cur != '\r')) {
	    if (*cur == 0)
		break;
	    command[i++] = *cur++;
	}
	command[i] = 0;
	if (i == 0) continue;
	nbargs++;

	/*
	 * Parse the argument
	 */
	while ((*cur == ' ') || (*cur == '\t')) cur++;
	i = 0;
	while ((*cur != '\n') && (*cur != '\r') && (*cur != 0)) {
	    if (*cur == 0)
		break;
	    arg[i++] = *cur++;
	}
	arg[i] = 0;
	if (i != 0) 
	    nbargs++;

	commandId = lookupName(command, commandNames);
	/* try command shorts if command is not found*/
	if (commandId < 0)
	  commandId = lookupName(command, shortCommandNames);

	/*
	 * start interpreting the command
	 */
	switch(commandId + DEBUG_HELP_CMD){
	  /* --- Help related commands --- */
	case DEBUG_HELP_CMD:
	  helpTop(arg);	  
	  break;


	  /* --- Running related commands --- */
	case DEBUG_BYE_CMD:
	case DEBUG_EXIT_CMD:
	case DEBUG_QUIT_CMD:
	  /* allow the stylesheet to exit */
	  xslDebugStatus = DEBUG_QUIT;
	  exitShell++;
	  break;

	case DEBUG_STEP_CMD:
	  xslDebugStatus = DEBUG_STEP;
	  exitShell++;
	  break;

	case DEBUG_STEPUP_CMD:
	  {
	    char *noOfFrames = arg;
	    // skip until next space character
	    while (*noOfFrames && (*noOfFrames != ' ')){
	      noOfFrames++;
	    }
	    xslDbgShellFrameBreak(noOfFrames, 1);
	    exitShell++;
	  }
	  break;


	case DEBUG_STEPDOWN_CMD:
	  {
	    char *noOfFrames = arg;
	    // skip until next space character
	    while (*noOfFrames && (*noOfFrames != ' ')){
	      noOfFrames++;
	    }
	    xslDbgShellFrameBreak(noOfFrames, 0);
	    exitShell++;
	  }
	  break;
	/* continue to next break point */
	case DEBUG_CONT_CMD:
	  xslDebugStatus = DEBUG_CONT;
	  exitShell++;
	  break;

	/* restart */
	case DEBUG_RUN_CMD:
	  xslDebugStatus = DEBUG_RUN_RESTART;
	  exitShell++;
	  break;


	  /* --- Template related commands --- */
	case DEBUG_TEMPLATES_CMD :
	  {
	    int allFiles = 1, verbose = 1;
	    if(strlen(arg)){ 
	      if (!strcmp(arg, "this")){
		allFiles = 0;
	      }else{
		fprintf(stderr,"Unknown option %s \n", arg);
	      }
	    }
	      
	  /* be verbose when printing template names */
	  /* if args is not empty then print names this stylesheet */
	  xslDbgPrintTemplateNames(styleCtxt, ctxt, arg, verbose, allFiles );
	  break;
	  }
	  

	case DEBUG_WHERE_CMD:
	  /* Print the current working directory as well */
	  xslDbgPrintCallStack();
	  if (!xmlShellPwd(ctxt, dir, ctxt->node, NULL))
	    fprintf(stderr, "%s", dir);
	  if (ctxt->node && ctxt->node && ctxt->node->doc && ctxt->node->doc->URL)
	    if (xslActiveBreakPoint() > 0){
	      fprintf(stderr, " in file %s : line %ld \n", ctxt->node->doc->URL, (long)ctxt->node->content);
	    }else{
	      fprintf(stderr, " at file %s : line %ld \n", ctxt->node->doc->URL, (long)ctxt->node->content);
	    }
	  else
	    fprintf(stderr, "\n");
	  break;


	  /* --- Break point related commands --- */
	case DEBUG_BREAK_CMD:
	  if (strlen(arg))
	    xslDbgShellBreak(arg, styleCtxt->style);
	  else{
	    /* select current node to break at */
	    char buff[100];
	    if (ctxt->node->doc && ctxt->node->doc->URL)
	      	    snprintf(buff, 99, "-l %s %ld",ctxt->node->doc->URL,
			     xmlGetLineNo(ctxt->node));
	    xslDbgShellBreak(buff, styleCtxt->style);
	  }
	  break;

	case DEBUG_SHOWBREAK_CMD:
	  fprintf(stderr, "\n");
	  if (xslBreakPointCount()){
	    for (i = 1; i <= xslBreakPointCount(); i++){
	      fprintf(stderr, " "); // flag xemacs debug mode we're part of a break point list
	      xslPrintBreakPoint(stderr, i);
	      fprintf(stderr, "\n");
	    }
	  }else{
	    fprintf(stderr, "\nNo file break points set:\n");	      
	  }
	  break;

	case DEBUG_DELETE_CMD:
	  if (strlen(arg))
	    xslDbgShellDelete(arg);
	  else{
	    int breakPointNumber = 0;
	    if (ctxt->node->doc && ctxt->node->doc->URL)
	      breakPointNumber = xslFindBreakPointByLineNo(ctxt->node->doc->URL, 
						   xmlGetLineNo(ctxt->node));
	    if (!breakPointNumber || !xslDeleteBreakPoint(breakPointNumber))
	      fprintf(stderr, "Unable to add delete point");
	  }	  
	break;

	case DEBUG_ENABLE_CMD:
	  if (strlen(arg))	  
	    xslDbgShellEnable(arg, XSL_TOGGLE_BREAKPOINT);
	  else{
	    int breakPointNumber = 0;
	    if (ctxt->node->doc && ctxt->node->doc->URL)
	      breakPointNumber = xslFindBreakPointByLineNo(ctxt->node->doc->URL, 
						   xmlGetLineNo(ctxt->node));
	    if (!breakPointNumber || 
		(!xslEnableBreakPoint(breakPointNumber, 
				     !xslIsBreakPointEnabled(breakPointNumber)) ))
	      fprintf(stderr, "Unable to add enable/disable point\n");
	  }
	break;

	case DEBUG_DISABLE_CMD:
	  if (strlen(arg))	  
	    xslDbgShellEnable(arg, 0);
	  else{
	    int breakPointNumber = 0;
	    if (ctxt->node->doc && ctxt->node->doc->URL)
	      breakPointNumber = xslFindBreakPointByLineNo(ctxt->node->doc->URL, 
						   xmlGetLineNo(ctxt->node));
	    if (!breakPointNumber || 
		(!xslEnableBreakPoint(breakPointNumber, 
				     !xslIsBreakPointEnabled(breakPointNumber)) ))
	      fprintf(stderr, "Unable to add enable/disable point\n");
	  }
	break;



	  /* --- Node view related commands --- */
	case DEBUG_LS_CMD:
	  xslDbgPrintList(ctxt, arg, 0);	  
	  break;

	case DEBUG_DIR_CMD:
	  xslDbgPrintList(ctxt, arg, 1);	  
	  break;

	case DEBUG_DU_CMD:
	  xmlShellDu(ctxt, NULL, ctxt->node, NULL);	  
	  break;

	case DEBUG_CAT_CMD:
	case DEBUG_PRINT_CMD:
	  xslDbgCat(styleCtxt, ctxt, arg);	  
	  break;

	case DEBUG_PWD_CMD:
	  if (!xmlShellPwd(ctxt, dir, ctxt->node, NULL)){
	    fprintf(stderr, "\n%s", dir);	  
	    if (ctxt->node && ctxt->node->doc && ctxt->node->doc->URL)
	      fprintf(stderr, " in file %s : line %ld", ctxt->node->doc->URL, xmlGetLineNo(ctxt->node));
	  }
	  fprintf(stderr, "\n");
	  
	  break;

	case DEBUG_DUMP_CMD:
	  xmlDebugDumpDocument(stdout, ctxt->doc);
	  break;

	case DEBUG_BASE_CMD:
	  xmlShellBase(ctxt, NULL, ctxt->node, NULL);
	  break;


	  /* ---  Variable related commands --- */
	case DEBUG_GLOBALS_CMD:
	  xslDbgPrintVariable(styleCtxt, arg, DEBUG_PRINT_GLOBAL_VAR);	  
	  break;

	case DEBUG_LOCALS_CMD:
	  xslDbgPrintVariable(styleCtxt, arg, DEBUG_PRINT_LOCAL_VAR);
	  break;


	  /* --- Node selection related commands --- */
	case DEBUG_SOURCE_CMD:
	  if (strlen(arg) == 0){
	  if (ctxt->doc == doc->doc)
	    lastDocNode = ctxt->node;
	  ctxt->doc = source->doc;
	  ctxt->node = lastSourceNode;
	  ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
	  showSource = 1;
	  if (ctxt->pctxt == NULL) {
	    xmlFree(ctxt);
	    xslDebugStatus = DEBUG_QUIT;
	    return;
	  }else	    	    
	    break;
	  }else{
	    /* load new stylesheet file*/
            char * response = ctxt->input("Load stylesheet file (yes/no)");
	    if (!strcmp(response, "yes")){
	      loadXmlFile(arg, FILES_SOURCEFILE_TYPE);
	      source = (xmlNodePtr)getStylesheet()->doc;
	      if (source){
		if (ctxt->doc == source->doc)
		  lastDocNode = ctxt->node;	  
		ctxt->doc = doc->doc;
		ctxt->node = doc;
		ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
		showSource = 1;
		if (ctxt->pctxt == NULL) {
		  xmlFree(ctxt);
		  xslDebugStatus = DEBUG_QUIT;
		  return;	    
		}
		printf("Loaded %s ok\n", arg);
	      }
	    }
	  }
	  break;

	case DEBUG_DATA_CMD:
	  if (strlen(arg) == 0){
	    if (ctxt->doc == source->doc)
	      lastSourceNode = ctxt->node;	  
	    ctxt->doc = doc->doc;
	    ctxt->node = lastDocNode;
	    ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
	    showSource = 0;
	    if (ctxt->pctxt == NULL) {
	      xmlFree(ctxt);
	      xslDebugStatus = DEBUG_QUIT;
	      return;	    
	    }
	    else	  
	      break;
	  }else{
	    /* load new xml file*/
            char * response = ctxt->input("Load xml DATA file (yes/no)");
	    if (!strcmp(response, "yes")){
	      loadXmlFile(arg, FILES_XMLFILE_TYPE);
	      doc = (xmlNodePtr)getMainDoc();
	      if (doc){
		if (ctxt->doc == source->doc)
		  lastSourceNode = ctxt->node;	  
		ctxt->doc = doc->doc;
		ctxt->node = doc;
		ctxt->pctxt = xmlXPathNewContext(ctxt->doc);
		showSource = 0;
		if (ctxt->pctxt == NULL) {
		  xmlFree(ctxt);
		  xslDebugStatus = DEBUG_QUIT;
		  return;	    
		}
		printf("Loaded %s ok\n", arg);
	      }
	    }
	  }
	  break;

	case DEBUG_CD_CMD:
	  /* use dir as a working buffer */
	  strncpy(dir, arg, 2);			
	  dir[2] = '\0';
	  shortCutId = lookupName(dir,cdShortCuts);
	  if (shortCutId >= 0){
	    if (strlen(arg) == 2){
	      xslDbgCd(styleCtxt, ctxt,
		       cdAlternative[shortCutId] ,
		       source);	
	    }else{				

	      strcpy(dir, cdAlternative[shortCutId]);
	      strcat(dir, &arg[2]);
	      xslDbgCd(styleCtxt, ctxt, dir, source);					
	    }
				      	
	  }else
	    xslDbgCd(styleCtxt, ctxt, arg, source);	    
	  break;


	  /* --- File related commands --- */
	case DEBUG_VALIDATE_CMD:
	  fprintf(stderr, "validate disabled\n");
	  /*
	  xmlShellValidate(ctxt, arg, NULL, NULL);
	  */
	  break;
	  
	case DEBUG_LOAD_CMD:
	  fprintf(stderr, "loading disabled\n");
	  /*
	    xmlShellLoad(ctxt, arg, NULL, NULL);
	  */
	  break;

	case DEBUG_SAVE_CMD:
	  fprintf(stderr, "saving disabled\n");
	  /*
	    xmlShellSave(ctxt, arg, NULL, NULL);
	  */
	  break;

	case DEBUG_WRITE_CMD:
	  fprintf(stderr, "write disabled\n");
	  /*
	    xmlShellWrite(ctxt, arg, NULL, NULL);
	  */
	  break;

	case DEBUG_FREE_CMD:
	  fprintf(stderr, "free disabled\n");
	  /*
	    if (arg[0] == 0) {
		xmlMemShow(stdout, 0);
	    } else {
	        int len = 0;
		sscanf(arg, "%d", &len);
		xmlMemShow(stdout, len);
	    }
	  */
	  break;

	case   DEBUG_CHDIR_CMD:
	  if (strlen(arg))
	    changeDir(arg);
	  else
	    printf("Missing path name after chdir command\n");

	  break;

	default:
	  xmlGenericError(xmlGenericErrorContext,
			  "Unknown command %s, try help\n", command);
	}


	free(cmdline); /* not xmlFree here ! */
	cmdline = NULL;
    }
    
    xmlXPathFreeContext(ctxt->pctxt);
    /* we don't load the file so it's not needed*/
    /*   if (ctxt->loaded) { 
        xmlFreeDoc(ctxt->doc);
	} */
    if (ctxt->filename != NULL)
	  xmlFree(ctxt->filename);   
    xmlFree(ctxt);
    if (cmdline != NULL)
        free(cmdline); /* not xmlFree here ! */
    xslSetActiveBreakPoint(0);
}


