/***************************************************************************
                          testList.c - a test stub for the xslArrayList 
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

#include "../src/arraylist.h"
#include "../src/options.h"

ArrayListPtr createTestList();
ArrayListPtr createParamList();

int main(){
  int index, result = 0;
  ArrayListPtr list = createTestList();
  if (list){
  printf("\nTest free all items. List size now %d\n", xslArrayListCount(list));    
    for (index = 0; index < xslArrayListCount(list); index++)
      printf("%s\n", (char*)xslArrayListGet(list, index));
    printf("Freeing list now\n");
    xslArrayListFree(list);
    printf("Done!\n");
  }else 
    result = 1;

  list = createTestList();
  if (list){
    printf("\nTest removing item2\n");
    xslArrayListDelete(list, 1);
    for (index = 0; index < xslArrayListCount(list); index++)
      printf("%s\n", (char*)xslArrayListGet(list, index));
  
    printf("Size now %d\nDone!\n", xslArrayListCount(list));

    printf("\nTest removing item1\n");
    xslArrayListDelete(list, 0);
    for (index = 0; index < xslArrayListCount(list); index++)
      printf("%s\n", (char*)xslArrayListGet(list, index));
  
    printf("Size now %d\nDone!\n", xslArrayListCount(list));

    printf("\nTest removing item4\n");
    xslArrayListDelete(list, xslArrayListCount(list) -1 );
    for (index = 0; index < xslArrayListCount(list); index++)
      printf("%s\n", (char*)xslArrayListGet(list, index));
  
    printf("Size now %d\nDone!\n", xslArrayListCount(list));
    xslArrayListFree(list);
  }else
    result = 1;

  list = createParamList();
  if (list){
    ParameterItemPtr item;
    printf("Test of parameter item list\n");
    for (index = 0; index < xslArrayListCount(list); index++){
      item = xslArrayListGet(list, index);
      if (item){
	printf("Parameter name :%s  value %s\n", item->name, item->value);
      }
    }    
    xslArrayListFree(list);
    printf("Done\n");
  }

   printf("Finished array list tests\n");
  /* A non zero result is an error*/
  return result;
}

ArrayListPtr createTestList(){
  xmlChar *item1 = xmlMemStrdup("item1");
  xmlChar *item2 = xmlMemStrdup("item2");
  xmlChar *item3 = xmlMemStrdup("item3");
  xmlChar *item4 = xmlMemStrdup("item4");
  ArrayListPtr list = xslArrayListNew(3, xmlFree);
  printf("Creating test list\n");
  if (list){
    xslArrayListAdd(list, item1);
    xslArrayListAdd(list, item2);
    xslArrayListAdd(list, item3);
    xslArrayListAdd(list, item4);
  }
  printf("Done!\n");
  return list;
}

ArrayListPtr createParamList(){
  ArrayListPtr list = xslArrayListNew(3, (freeItemFunc)paramItemFree);
  if (list){
    xslArrayListAdd(list, paramItemNew("param1", "'value1'"));
    xslArrayListAdd(list, paramItemNew("param2", "'value2'"));
    xslArrayListAdd(list, paramItemNew("param3", "'value3'"));
  }
  return list;
}
