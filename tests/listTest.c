/***************************************************************************
                          testList.c - a test stub for the arrayList 
                                          implementation
                             -------------------
    begin                : Sat Nov 10 2001
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

#include <stdio.h>
#include <libxsldbg/arraylist.h>
#include <libxsldbg/xslbreakpoint.h>
#include <libxsldbg/xslsearch.h>
#include <libxsldbg/options.h>

ArrayListPtr createTestList(void);
ArrayListPtr createParamList(void);
int hashTest(void);

ArrayListPtr createBreakPointList(void);
ArrayListPtr xslBreakPointLineList(void);
int breakPointInit(void);
void breakPointFree(void);

/*
 * Print data given by scan of breakPoints 
*/
void myWalker(void *payload, void *data, xmlChar *name);



int main(void){
  int itemIndex, result = 0;
  ArrayListPtr list = createTestList();
  xmlInitMemory();
  if (list){
  fprintf(stdout, "\nTest free all items. List size now %d\n", arrayListCount(list));    
    for (itemIndex = 0; itemIndex < arrayListCount(list); itemIndex++)
      printf("%s\n", (char*)arrayListGet(list, itemIndex));
    fprintf(stdout, "Freeing list now\n");
    arrayListFree(list);
    fprintf(stdout, "Done!\n");
  }else 
    result = 1;

  list = createTestList();
  if (list){
    fprintf(stdout, "\nTest removing item2\n");
    arrayListDelete(list, 1);
    for (itemIndex = 0; itemIndex < arrayListCount(list); itemIndex++)
      fprintf(stdout, "%s\n", (char*)arrayListGet(list, itemIndex));
  
    fprintf(stdout, "Size now %d\nDone!\n", arrayListCount(list));

    fprintf(stdout, "\nTest removing item1\n");
    arrayListDelete(list, 0);
    for (itemIndex = 0; itemIndex < arrayListCount(list); itemIndex++)
      fprintf(stdout, "%s\n", (char*)arrayListGet(list, itemIndex));
  
    fprintf(stdout, "Size now %d\nDone!\n", arrayListCount(list));

    fprintf(stdout, "\nTest removing last item in list\n");
    arrayListDelete(list, arrayListCount(list) -1 );
    for (itemIndex = 0; itemIndex < arrayListCount(list); itemIndex++)
      fprintf(stdout, "%s\n", (char*)arrayListGet(list, itemIndex));
  
    fprintf(stdout, "Size now %d\nDone!\n", arrayListCount(list));
    arrayListFree(list);
  }else
    result = 1;

  fprintf(stdout, "\n\nTest of parameter item list\n");
  list = createParamList();
  if (list){
    ParameterItemPtr item;
    for (itemIndex = 0; itemIndex < arrayListCount(list); itemIndex++){
      item = arrayListGet(list, itemIndex);
      if (item){
	fprintf(stdout, "Parameter name :%s  value %s\n", item->name, item->value);
      }
    }    
    arrayListFree(list);
  }
  fprintf(stdout, "Done\n");


  printf("\n\nTest of breakPoint list\n");
  breakPointInit();
  list = createBreakPointList();
  if (list){
    xslBreakPointPtr breakPoint = getBreakPoint((xmlChar*)"index.html", 1);
    if (breakPoint){
      fprintf(stdout, "Added breakPoint :");
      printBreakPoint(stdout, breakPoint);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #1 not found!\n");
      result = 1;
    }

    breakPoint = getBreakPoint((xmlChar*)"index2.html", 2);
    if (breakPoint){
      fprintf(stdout, "Added breakPoint :");
      printBreakPoint(stdout, breakPoint);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #2 not found!\n");
      result = 1;
    }

    breakPoint = getBreakPoint((xmlChar*)"index3.html", 3);
    if (breakPoint){
      fprintf(stdout, "Added breakPoint :");
      printBreakPoint(stdout, breakPoint);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #3 not found!\n");
      result = 1;
    }

    breakPoint = getBreakPoint((xmlChar*)"index2.html", 1);
    if (breakPoint){
      fprintf(stdout, "Added breakPoint :");
      printBreakPoint(stdout, breakPoint);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #4 not found!\n");
      result = 1;
    }

    breakPoint = getBreakPoint((xmlChar*)"index2.html", 3);
    if (breakPoint){
      fprintf(stdout, "Added breakPoint :");
      printBreakPoint(stdout, breakPoint);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #5 not found!\n");
      result = 1;
    }

    breakPoint = getBreakPoint((xmlChar*)"index2.html", 4);
    if (breakPoint){
      fprintf(stdout, "Added breakPoint :");
      printBreakPoint(stdout, breakPoint);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #6 not found!\n");
      result = 1;
    }

    fprintf(stdout, "\nTrying a breakPoint scanner to print breakPoints\n" \
	    "BreakPoints will be ordered by line number\n");
    walkBreakPoints((xmlHashScanner)myWalker, NULL);

    fprintf(stdout,"\nLooking for breakpoint with id of 2\n");
    breakPoint = findBreakPointById(2);
    if (breakPoint){
      fprintf(stdout,"Found it! Here it is!\n");
      printBreakPoint(stdout, breakPoint);   
      fprintf(stdout,"\n");   
    }else{
      fprintf(stdout,"Not found!\n");
      result = 1;
    }     

    fprintf(stdout,"\nLooking for breakpoint with template name of 'three'\n");
    breakPoint = findBreakPointByName((xmlChar*)"three");
    if (breakPoint){
      fprintf(stdout,"Found it! Here it is!\n");
      printBreakPoint(stdout, breakPoint);   
      fprintf(stdout,"\n");   
    }else{
      fprintf(stdout,"Not found!\n");
      result = 1;
    }  
    fprintf(stdout,"Done!\n");

    fprintf(stdout, "\nTest for adding two duplicate breakpoints these should fail\n");
    addBreakPoint((xmlChar*)"index.html", 1, (xmlChar*)"one", DEBUG_BREAK_SOURCE);
    addBreakPoint((xmlChar*)"index2.html", 4, NULL, DEBUG_BREAK_SOURCE);
    fprintf(stdout,"Done!\n");

    
    fprintf(stdout, "\nFreeing memory used by breakpoints\n");    
    emptyBreakPoint();
  }
  fprintf(stdout,"Done\n");

  xmlMemoryDump();
  fprintf(stdout, "\nFinished array list tests\n");
  /* A non zero result is an error*/
  return result;

}

ArrayListPtr createTestList(void){
  ArrayListPtr list = arrayListNew(3, xmlFree);
  fprintf(stdout, "Creating test list\n");
  if (list){
    arrayListAdd(list, xmlMemStrdup("item1"));
    arrayListAdd(list, xmlMemStrdup("item2"));
    arrayListAdd(list, xmlMemStrdup("item3"));
    arrayListAdd(list, xmlMemStrdup("item4"));
    arrayListAdd(list, xmlMemStrdup("item5"));
    arrayListAdd(list, xmlMemStrdup("item6"));
    arrayListAdd(list, xmlMemStrdup("item7"));
    arrayListAdd(list, xmlMemStrdup("item8"));
    arrayListAdd(list, xmlMemStrdup("item9"));
    arrayListAdd(list, xmlMemStrdup("item10"));
    arrayListAdd(list, xmlMemStrdup("item11"));
  }
  fprintf(stdout, "Done!\n");
  return list;
}


ArrayListPtr createParamList(void){
  ArrayListPtr list = arrayListNew(3, (freeItemFunc)paramItemFree);
  if (list){
    arrayListAdd(list, paramItemNew((xmlChar*)"param1", (xmlChar*)"'value1'"));
    arrayListAdd(list, paramItemNew((xmlChar*)"param2", (xmlChar*)"'value2'"));
    arrayListAdd(list, paramItemNew((xmlChar*)"param3", (xmlChar*)"'value3'"));
  }
  return list;
}

ArrayListPtr createBreakPointList(void){
  ArrayListPtr list;
  fprintf(stdout, "Creating breakPoint list\n");
  list = xslBreakPointLineList();
  if (list){
    /*    addBreakPoint(const xmlChar * url, long lineNumber,
	  const xmlChar * templateName, int type) */
    addBreakPoint((xmlChar*)"index.html", 1, (xmlChar*)"one", DEBUG_BREAK_SOURCE);
    addBreakPoint((xmlChar*)"index2.html", 2, (xmlChar*)"two", DEBUG_BREAK_SOURCE);
    addBreakPoint((xmlChar*)"index3.html", 3, (xmlChar*)"three", DEBUG_BREAK_SOURCE); 
    addBreakPoint((xmlChar*)"index2.html", 1, (xmlChar*)"four", DEBUG_BREAK_SOURCE); 
    addBreakPoint((xmlChar*)"index2.html", 3, NULL, DEBUG_BREAK_SOURCE); 
    addBreakPoint((xmlChar*)"index2.html", 4, NULL, DEBUG_BREAK_SOURCE); 
  }
  fprintf(stdout, "Done!\n");
  return list;
}


/*
 * Print data given by scan of breakPoints 
*/
void myWalker(void *payload, void *data ATTRIBUTE_UNUSED, xmlChar *name ATTRIBUTE_UNUSED){
  if (payload){
     printBreakPoint(stdout, (xslBreakPointPtr)payload);
    fprintf(stdout, "\n");   
  }
}



