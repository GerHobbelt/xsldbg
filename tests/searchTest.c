#include <stdio.h>
#include "../src/arraylist.h"
#include "../src/xslbreakpoint.h"
#include "../src/xslsearch.h"
#include "../src/options.h"


void testNodeScan(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED);
int nodeSearchTest(void);
int breakPointSearchTest(void);

int main(void){
  int result = 1;
  if (!searchInit())
    return result; /* error couldn't setup searching*/

    printf("Starting search tests\n");
  result = result &&  breakPointSearchTest();
  result = result && nodeSearchTest();
  
  if (result)
    printf("\nSuccess all search tests passed!\n");
  
  searchFree();
  if (result >= 1)
    exit(0);
  else
    exit(1);
}

int breakPointSearchTest(void)
{
  int result = 0;
  xslBreakPointPtr breakPoint = breakPointItemNew();
  xmlNodePtr node = NULL;
  xmlDocPtr doc = NULL;

   printf( "Search for breakpoints in search dataBase\n");
  if (!breakPoint){
    printf( "Create of breakPoint item failed\n");
    return result;
  }

  breakPoint->url = xmlStrdup((xmlChar*)"index.xsl");
  breakPoint->lineNo = 10;
  breakPoint->templateName = xmlStrdup((xmlChar*)"/");
  breakPoint->id = 1;
  node = searchBreakPointNode(breakPoint);
  if(node){
    xmlShellPrintNode(node);
    result++;
  }else{
    printf("Create search breakpoint node failed\n");
  }  

  doc = searchDoc();
  if ((result == 1) && doc){
    searchAdd(node);
    breakPoint->lineNo = 12;
    node = searchBreakPointNode(breakPoint);      
    searchAdd(node);
    breakPoint->lineNo = 13;
    node = searchBreakPointNode(breakPoint);      
    searchAdd(node);
    xmlShellPrintNode(node);
    searchSave((xmlChar*)"search.data");
    /* no need to free doc as it will be done by searchFree function*/
    result++;
  }else{
    if (node)
      xmlFreeNode(node); 
  }

  breakPointItemFree(breakPoint, NULL); 
  return result;
}


/* lets try walking/finding xml nodes of test1.xsl */
int nodeSearchTest(void)
{
  int result = 0;
  xmlDocPtr searchDoc = NULL;
  searchInfoPtr searchInf = searchNewInfo(SEARCH_NODE);
  nodeSearchDataPtr searchData = NULL;

  printf( "\n\nSearching for a xml node at file test1.xsl: line 11\n");
  xmlLineNumbersDefault(1);
  searchDoc = xmlParseFile("test1.xsl");

  if (searchDoc && searchInf && searchInf->data){
    searchData = (nodeSearchDataPtr)searchInf->data;
    searchData->lineNo = 11;
    searchData->url = (xmlChar*)xmlMemStrdup("test1.xsl");
    walkChildNodes((xmlHashScanner) testNodeScan, searchInf, (xmlNodePtr)searchDoc);
    if (searchInf->found){
      /* success !*/
      printf("Success found node at file %s: line %d\n",
		       searchData->node->doc->URL, 
		       xmlGetLineNo(searchData->node));
      /* now try finding a line/url that doesn't exist */
      searchData->lineNo = 10;
      searchInf->found = 0;
      printf("\nSearching for node at non-existant file test1.xsl: line 10\n");	    
      walkChildNodes((xmlHashScanner) testNodeScan, searchInf, (xmlNodePtr)searchDoc);
      if (!searchInf->found){
	/* success*/
	printf("Success search for node has passed\n");
	result++;
      }
    }else{
      printf("Failed node not found\n");
    }
  }else{
    printf("Unable to create searchInfo or load test1.xsl for node searching\n");
  }

  if (searchInf)
    searchFreeInfo(searchInf);

  if (searchDoc)
    xmlFreeDoc(searchDoc);

  return result;
}


int changeDir(const xmlChar * path);

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


/*
 * A break point has been found so pass control to user
 * @templ : The source node being executed
 * @node : The data node being processed
 * @root : The template being applied to "node"
 * @ctxt :
 * 
 * Start the xsldbg command prompt
 */
void
debugBreak(xmlNodePtr templ, xmlNodePtr node, xsltTemplatePtr root,
              xsltTransformContextPtr ctxt){
}

void testNodeScan(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED){
  searchInfoPtr searchInf = (searchInfoPtr)data;
  nodeSearchDataPtr searchData = NULL;
  xmlNodePtr node = (xmlNodePtr)payload;

  if (!node || !node->doc || !node->doc->URL || 
      !searchInf || (searchInf->type != SEARCH_NODE))
    return;

  searchData = (nodeSearchDataPtr)searchInf->data;

  if (((searchData->lineNo >= 0) && (searchData->lineNo == xmlGetLineNo(node))) ||
      (searchData->url && strcmp((char*)searchData->url, (char*)node->doc->URL))){
    searchData->node = node;
    searchInf->found = 1;
  }
}
