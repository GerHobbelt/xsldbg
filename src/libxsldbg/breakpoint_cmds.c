
/***************************************************************************
                          breakpoint_cmds.c  - breakpoint commands for xsldbg
                             -------------------
    begin                : Wed Nov 21 2001
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

#ifdef VERSION
#undef VERSION
#endif

#include "xsldbg.h"
#include "debugXSL.h"
#include "files.h"
#include "utils.h"
#include <libxml/valid.h>       /* needed for xmlSplitQName2 */
#include <libxml/xpathInternals.h> /* needed for xmlNSLookup */
#include "xsldbgthread.h"       /* for getThreadStatus() */
#include "xsldbgmsg.h"
#include "options.h"

/* temp buffer needed occationaly */
static xmlChar buff[DEBUG_BUFFER_SIZE];

/* ---------------------------------------------------
   Private function declarations for breakpoint_cmds.c
 ----------------------------------------------------*/

/**
 * validateSource:
 * @url : is valid name of a xsl source file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int validateSource(xmlChar ** url, long *lineNo);

/**
 * validateData:
 * @url : is valid name of a xml data file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int validateData(xmlChar ** url, long *lineNo);


/* ------------------------------------- 
    End private functions
---------------------------------------*/



/* -----------------------------------------

   BreakPoint related commands

  ------------------------------------------- */


/**
 * xslDbgShellFrameBreak:
 * @arg: Is valid number of frames to change location by
 * @stepup: If != 1 then we step up, otherwise step down
 *
 * Set a "frame" break point either up or down from here
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellFrameBreak(xmlChar * arg, int stepup)
{
    int result = 0;

    /* how many frames to go up/down */
    int noOfFrames;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to add break point\n";

    if (!filesGetStylesheet() || !filesGetMainDoc()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Debugger has no files loaded, try reloading files\n%s",
                         errorPrompt);
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (xmlStrLen(arg) > 0) {
        if (!sscanf((char *) arg, "%d", &noOfFrames)) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to read number of frames\n");
            return result;
        }
    } else {
        noOfFrames = 0;
    }

    if (stepup) {
        result = callStackStepup(callStackGetDepth() - noOfFrames);
    } else {
        result = callStackStepdown(callStackGetDepth() + noOfFrames);
    }

    if (!result)
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Failed to set frame break point\n");
    return result;
}


/**
 * validateSource:
 * @url : is valid name of a xsl source file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int
validateSource(xmlChar ** url, long *lineNo)
{

    int result = 0, type;
    searchInfoPtr searchInf;
    nodeSearchDataPtr searchData = NULL;

    if (!filesGetStylesheet()) {
       if (!optionsGetIntOption(OPTIONS_GDB)){
           xsltGenericError(xsltGenericErrorContext,
                   "Error: Stylesheet not valid files not loaded yet?\n");
       }else {
           xsltGenericError(xsltGenericErrorContext,
                   "Debug: Stylesheet not valid files not loaded yet?\n");
	}
        return result;
    }

    if (!url) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    searchInf = searchNewInfo(SEARCH_NODE);

    if (searchInf && searchInf->data) {
        type = DEBUG_BREAK_SOURCE;
        searchData = (nodeSearchDataPtr) searchInf->data;
        if (lineNo != NULL)
            searchData->lineNo = *lineNo;
        searchData->nameInput = (xmlChar *) xmlMemStrdup((char *) *url);
        guessStylesheetName(searchInf);
        /* try to verify that the line number is valid */
        if (searchInf->found) {
            /* ok it looks like we've got a valid url */
            /* searchData->url will be freed by searchFreeInfo */
            if (searchData->absoluteNameMatch)
                searchData->url = (xmlChar *)
                    xmlMemStrdup((char *) searchData->absoluteNameMatch);
            else
                searchData->url = (xmlChar *)
                    xmlMemStrdup((char *) searchData->guessedNameMatch);

            if (lineNo != NULL) {
                /* now to check the line number */
                if (searchData->node) {
                    searchInf->found = 0;
                    /* searchData->node is set to the topmost node in stylesheet */
                    walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                                   searchData->node);
                    if (!searchInf->found) {
                        xsltGenericError(xsltGenericErrorContext,
                                         "Warning: Breakpoint at file %s : line %ld doesn't "
                                         "seem to be valid.\n",
                                         *url, *lineNo);
                    }

                    *lineNo = searchData->lineNo;
                    xmlFree(*url);
                    *url = xmlStrdup(searchData->url);
                    result = 1;
                }

            } else {
                /* we've been asked just to check the file name */
                if (*url)
                    xmlFree(*url);
		if (searchData->absoluteNameMatch)
		    *url = (xmlChar *)
			xmlMemStrdup((char *) searchData->absoluteNameMatch);
		else
		    *url = (xmlChar *)
			xmlMemStrdup((char *) searchData->guessedNameMatch);
                result = 1;
            }
        } else{
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to find a stylesheet file whose name contains %s\n",
                             *url);
	    if (lineNo){
	      xsltGenericError(xsltGenericErrorContext,
			     "Warning: Breakpoint at file %s : line %ld doesn't "
			     "seem to be valid.\n",
			     *url, *lineNo);
	    }
	}
    }

    if (searchInf)
        searchFreeInfo(searchInf);
    else
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create searchInfo out of memory?\n");

    return result;
}




/**
 * validateData:
 * @url : is valid name of a xml data file
 * @lineNo : lineNo >= 0
 *
 * Returns 1 if a breakpoint could be set at specified file url and line number
 *         0 otherwise
 */
int
validateData(xmlChar ** url, long *lineNo)
{
    int result = 0;
    searchInfoPtr searchInf;
    nodeSearchDataPtr searchData = NULL;
    char *lastSlash;

    if (!filesGetMainDoc()) {
       if (!optionsGetIntOption(OPTIONS_GDB)){
           xsltGenericError(xsltGenericErrorContext,
                         "Error: Document not valid files not loaded yet?\n");
       }
        return result;
    }

    if (!url) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    searchInf = searchNewInfo(SEARCH_NODE);
    if (searchInf && searchInf->data && filesGetMainDoc()) {
        /* Try to verify that the line number is valid. 
	   First try an absolute name match */
        searchData = (nodeSearchDataPtr) searchInf->data;
        if (lineNo != NULL)
            searchData->lineNo = *lineNo;
        else
            searchData->lineNo = -1;
        searchData->url = (xmlChar *) xmlMemStrdup((char *) *url);
        walkChildNodes((xmlHashScanner) scanForNode, searchInf,
                       (xmlNodePtr) filesGetMainDoc());

        /* Next try to guess file name by adding the prefix of main document 
	   if no luck so far */
        if (!searchInf->found) {
	  /* Find the last separator of the documents URL */
	  lastSlash = xmlStrrChr(filesGetMainDoc()->URL, URISEPARATORCHAR);
	  if (!lastSlash)
	    lastSlash = xmlStrrChr(filesGetMainDoc()->URL, PATHCHAR);
	  
	  if (lastSlash) {
	    lastSlash++;
	    xmlStrnCpy(buff, filesGetMainDoc()->URL,
		       lastSlash - (char *) filesGetMainDoc()->URL);
	    buff[lastSlash - (char *) filesGetMainDoc()->URL] = '\0';
	    xmlStrCat(buff, *url);
	  } else
	    xmlStrCpy(buff, "");
	  if (xmlStrLen(buff) > 0) {
	    if (searchData->url)
	      xmlFree(searchData->url);
	    searchData->url = (xmlChar *) xmlMemStrdup((char *) buff);
	    walkChildNodes((xmlHashScanner) scanForNode, searchInf,
			   (xmlNodePtr) filesGetMainDoc());
	  }
        }

        if (!searchInf->found) {
	  if (lineNo){
             xsltGenericError(xsltGenericErrorContext,
                             "Warning: Breakpoint at file %s : line %ld doesn't "
                             "seem to be valid.\n", *url, *lineNo);
	  } else{
             xsltGenericError(xsltGenericErrorContext,
                             "Error: Unable to find a data file whose name contains %s\n",
                             *url);	       
	  }
            result = 1;
        } else {
            if (*url)
                xmlFree(*url);
            *url = xmlStrdup(searchData->url);
            result = 1;
        }
    }

    if (searchInf)
        searchFreeInfo(searchInf);
    else
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Unable to create searchInfo out of memory?\n");

    return result;
}


/**
 * xslDbgShellBreak:
 * @arg: Is valid and in UTF-8
 * @style: Is valid
 * @ctxt: Is valid
 * 
 * Add break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellBreak(xmlChar * arg, xsltStylesheetPtr style,
                 xsltTransformContextPtr ctxt)
{
    int result = 0;
    long lineNo = -1;
    xmlChar *url = NULL;
    int orphanedBreakPoint = 0;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to add break point\n";

    if (style == NULL) {
        style = filesGetStylesheet();
    }
    if (!style || !filesGetMainDoc()) {
       if (!optionsGetIntOption(OPTIONS_GDB)){
	    xsltGenericError(xsltGenericErrorContext,
                         "Error: Debugger has no files loaded, try reloading files\n%s",
                         errorPrompt);
	    return result;
	}else{
	    orphanedBreakPoint = 1;
	}
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2];

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if ((xmlStrlen(opts[1]) == 0) || 
		       !sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to read line number %s\n",
                                     errorPrompt);
                    return result;
                } else {
                    /* try to guess whether we are looking for source or data 
                     * break point
                     */
                    trimString(opts[0]);
                    url = xmlStrdup(opts[0]);
                    if (url) {
			if (!orphanedBreakPoint){
			    if (filesIsSourceFile(url)) {
				if (validateSource(&url, &lineNo))
				    result =
					breakPointAdd(url, lineNo, NULL, NULL,
						DEBUG_BREAK_SOURCE);
			    } else {
				if (validateData(&url, &lineNo))
				    result =
					breakPointAdd(url, lineNo, NULL, NULL,
						DEBUG_BREAK_DATA);
			    }
			}else{
			    breakPointPtr brk;
			    if (filesIsSourceFile(url)) {
				result =
				    breakPointAdd(url, lineNo, NULL, NULL,
					    DEBUG_BREAK_SOURCE);
			    }else{
				result =
				    breakPointAdd(url, lineNo, NULL, NULL,
					    DEBUG_BREAK_DATA);
			    }
			    brk = breakPointGet(url, lineNo);
			    if (brk){
				brk->flags |= BREAKPOINT_ORPHANED;
			    }else{
				xsltGenericError(xsltGenericErrorContext,
					"Error: Unable to find added breakpoint");
			    }
			}
		    }
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: break command arguments not in format \"-l <FILE_NAME> <LINE_NUMBER>\"\n");
        }
    } else {
        /* add breakpoint at specified template names */
        xmlChar *opts[2];
	xmlChar *qName[3];
        xmlChar *name = NULL, *nameURI = NULL, *mode = NULL, *modeURI = NULL;
	xmlChar *templateName = NULL, *modeName = NULL;
	xmlChar *tempUrl = NULL; /* we must use a non-const xmlChar *
				   and we are not making a copy
				   of orginal value so this must not be 
				   freed */ 
        xmlChar *defaultUrl = (xmlChar *) "<n/a>";
        int newBreakPoints = 0;
	int allTemplates = 0;
	int ignoreTemplateNames = 0;
	int argCount;
	int found;	
        xsltTemplatePtr templ;
	if (orphanedBreakPoint){
	    xsltGenericError(xsltGenericErrorContext,
		    "Error: Can't add orphaned breakpoint to a template \n");
	    return 0;
	}


	argCount = splitString(arg, 2, opts);
	if ((argCount == 2) && (xmlStrLen(opts[1]) == 0))
	    argCount = 1;

	switch (argCount){
	case 0:
	  allTemplates = 1;
	  break;
	  
	case 1:
	  if (xmlStrEqual(opts[0], (xmlChar*)"*")){
	    allTemplates = 1;	    
	  }else{

	    if (xmlStrEqual(opts[0], (xmlChar*)"\\*")){
	      opts[0][0] = '*';
	      opts[0][1] = '\0';
	    }

	    name = xmlSplitQName2(opts[0], &nameURI);
	    if (name == NULL){
	      name = xmlStrdup(opts[0]);
	    }else{
	      if (nameURI){
		/* get the real URI for this namespace */
		const xmlChar *temp = xmlXPathNsLookup(ctxt->xpathCtxt, nameURI);
		if (temp)
		  xmlFree(nameURI);
		nameURI = xmlStrdup(temp);
	      }
	      
	    }
	  }
	break;

	case 2:
	   if (xmlStrLen(opts[0]) == 0){
	     /* we don't care about the template name ie we are trying to match
	        templates with a given mode */
	     ignoreTemplateNames = 1;
	    }else{
	      name = xmlSplitQName2(opts[0], &nameURI);
	      if (name == NULL)
                name = xmlStrdup(opts[0]);
	      if (nameURI){
		/* get the real URI for this namespace */
		const xmlChar *temp = xmlXPathNsLookup(ctxt->xpathCtxt, 
						       nameURI);
		if (temp)
		  xmlFree(nameURI);
		nameURI = xmlStrdup(temp);
	    }
	    }
            mode = xmlSplitQName2(opts[1], &modeURI);
            if (mode == NULL)
                mode = xmlStrdup(opts[1]);
	    if (modeURI){
	      /* get the real URI for this namespace */
	      const xmlChar *temp = xmlXPathNsLookup(ctxt->xpathCtxt, modeURI);
	      if (temp)
		xmlFree(modeURI);
	      modeURI = xmlStrdup(temp);
	    }
	  break;

	default:	  
	  xsltGenericError(xsltGenericErrorContext,
                                 "Error: break command arguments not in format \"<TEMPLATE_NAME> <MODE_NAME>. Where <MODE_NAME> is optional \n");
	  return 0;
	}

        while (style) {
            templ = style->templates;
            while (templ) {
	        found = 0;
                if (templ->elem && templ->elem->doc
                    && templ->elem->doc->URL) {
                    tempUrl = (xmlChar *) templ->elem->doc->URL;
                } else {
                    tempUrl = defaultUrl;
                }

		if (templ->match)
		    templateName = xmlStrdup(templ->match);
		else
	            templateName = fullQName(templ->nameURI, templ->name);
		    
		if (allTemplates)
		  found = 1;
		else {
		  if (ignoreTemplateNames){
		      if (!mode || (xmlStrEqual(templ->mode, mode) && 
				    (!modeURI || xmlStrEqual(templ->modeURI, 
							     modeURI))))
			found = 1;
		  } else if (templ->match){
		    if ((xmlStrEqual(templ->match, name) &&
			 (!modeURI || xmlStrEqual(templ->modeURI, 
						  modeURI))))
			found = 1;		     
		  }else{
			if(xmlStrEqual(templ->name, name) && 
			   (!nameURI || xmlStrEqual(templ->nameURI, nameURI))) 
			  found = 1;
		  }
	        }
                if (found) {
		    if (templ->mode)
		       modeName = 
			 fullQName(templ->modeURI, templ->mode);
		    if (!breakPointAdd(tempUrl, 
					     xmlGetLineNo(templ->elem),
                                       templateName, modeName, 
					     DEBUG_BREAK_SOURCE)){
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Can't add breakPoint to file %s : line %d\n",
                                         tempUrl, xmlGetLineNo(templ->elem));
                        xsltGenericError(xsltGenericErrorContext,
                                         "Error: Breakpoint to template :\"%s\" in file %s :"
                                         " line %d exists \n", templateName,
                                         templ->elem->doc->URL,
                                         xmlGetLineNo(templ->elem));
                    } else {
                        newBreakPoints++;
		    }
		}
		if (templateName){
		  xmlFree(templateName);
		  templateName = NULL;
		}
		if (modeName){
		  xmlFree(modeName);
		  modeName = NULL;
		}
                templ = templ->next;
            }
            if (style->next)
                style = style->next;
            else
                style = style->imports;
        }

        if (newBreakPoints == 0) {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: No templates found or unable to add any breakPoints\n ");
            url = NULL;         /* flag that we've printed partial error message about the problem url */
        } else {
            result = 1;
            xsltGenericError(xsltGenericErrorContext,
                             "Information: Added %d new breakPoints\n",
                             newBreakPoints);
        }

        if (name)
	  xmlFree(name);
	if (nameURI)
	  xmlFree(nameURI);
	if (mode)
	  xmlFree(mode);
	if (modeURI)
	  xmlFree(modeURI);
        if (defaultUrl)
            xmlFree(defaultUrl);
	if (tempUrl)
	  url = xmlStrdup(tempUrl);
    }  /* end add template breakpoints */

    if (!result) {
        if (url)
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Failed to add breakpoint "
                             "at file %s: line %ld\n", url, lineNo);
        else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Failed to add breakpoint(s)\n");
    }

    if (url)
        xmlFree(url);
    return result;
}


/**
 * xslDbgShellDelete:
 * @arg: Is valid and in UTF-8
 * 
 * Delete break point specified by arg
 *
 * Returns 1 on success,
 *         0 otherwise
 */
int
xslDbgShellDelete(xmlChar * arg)
{
    int result = 0, breakPointId;
    long lineNo;
    breakPointPtr breakPtr = NULL;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to delete break point\n";

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2], *url = NULL;

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if ((xmlStrlen(opts[1]) == 0) ||
		    !sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to read line number. %s\n",
                                     errorPrompt);
                } else {
                    url = xmlStrdup(opts[0]);
                    if (url) {
                        if (filesIsSourceFile(url)) {
                            if (validateSource(&url, &lineNo))
                                breakPtr = breakPointGet(url, lineNo);
                        } else if (validateData(&url, &lineNo))
                            breakPtr = breakPointGet(url, lineNo);
                        if (!breakPtr || !breakPointDelete(breakPtr))
                            xsltGenericError(xsltGenericErrorContext,
                                             "Error: Breakpoint doesn't exist at file %s : line %ld. %s\n",
                                             url, lineNo, errorPrompt);
                        else
                            result = 1;
                        xmlFree(url);
                    }
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: delete command arguments not in format \"-l <FILE_NAME> <LINE_NUMBER>\" %s\n",
                                 errorPrompt);
        }
    } else if (xmlStrEqual((xmlChar*)"*", arg)) {
        result = 1;
        /*remove all from breakpoints */
        breakPointEmpty();

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPtr = findBreakPointById(breakPointId);
        if (breakPtr) {
            result = breakPointDelete(breakPtr);
            if (!result) {

                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to delete breakpoint %d . %s\n",
                                 breakPointId, errorPrompt);
            }
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint %d doesn't exist. %s\n",
                             breakPointId, errorPrompt);
        }
    } else {
        breakPtr = findBreakPointByName(arg);
        if (breakPtr) {
            result = breakPointDelete(breakPtr);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Delete breakpoint to template %s failed. %s\n",
                                 arg, errorPrompt);
            }
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint at template '%s' doesn't exist. %s\n",
                             arg, errorPrompt);
    }
    return result;
}


/**
 * xslDbgShellEnableBreakPoint:
 * @payload: A valid breakPointPtr
 * @data: Enable type, a pointer to an integer 
 *         for a value of 
 *                 1 enable break point
 *                 0 disable break point
 *                 -1 toggle enabling of break point 
 * @name: Not used
 *
 * Enable/disable break points via use of scan of break points
*/
void
xslDbgShellEnableBreakPoint(void *payload, void *data,
                            xmlChar * name ATTRIBUTE_UNUSED)
{
    if (payload && data) {
        breakPointEnable((breakPointPtr) payload, *(int *) data);
    }
}


/**
 * xslDbgShellEnable:
 * @arg : is valid and in UTF-8
 * @enableType : enable break point if 1, disable if 0, toggle if -1
 *
 * Enable/disable break point specified by arg using enable 
 *      type of @enableType
 * Returns 1 if successful,
 *         0 otherwise
 */
int
xslDbgShellEnable(xmlChar * arg, int enableType)
{
    int result = 0, breakPointId;
    long lineNo;
    breakPointPtr breakPtr = NULL;
    static const xmlChar *errorPrompt =
        (xmlChar *) "Failed to enable/disable break point\n";

    if (!filesGetStylesheet() || !filesGetMainDoc()) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: Debugger has no files loaded, try reloading files\n%s",
                         errorPrompt);
        return result;
    }

    if (!arg) {
        xsltGenericError(xsltGenericErrorContext,
                         "Error: NULL argument provided\n");
        return result;
    }

    if (arg[0] == '-') {
        xmlChar *opts[2], *url = NULL;

        if ((xmlStrLen(arg) > 1) && (arg[1] == 'l')) {
            if (splitString(&arg[2], 2, opts) == 2) {
                if ((xmlStrlen(opts[1]) == 0) ||
		    !sscanf((char *) opts[1], "%ld", &lineNo)) {
                    xsltGenericError(xsltGenericErrorContext,
                                     "Error: Unable to read line number. %s\n",
                                     errorPrompt);
                } else {
                    url = xmlStrdup(opts[0]);
                    if (url) {
                        if (strstr((char *) url, ".xsl")) {
                            if (validateSource(&url, NULL))
                                breakPtr = breakPointGet(url, lineNo);
                        } else if (validateData(&url, NULL))
                            breakPtr = breakPointGet(url, lineNo);
                        if (breakPtr)
                            result =
                                breakPointEnable(breakPtr, enableType);
                        else
                            xsltGenericError(xsltGenericErrorContext,
                                             "Error: Breakpoint does not exist at %s:%ld . %s", 
					     url, lineNo, errorPrompt);
                        xmlFree(url);
                    }
                }
            } else
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: enable/disable command arguments not in format \"-l <FILE_NAME> <LINE_NUMBER>\" %s\n",
                                 errorPrompt);
        }
    } else if (xmlStrEqual((xmlChar*)"*", arg)) {
        result = 1;
        /*enable/disable all from breakpoints */
        walkBreakPoints((xmlHashScanner) xslDbgShellEnableBreakPoint,
                        &enableType);

    } else if (sscanf((char *) arg, "%d", &breakPointId)) {
        breakPtr = findBreakPointById(breakPointId);
        if (breakPtr) {
            result = breakPointEnable(breakPtr, enableType);
            if (!result) {
                xsltGenericError(xsltGenericErrorContext,
                                 "Error: Unable to enable/disable breakpoint %d\n",
                                 breakPointId);
            }
        } else {
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint %d doesn't exist\n",
                             breakPointId);
        }
    } else {
        breakPtr = findBreakPointByName(arg);
        if (breakPtr) {
            result = breakPointEnable(breakPtr, enableType);
        } else
            xsltGenericError(xsltGenericErrorContext,
                             "Error: Breakpoint at template '%s' does not exist. %s\n",
                             arg, errorPrompt);
    }
    return result;
}


/**
 * xslDbgShellPrintBreakPoint:
 * @payload: A valid breakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print data given by scan of break points 
*/
void
xslDbgShellPrintBreakPoint(void *payload, void *data ATTRIBUTE_UNUSED,
                           xmlChar * name ATTRIBUTE_UNUSED)
{

    if (payload) {
        if (getThreadStatus() == XSLDBG_MSG_THREAD_RUN) {
            notifyListQueue(payload);
        } else {
            printCount++;
            xsltGenericError(xsltGenericErrorContext, " ");
            breakPointPrint(NULL, (breakPointPtr) payload);
            xsltGenericError(xsltGenericErrorContext, "\n");
        }
    }
}

/**
 * xslDbgShellValidateBreakPoint:
 * @payload: A valid breakPointPtr
 * @data: Not used
 * @name: Not used
 *
 * Print an warning if a breakpoint is invalid
*/
void xslDbgShellValidateBreakPoint(void *payload, void *data ATTRIBUTE_UNUSED,
                           xmlChar * name ATTRIBUTE_UNUSED)
{
  int result = 0, flags; 
  if (payload){
    breakPointPtr breakPtr =  (breakPointPtr) payload;
    if (filesIsSourceFile(breakPtr->url)) {
      result = validateSource(&breakPtr->url, &breakPtr->lineNo);
    } else {
      result = validateData(&breakPtr->url, &breakPtr->lineNo);
    }    
    flags = breakPtr->flags;
    if (result)
       breakPtr->flags &= BREAKPOINT_ALLFLAGS ^ BREAKPOINT_ORPHANED;
    else 
       breakPtr->flags |= BREAKPOINT_ORPHANED;

   if ( breakPtr->flags & BREAKPOINT_ORPHANED){
	xsltGenericError(xsltGenericErrorContext,"Warning: Breakpoint #%d is orphaned result %d, old flags %d new flags %d\n", breakPtr->id, result, flags, breakPtr->flags);
    }
  }
}
