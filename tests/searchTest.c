#include "../src/breakpointInternals.h"
#include <libxml/debugXML.h>

int main(void){
  int result = 0;
  xslBreakPointPtr breakPoint = breakPointItemNew();
  xmlNodePtr node = NULL;
  xmlDocPtr doc = NULL;
  if (!searchInit())
      return 1;

  if (breakPoint){
    breakPoint->url = xmlStrdup("index.xsl");
    breakPoint->lineNo = 10;
    breakPoint->templateName = xmlStrdup("/");
    breakPoint->id = 1;
    node = searchBreakPointNode(breakPoint);
    if(node){
      xmlShellPrintNode(node);
    }else{
       xsltGenericError(xsltGenericErrorContext,
			"Create search breakpoint node failed\n");
    }
    doc = searchDoc();
    if (doc){
      xslSearchAdd(node);
      breakPoint->lineNo = 12;
      node = searchBreakPointNode(breakPoint);      
      xslSearchAdd(node);
      breakPoint->lineNo = 13;
      node = searchBreakPointNode(breakPoint);      
      xslSearchAdd(node);
      xmlShellPrintNode(node);
      xslSearchSave("search.data");
      /* no need to free doc as it will be done by searchFree function*/
    }else{
      if (node)
	xmlFreeNode(node);
    }
      
    breakPointItemFree(breakPoint, NULL);    
    result++;
  }

  
  if (result)
     xsltGenericError(xsltGenericErrorContext, "Success!\n");

  searchFree();
  return !result;
}

/**
 * changeDir:
 * @path : path to adopt as new working directory
 *
 * Change working directory to path 
 */
int
changeDir(const xmlChar * path)
{
  return 0;
}
