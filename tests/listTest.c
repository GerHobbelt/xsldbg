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
#include "../src/libxsldbg/arraylist.h"
#include "../src/libxsldbg/breakpoint.h"
#include "../src/libxsldbg/search.h"
#include "../src/libxsldbg/options.h"

arrayListPtr createTestList(void);
arrayListPtr createParamList(void);
int hashTest(void);

arrayListPtr createBreakPointList(void);
arrayListPtr xslBreakPointLineList(void);

/*
 * Print data given by scan of breakPoints 
*/
void myWalker(void *payload, void *data, xmlChar *name);



int main(void){
  int itemIndex, result = 0;
  arrayListPtr list = createTestList();
  xmlInitMemory();  
  if (list){
  fprintf(stdout, "\nTest free all items. List size now %d\n", 
	  arrayListCount(list));    
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

  optionsInit();
  fprintf(stdout, "\n\nTest of parameter item list\n");
  list = createParamList();
  optionsPrintParamList();
  fprintf(stdout, "Freeing memory used by options\n");
  optionsFree();
  fprintf(stdout, "Done\n");
  

  printf("\n\nTest of breakPoint list\n");
  breakPointInit();
  list = createBreakPointList();
  if (list){
    breakPointPtr breakPtr = breakPointGet((xmlChar*)"index.html", 1);
    if (breakPtr){
      fprintf(stdout, "Added breakPoint :");
      breakPointPrint(stdout, breakPtr);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #1 not found!\n");
      result = 1;
    }

    breakPtr = breakPointGet((xmlChar*)"index2.html", 2);
    if (breakPtr){
      fprintf(stdout, "Added breakPoint :");
      breakPointPrint(stdout, breakPtr);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #2 not found!\n");
      result = 1;
    }

    breakPtr = breakPointGet((xmlChar*)"index3.html", 3);
    if (breakPtr){
      fprintf(stdout, "Added breakPoint :");
      breakPointPrint(stdout, breakPtr);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #3 not found!\n");
      result = 1;
    }

    breakPtr = breakPointGet((xmlChar*)"index2.html", 1);
    if (breakPtr){
      fprintf(stdout, "Added breakPoint :");
      breakPointPrint(stdout, breakPtr);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #4 not found!\n");
      result = 1;
    }

    breakPtr = breakPointGet((xmlChar*)"index2.html", 3);
    if (breakPtr){
      fprintf(stdout, "Added breakPoint :");
      breakPointPrint(stdout, breakPtr);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #5 not found!\n");
      result = 1;
    }

    breakPtr = breakPointGet((xmlChar*)"index2.html", 4);
    if (breakPtr){
      fprintf(stdout, "Added breakPoint :");
      breakPointPrint(stdout, breakPtr);
      fprintf(stdout, "\n");
    }else{
      fprintf(stdout,"\nBreakpoint #6 not found!\n");
      result = 1;
    }

    fprintf(stdout, "\nTrying a breakPoint scanner to print breakPoints\n" \
	    "BreakPoints will be ordered by line number\n");
    walkBreakPoints((xmlHashScanner)myWalker, NULL);

    fprintf(stdout,"\nLooking for breakpoint with id of 2\n");
    breakPtr = findBreakPointById(2);
    if (breakPtr){
      fprintf(stdout,"Found it! Here it is!\n");
      breakPointPrint(stdout, breakPtr);   
      fprintf(stdout,"\n");   
    }else{
      fprintf(stdout,"Not found!\n");
      result = 1;
    }     

    fprintf(stdout,"\nLooking for breakpoint with template name of 'three'\n");
    breakPtr = findBreakPointByName((xmlChar*)"three");
    if (breakPtr){
      fprintf(stdout,"Found it! Here it is!\n");
      breakPointPrint(stdout, breakPtr);   
      fprintf(stdout,"\n");   
    }else{
      fprintf(stdout,"Not found!\n");
      result = 1;
    }  
    fprintf(stdout,"Done!\n");

    fprintf(stdout, "\nTest for adding two duplicate breakpoints these should fail\n");
    breakPointAdd((xmlChar*)"index.html", 1, (xmlChar*)"one", NULL,
		  DEBUG_BREAK_SOURCE);
    breakPointAdd((xmlChar*)"index2.html", 4, NULL, NULL, DEBUG_BREAK_SOURCE);
    fprintf(stdout,"Done!\n");  
  }

  fprintf(stdout, "\nFreeing memory used by breakpoints\n");    
  breakPointFree();

  fprintf(stdout,"Done\n");

  xmlMemoryDump();
  fprintf(stdout, "\nFinished array list tests\n");
  /* A non zero result is an error*/
  return result;

}

arrayListPtr createTestList(void){
  arrayListPtr list = arrayListNew(3, xmlFree);
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


arrayListPtr createParamList(void){
  arrayListPtr list =   optionsGetParamItemList();
  fprintf(stdout, "Creating parameters list\n");
  if (list){
    arrayListAdd(list, optionsParamItemNew((xmlChar*)"param1", (xmlChar*)"'value1'"));
    arrayListAdd(list, optionsParamItemNew((xmlChar*)"param2", (xmlChar*)"'value2'"));
    arrayListAdd(list, optionsParamItemNew((xmlChar*)"param3", (xmlChar*)"'value3'"));
  }
  return list;
}

  arrayListPtr createBreakPointList(void){
  arrayListPtr list = breakPointLineList();
  fprintf(stdout, "Creating breakPoint list\n");
  if (list){
    /*    breakPointAdd(const xmlChar * url, long lineNumber,
	  const xmlChar * templateName, int type) */
    breakPointAdd((xmlChar*)"index.html", 1, (xmlChar*)"one", NULL, DEBUG_BREAK_SOURCE);
    breakPointAdd((xmlChar*)"index2.html", 2, (xmlChar*)"two", NULL, DEBUG_BREAK_SOURCE);
    breakPointAdd((xmlChar*)"index3.html", 3, (xmlChar*)"three", NULL, DEBUG_BREAK_SOURCE); 
    breakPointAdd((xmlChar*)"index2.html", 1, (xmlChar*)"four", NULL, DEBUG_BREAK_SOURCE); 
    breakPointAdd((xmlChar*)"index2.html", 3, NULL, NULL, DEBUG_BREAK_SOURCE); 
    breakPointAdd((xmlChar*)"index2.html", 4, NULL, NULL, DEBUG_BREAK_SOURCE); 
  }
  fprintf(stdout, "Done!\n");
  return list;
}


/*
 * Print data given by scan of breakPoints 
*/
void myWalker(void *payload, void *data ATTRIBUTE_UNUSED, xmlChar *name ATTRIBUTE_UNUSED){
  if (payload){
     breakPointPrint(stdout, (breakPointPtr)payload);
    fprintf(stdout, "\n");   
  }
}



