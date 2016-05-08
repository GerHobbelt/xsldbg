
#ifndef ARRAYLIST_H
#define ARRAYLIST_H


typedef void (*freeItemFunc) (void *item);

/* A dynamic structure that behave like a list */
typedef struct _arrayList arrayList;
typedef arrayList *arrayListPtr;

struct _arrayList {
    int size, count;
    void **data;
    freeItemFunc deleteFunction;
};

/* what size of the list do we stop automatic doubling of capacity
   if array list size growth is needed */
#define DOUBLE_SIZE_MAX_ITEM 10



/**
 * Create a new list with a size of @p initialSize
 *
 * @returns Non-null on success,
 *          NULL otherwise
 *
 * @param initial The initial size of list
 * @param deleteFunction the Function to call to free items in the list
 */
arrayListPtr arrayListNew(int initialSize, freeItemFunc deleteFunction);


/**
 * Free memory assocated with array list, if the array list 
 *   has a valid deleteFunction then content with be freed with 
 *    using that deleteFunction
 *
 * @param list A valid list
 */
void arrayListFree(arrayListPtr list);


/**
 * Empties the list of its content
 *
 * @returns 1 on success,
 *          0 otherwise
 *
 * @param list A valid list
 */
int arrayListEmpty(arrayListPtr list);


/**
 * Return the maximum number elements this list can contain
 *
 * @returns The maximum number elements this list can contain
 *
 * @param list A valid list
 */
int arrayListSize(arrayListPtr list);


/**
 * Return the count of number items in list
 *
 * @returns The count of number items in list
 *
 * @param list A valid list
 */
extern int arrayListCount(arrayListPtr list);


/**
 * Add @p item to @p list
 *
 * @returns 1 if able to add @p item to end of @p list,
 *          0 otherwise
 *
 * @param list A valid list
 * @param item A valid item
 */
int arrayListAdd(arrayListPtr list, void *item);


/**
 * @returns 1 if able to delete element in @p list at position @p position,
 *          0 otherwise 
 *
 * @param list A valid list
 * @param position  0 =< @p position < arrayListCount(@p list)
 */
int arrayListDelete(arrayListPtr list, int position);


/**
 * @returns Non-null if able to retrieve element in @p list at position
 *          @p position,
 *         NULL otherwise
 *
 * @param list A valid list
 * @param position  0 =< @p position < arrayListCount(@p list)
 */
void *arrayListGet(arrayListPtr list, int position);


#endif
