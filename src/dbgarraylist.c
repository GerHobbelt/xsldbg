
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
#include <libxslt/xsltutils.h>

#ifndef NULL
#define NULL 0
#endif

/**
 * arrayListNew:
 * @intialSize: initial size of list
 * @deleteFunction: function to call to free items in the list
 *
 * Create a new list with a size of @initialSize
 * Returns non-null on success,
 *         NULL otherwise
 */
ArrayListPtr
arrayListNew(int initialSize, freeItemFunc deleteFunction)
{
    ArrayListPtr list = NULL;

    if (initialSize <= 0) {
        xsltGenericError(xsltGenericErrorContext,
                         "arrayListNew invalid initialSize %d\n",
                         initialSize);
    } else
        list = (ArrayListPtr) xmlMalloc(sizeof(ArrayList));

    if (list) {
        list->data = (void *) xmlMalloc(sizeof(void *) * initialSize);
        list->deleteFunction = deleteFunction;
        list->count = 0;
        list->size = initialSize;
    }

    return list;
}


/**
 * arrayListFree:
 * @list: a valid list
 *
 * Free memory assocated with array list, if the array list 
 *   has a valid deleteFunction then content with be freed with 
 *    useing that deleteFunction
 */
void
arrayListFree(ArrayListPtr list)
{
    if (!list)
        return;

    arrayListEmpty(list);
    xmlFree(list->data);
    xmlFree(list);
}


/**
 * arrayListEmpty:
 * @list: a valid list
 *
 * Empties the list of its content
 * Returns 1 on success,
 *         0 otherwise
 */
int
arrayListEmpty(ArrayListPtr list)
{
    int index, result = 0;

    if (list) {
        if (list->deleteFunction) {
            for (index = 0; index < list->count; index++) {
                if (list->data[index])
                    (*list->deleteFunction) (list->data[index]);
            }
            result++;
            list->count = 0;
        }
    }
    return result;
}


/**
 * arrayListSize:
 * @list: a valid list
 *
 * Returns the maximum number elements this list can contain
 */
int
arrayListSize(ArrayListPtr list)
{
    int result = 0;

    if (list)
        result = list->size;

    return result;
}


/**
 * arrayListCount:
 * @list: a valid list
 *
 * Returns the count of number items in list
 */
int
arrayListCount(ArrayListPtr list)
{
    int result = 0;

    if (list)
        result = list->count;

    return result;
}


/**
 * arrayListAdd:
 * @list: valid list
 * @item: valid item
 *
 * Returns 1 if able to add @item to end of @list
 *         0 otherwise
 */
int
arrayListAdd(ArrayListPtr list, void *item)
{
    int result = 0;

    if (list && item) {
        if (list->count + 1 > list->size) {
            /* grow the size of data */
            void **temp;
            int newSize, index;

            if (list->size < DOUBLE_SIZE_MAX_ITEM)
                newSize = list->size * 2;
            else
                newSize = (int) (list->size * 1.5);
            temp = (void *) xmlMalloc(sizeof(void *) * newSize);
            for (index = 0; index < list->count; index++) {
                temp[index] = list->data[index];
            }
            xmlFree(list->data);
            list->data = temp;
            list->size = newSize;
        }
        list->data[list->count++] = item;
        result++;
    }
    return result;
}


/**
 * arrayListDelete:
 * @list: valid list
 * @position: 0 =< position < arrayListCount(list)
 *
 * Returns 1 if able to delete element in @list at position @position
 *         0 otherwise 
 */
int
arrayListDelete(ArrayListPtr list, int position)
{
    int result = 0, index;

    if (list && (list->count > 0) && (position >= 0) &&
        (position < list->count) && list->data[position]) {
        if (list->deleteFunction)
            (*list->deleteFunction) (list->data[position]);

        /* shuffle all elements upwards */
        for (index = position; index < (list->count - 1); index++) {
            list->data[index] = list->data[index + 1];
        }
        list->count--;
        result++;
    }
    return result;
}


/**
 * arrayListGet:
 * @list: valid list
 * @position: 0 =< position < arrayListCount(list)
 *
 * Returns non-null if able to retrieve element in @list at position
 *          @position
 *         NULL otherwise
 */
void *
arrayListGet(ArrayListPtr list, int position)
{
    void *result = NULL;

    if (list && (position >= 0) && (position < list->count)) {
        result = list->data[position];
    }
    return result;
}
