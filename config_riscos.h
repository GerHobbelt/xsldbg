/* config_riscos.h.  config.h file needed by risc os
                          Made by Justin Fletcher */

 /* Define if you need the GNU extensions to compile */
 #undef _GNU_SOURCE
 #define WITH_DEBUGGER
 #undef HAVE_ISINF
 #undef HAVE_ISNAN
 #define HAVE_POW
 #define HAVE_FLOOR
 #define HAVE_FABS
 #undef HAVE_READLINE
 #undef HAVE_HISTORY
 #define TIMESTAMP __DATE__
 #define WITH_DEBUG_HELP

 /* Define if you have the _stat function.  */
 /* #define HAVE__STAT */

 /* Define if you have the asctime function.  */
 #define HAVE_ASCTIME

 /* Define if you have the gettimeofday function.  */
 #undef HAVE_GETTIMEOFDAY

 /* Define if you have the localtime function.  */
 #define HAVE_LOCALTIME

 /* Define if you have the mktime function.  */
 #define HAVE_MKTIME

 /* Define if you have the stat function.  */
 /* #define HAVE_STAT */

 /* Define if you have the usleep function.  */
 /* #define HAVE_USLEEP */

 /* Define if you have the <ansidecl.h> header file.  */
 /* #define HAVE_ANSIDECL_H */

 /* Define if you have the <float.h> header file.  */
 /* #define HAVE_FLOAT_H */

 /* Define if you have the <fp_class.h> header file.  */
 /* #define HAVE_FP_CLASS_H */

 /* Define if you have the <ieeefp.h> header file.  */
 /* #define HAVE_IEEEFP_H */

 /* Define if you have the <math.h> header file.  */
 #define HAVE_MATH_H

 /* Define if you have the <nan.h> header file.  */
 /* #define HAVE_NAN_H */

 /* Define if you have the <stdarg.h> header file.  */
 #define HAVE_STDARG_H

 /* Define if you have the <stdlib.h> header file.  */
 #define HAVE_STDLIB_H

 /* We have string.h */
 #define HAVE_STRING_H

 /* Define if you have the <sys/stat.h> header file.  */
 /* #define HAVE_SYS_STAT_H */

 /* Define if you have the <sys/time.h> header file.  */
 /* #define HAVE_SYS_TIME_H */

 /* Define if you have the <sys/types.h> header file.  */
 /* #define HAVE_SYS_TYPES_H */

 /* Define if you have the <time.h> header file.  */
 #define HAVE_TIME_H

 /* Define if you have the <unistd.h> header file.  */
 /* #define HAVE_UNISTD_H */

 /* Name of package */
 #define PACKAGE "xsldbg"

 /* what is the name of the binary (we use a variable that we set in main)
 */
  #define XSLDBG_BIN (xsldbgCommand==NULL ? \
                          "Error No XSLDBG present|M||" : \
                          xsldbgCommand)
  extern char *xsldbgCommand; /* Our invocation command */

 /* Version number of package */
 #define VERSION "3/0/4"



