#include <stdio.h>
#include <libxsldbg/arraylist.h>
#include <libxsldbg/breakpoint.h>
#include <libxsldbg/search.h>
#include <libxsldbg/options.h>


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
  breakPointPtr breakItem = breakPointItemNew();
  xmlNodePtr node = NULL;
  xmlDocPtr doc = NULL;

   printf( "Search for breakpoints in search dataBase\n");
  if (!breakItem){
    printf( "Create of breakPoint item failed\n");
    return result;
  }

  breakItem->url = xmlStrdup((xmlChar*)"index.xsl");
  breakItem->lineNo = 10;
  breakItem->templateName = xmlStrdup((xmlChar*)"/");
  breakItem->id = 1;
  node = searchBreakPointNode(breakItem);
  if(node){
    xmlShellPrintNode(node);
    result++;
  }else{
    printf("Create search breakpoint node failed\n");
  }  

  doc = searchDoc();
  if ((result == 1) && doc){
    searchAdd(node);
    breakItem->lineNo = 12;
    node = searchBreakPointNode(breakItem);      
    searchAdd(node);
    breakItem->lineNo = 13;
    node = searchBreakPointNode(breakItem);      
    searchAdd(node);
    xmlShellPrintNode(node);
    searchSave((xmlChar*)"search.data");
    /* no need to free doc as it will be done by searchFree function*/
    result++;
  }else{
    if (node)
      xmlFreeNode(node); 
  }

  breakPointItemFree(breakItem, NULL); 
  return result;
}


/* lets try walking/finding xml nodes of test1.xsl */
int nodeSearchTest(void)
{
  int result = 0;
  xmlDocPtr searchDoc = NULL;
  searchInfoPtr info = searchNewInfo(SEARCH_NODE);
  nodeSearchDataPtr searchData = NULL;

  printf( "\n\nSearching for a xml node at file test1.xsl: line 11\n");
  xmlLineNumbersDefault(1);
  searchDoc = xmlParseFile("test1.xsl");

  if (searchDoc && info && info->data){
    searchData = (nodeSearchDataPtr)info->data;
    searchData->lineNo = 11;
    searchData->url = (xmlChar*)xmlMemStrdup("test1.xsl");
    walkChildNodes((xmlHashScanner) testNodeScan, info, (xmlNodePtr)searchDoc);
    if (info->found){
      /* success !*/
      printf("Success found node at file %s: line %d\n",
		       searchData->node->doc->URL, 
		       xmlGetLineNo(searchData->node));
      /* now try finding a line/url that doesn't exist */
      searchData->lineNo = 10;
      info->found = 0;
      printf("\nSearching for node at non-existant file test1.xsl: line 10\n");	    
      walkChildNodes((xmlHashScanner) testNodeScan, info, (xmlNodePtr)searchDoc);
      if (!info->found){
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

  if (info)
    searchFreeInfo(info);

  if (searchDoc)
    xmlFreeDoc(searchDoc);

  return result;
}

void testNodeScan(void *payload, void *data,
                       xmlChar * name ATTRIBUTE_UNUSED){
  searchInfoPtr info = (searchInfoPtr)data;
  nodeSearchDataPtr searchData = NULL;
  xmlNodePtr node = (xmlNodePtr)payload;

  if (!node || !node->doc || !node->doc->URL || 
      !info || (info->type != SEARCH_NODE))
    return;

  searchData = (nodeSearchDataPtr)info->data;

  if (((searchData->lineNo >= 0) && (searchData->lineNo == xmlGetLineNo(node))) ||
      (searchData->url && strcmp((char*)searchData->url, (char*)node->doc->URL))){
    searchData->node = node;
    info->found = 1;
  }
}
