
/***************************************************************************
                          options.h  -  declare the functions for 
                                        implementation of the array list
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

#ifndef ARRAYLIST_H
#define ARRAYLIST_H

typedef void (*freeItemFunc) (void *item);

typedef struct _ArrayList ArrayList;
typedef ArrayList *ArrayListPtr;
struct _ArrayList {
    int size, count;
    void **data;
    freeItemFunc deleteFunction;
};

#define DOUBLE_SIZE_MAX_ITEM 10


/**
 * xslArrayListNew:
 * @intialSize : initial size of list
 * @autoDelete : free the memory of content upon deletion
 *
 * Create a new list with a size of @initialSize
 * Returns non-null on success,
 *         0 otherwise
 */
ArrayListPtr xslArrayListNew(int initialSize, freeItemFunc deleteFunction);


/**
 * arrListFree:
 * @list : a valid list
 *
 * Free memory assocated with array list, if the array list 
 *   has is autoDelete enabled then content with be freed with 
 *   xmlFree
 */
void xslArrayListFree(ArrayListPtr list);


/**
 * xslArrayListEmpty:
 * @list : a valid list
 *
 * Empties the list of its content
 * Returns 1 on success,
 *         0 otherwise
 */
int xslArrayListEmpty(ArrayListPtr list);


/**
 * xslArrayListSize:
 * @list : a valid list
 *
 * Returns the maximum number elements this list can contain
 */
int xslArrayListSize(ArrayListPtr list);


/**
 * xslArrayListCount:
 * @list : a valid list
 *
 * Returns the count of number items in list
 */
int xslArrayListCount(ArrayListPtr list);


/**
 * xslArrayListAdd:
 * @list : valid list
 * @item : valid item
 *
 * Add item to end of list
 */
int xslArrayListAdd(ArrayListPtr list, void *item);


/**
 * xslArrayListDelete:
 * @list : valid list
 * @position : 0 =< position < xslArrayListCount(list)
 *
 * Add item to end of list
 */
int xslArrayListDelete(ArrayListPtr list, int position);


/**
 * xslArrayListGet:
 * @list : valid list
 * @position : 0 =< position < xslArrayListCount(list)
 *
 * Add non-null if item is valid
 *     NULL otherwise
 */
void *xslArrayListGet(ArrayListPtr list, int position);

#endif
