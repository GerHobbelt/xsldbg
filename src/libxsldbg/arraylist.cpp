/***************************************************************************
                          arrayList.c  - create and work with arrays
                             -------------------
    begin                : Mon May 2 2016
    copyright            : (C) 2016 by Keith Isdale
    email                : keithisdale@gmail.com
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

arrayListPtr arrayListNew(int initialSize, freeItemFunc deleteFunction)
{
    arrayListPtr list = NULL;

    if (initialSize <= 0) {
#ifdef WITH_XSLDBG_DEBUG_PROCESS
        xsltGenericError(xsltGenericErrorContext,
                         "arrayListNew invalid initialSize %d\n",
                         initialSize);
#endif
    } else
        list = (arrayListPtr) xmlMalloc(sizeof(arrayList));

    if (list) {
        list->data = (void **) xmlMalloc(sizeof(void *) * initialSize);
        list->deleteFunction = deleteFunction;
        list->count = 0;
        list->size = initialSize;
    }

    return list;
}


void arrayListFree(arrayListPtr list)
{
    if (!list)
        return;

    arrayListEmpty(list);
    xmlFree(list->data);
    xmlFree(list);
}


int arrayListEmpty(arrayListPtr list)
{
    int index, result = 0;

    if (list) {
        if (list->deleteFunction) {
            for (index = 0; index < list->count; index++) {
                if (list->data[index])
                    (*list->deleteFunction) (list->data[index]);
            }
            result = 1;
            list->count = 0;
        }
    }
    return result;
}


int arrayListSize(arrayListPtr list)
{
    int result = 0;

    if (list)
        result = list->size;

    return result;
}


int arrayListCount(arrayListPtr list)
{
    int result = 0;

    if (list)
        result = list->count;

    return result;
}


int arrayListAdd(arrayListPtr list, void *item)
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
            temp = (void **) xmlMalloc(sizeof(void *) * newSize);
            for (index = 0; index < list->count; index++) {
                temp[index] = list->data[index];
            }
            xmlFree(list->data);
            list->data = temp;
            list->size = newSize;
        }
        list->data[list->count++] = item;
        result = 1;
    }
    return result;
}


int arrayListDelete(arrayListPtr list, int position)
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
        result = 1;
    }
    return result;
}



void * arrayListGet(arrayListPtr list, int position)
{
    void *result = NULL;

    if (list && (position >= 0) && (position < list->count)) {
        result = list->data[position];
    }
    return result;
}
