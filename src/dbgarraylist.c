/***************************************************************************
                          options.c  -  define array implementation of a list
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

#include "arraylist.h"

/**
 * xslArrayListNew:
 * @intialSize : intial size of list
 * @autoDelete : free the memory of content upon deletion
 *
 * Create a new list with a size of @intialSize
 * Returns non-null on sucess,
 *         0 otherwise
 */
ArrayListPtr xslArrayListNew(int initialSize,   freeItemFunc deleteFunction){
  ArrayListPtr list = NULL;
  if (initialSize <= 0){
    xsltGenericError(xsltGenericErrorContext,
		     "xslArrayListNew invalid initialSize %d\n" , initialSize);
  }else 
    list =   (ArrayList*)xmlMalloc(sizeof(ArrayList));

  if (list){
    list->data = (void *)xmlMalloc(sizeof(void) * initialSize);
    list->deleteFunction  =  deleteFunction;
    list->count = 0;
    list->size = initialSize;    
  }

  return list;
}


/**
 * arrListFree:
 * @list : a valid list
 *
 * Free memory assocated with array list, if the array list 
 *   has is autoDelete enabled then content with be freed with 
 *   xmlFree
 */
void xslArrayListFree(ArrayListPtr list){
  if (!list)
    return;
  
  while(xslArrayListCount(list))
    xslArrayListDelete(list, xslArrayListCount(list) - 1);
  xmlFree(list);
}

/**
 * xslArrayListEmpty:
 * @list : a valid list
 *
 * Empties the list of its content
 * Returns 1 on success,
 *         0 otherwise
 */
int xslArrayListEmpty(ArrayListPtr list){
  int result = 0;
    if (list){
      while(xslArrayListCount(list))
	xslArrayListDelete(list, xslArrayListCount(list) - 1);
      result++;
    }
  return result;
}

/**
 * xslArrayListSize:
 * @list : a valid list
 *
 * Returns the maximum number elements this list can contain
 */
int xslArrayListSize(ArrayListPtr list){
  int result = 0;
  if (list)
    result = list->size;

  return result;
}


/**
 * xslArrayListCount:
 * @list : a valid list
 *
 * Returns the count of number items in list
 */
int xslArrayListCount(ArrayListPtr list){
  int result = 0;
  if (list)
    result = list->count;

  return result;
}


/**
 * xslArrayListAdd:
 * @list : valid list
 * @item : valid item
 *
 * Add item to end of list
 */
int xslArrayListAdd(ArrayListPtr list, void *item){
  int result = 0;
  if (list){
    if (list->count + 1 > list->size){
      /* grow the size of data */
      void **temp;
      int newSize, index;
      if (list->size < DOUBLE_SIZE_MAX_ITEM)
	newSize = list->size * 2;
      else
	newSize = (int)(list->size * 1.5);
      temp =  (void *)xmlMalloc(sizeof(void) * newSize);
      for (index = 0; index < list->count; index++){
	temp[index] = list->data[index];
      }
      xmlFree(list->data);
      list->data = temp;
      list->size = newSize;
    }
    list->data[list->count++]  = item;    
    result++;
  }
  return result;
}


/**
 * xslArrayListDelete:
 * @list : valid list
 * @position : 0 =< position < xslArrayListCount(list)
 *
 * Add item to end of list
 */
int xslArrayListDelete(ArrayListPtr list, int position){
  int result = 0, index;
  if (list && (position >=0 ) && 
      (position < list->count) && list->data[position]){
    if (list->deleteFunction)
      (*list->deleteFunction)(list->data[position]);

    /* shuffle all elements upwards*/
    for (index = position; index < (list->count - 1) ; index++){
      list->data[index] = list->data[index + 1];      
    }
    list->count--;
    result++;
  }
  return result;
}

/**
 * xslArrayListGet:
 * @list : valid list
 * @position : 0 =< position < xslArrayListCount(list)
 *
 * Add non-null if item is valid
 *     NULL otherwise
 */
void * xslArrayListGet(ArrayListPtr list, int position){
  int index;
  void *result = NULL;
  if (list && (position >= 0 ) && (position < list->count)){
    result = list->data[position];
  }
  return result;
}
