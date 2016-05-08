
#ifndef XSL_DEBUG_H
#define XSL_DEBUG_H


/**
 * Initialize debugger allocating any memory needed by debugger
 *
 * @returns 1 on success,
 *          0 otherwise
 */
int debugInit(void);


/**
 * Free up any memory taken by debugger
 */
void debugFree(void);


/**
 * Set flag that debuger has received control to value of @p reached
 *
 * @returns 1 if any breakpoint was reached previously,
 *          0 otherwise
 *
 * @param reached 1 if debugger has received control, -1 to read its value 
 *              or 0 to clear the flag
 */
int debugGotControl(int reached);

#endif
