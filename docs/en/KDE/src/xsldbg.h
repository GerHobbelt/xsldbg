
/***************************************************************************
                          xsldbg.h  - descibe the application level functions
                             -------------------
    begin                : Sun Sep 16 2001
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







/**
 * Provide provide application level services and useful bits and pieces
 *
 * @short application functions and useful bits and pieces
 *
 * @author Keith Isdale <k_isdale@tpg.com.au> 
 */


/* end of WITH_XSL_DEBUG */

/**
 * ATTRIBUTE_UNUSED:
 *
 * This macro is used to flag unused function parameters to GCC
 */







/* We want skip most of these includes when building documentation */



/* Copyright (C) 1991,92,93,95,96,97,98,99, 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

/*
 *	ISO C99 Standard: 7.21 String handling	<string.h>
 */




/* Copyright (C) 1991,92,93,95,96,97,98,99, 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */




/* These are defined by the user (or the compiler)
   to specify the desired environment:

   __STRICT_ANSI__	ISO Standard C.
   _ISOC99_SOURCE	Extensions to ISO C89 from ISO C99.
   _POSIX_SOURCE	IEEE Std 1003.1.
   _POSIX_C_SOURCE	If ==1, like _POSIX_SOURCE; if >=2 add IEEE Std 1003.2;
			if >=199309L, add IEEE Std 1003.1b-1993;
			if >=199506L, add IEEE Std 1003.1c-1995
   _XOPEN_SOURCE	Includes POSIX and XPG things.  Set to 500 if
			Single Unix conformance is wanted, to 600 for the
			upcoming sixth revision.
   _XOPEN_SOURCE_EXTENDED XPG things and X/Open Unix extensions.
   _LARGEFILE_SOURCE	Some more functions for correct standard I/O.
   _LARGEFILE64_SOURCE	Additional functionality from LFS for large files.
   _FILE_OFFSET_BITS=N	Select default filesystem interface.
   _BSD_SOURCE		ISO C, POSIX, and 4.3BSD things.
   _SVID_SOURCE		ISO C, POSIX, and SVID things.
   _GNU_SOURCE		All of the above, plus GNU extensions.
   _REENTRANT		Select additionally reentrant object.
   _THREAD_SAFE		Same as _REENTRANT, often used by other systems.

   The `-ansi' switch to the GNU C compiler defines __STRICT_ANSI__.
   If none of these are defined, the default is to have _SVID_SOURCE,
   _BSD_SOURCE, and _POSIX_SOURCE set to one and _POSIX_C_SOURCE set to
   199506L.  If more than one of these are defined, they accumulate.
   For example __STRICT_ANSI__, _POSIX_SOURCE and _POSIX_C_SOURCE
   together give you ISO C, 1003.1, and 1003.2, but nothing else.

   These are defined by this file and are used by the
   header files to decide what to declare or define:

   __USE_ISOC99		Define ISO C99 things.
   __USE_POSIX		Define IEEE Std 1003.1 things.
   __USE_POSIX2		Define IEEE Std 1003.2 things.
   __USE_POSIX199309	Define IEEE Std 1003.1, and .1b things.
   __USE_POSIX199506	Define IEEE Std 1003.1, .1b, .1c and .1i things.
   __USE_XOPEN		Define XPG things.
   __USE_XOPEN_EXTENDED	Define X/Open Unix things.
   __USE_UNIX98		Define Single Unix V2 things.
   __USE_XOPEN2K        Define XPG6 things.
   __USE_LARGEFILE	Define correct standard I/O things.
   __USE_LARGEFILE64	Define LFS things with separate names.
   __USE_FILE_OFFSET64	Define 64bit interface as default.
   __USE_BSD		Define 4.3BSD things.
   __USE_SVID		Define SVID things.
   __USE_MISC		Define things common to BSD and System V Unix.
   __USE_GNU		Define GNU extensions.
   __USE_REENTRANT	Define reentrant/thread-safe *_r functions.
   __FAVOR_BSD		Favor 4.3BSD things in cases of conflict.

   The macros `__GNU_LIBRARY__', `__GLIBC__', and `__GLIBC_MINOR__' are
   defined by this file unconditionally.  `__GNU_LIBRARY__' is provided
   only for compatibility.  All new code should use the other symbols
   to test for features.

   All macros listed above as possibly being defined by this file are
   explicitly undefined if they are not explicitly defined.
   Feature-test macros that are not defined by the user or compiler
   but are implied by the other feature-test macros defined (or by the
   lack of any definitions) are defined by the file.  */


/* Undefine everything, so we get a clean slate.  */




















/* Suppress kernel-name space pollution unless user expressedly asks
   for it.  */




/* Always use ISO C things.  */



/* If _BSD_SOURCE was defined by the user, favor BSD over POSIX.  */


/* If _GNU_SOURCE was defined by the user, turn on all the other features.  */


/* If nothing (other than _GNU_SOURCE) is defined,
   define _BSD_SOURCE and _SVID_SOURCE.  */





/* This is to enable the ISO C99 extension.  Also recognize the old macro
   which was used prior to the standard acceptance.  This macro will
   eventually go away and the features enabled by default once the ISO C99
   standard is widely adopted.  */


/* If none of the ANSI/POSIX macros are defined, use POSIX.1 and POSIX.2
   (and IEEE Std 1003.1b-1993 unless _XOPEN_SOURCE is defined).  */















































/* We do support the IEC 559 math functionality, real and complex.  */



/* wchar_t uses ISO 10646-1 (2nd ed., published 2000-09-15) / Unicode 3.0.  */


/* This macro indicates that the installed library is the GNU C Library.
   For historic reasons the value now is 6 and this will stay from now
   on.  The use of this variable is deprecated.  Use __GLIBC__ and
   __GLIBC_MINOR__ now (see below) when you want to test for a specific
   GNU C library version and use the values in <gnu/lib-names.h> to get
   the sonames of the shared libraries.  */



/* Major and minor version number of the GNU C library package.  Use
   these macros to test for features in specific releases.  */



/* Convenience macros to test the versions of glibc and gcc.
   Use them like this:
   #if __GNUC_PREREQ (2,8)
   ... code requiring gcc 2.8 or later ...
   #endif
   Note - they won't work for gcc1 or glibc1, since the _MINOR macros
   were not defined then.  */






/* This is here only because every header file already includes this one.  */


/* Copyright (C) 1992,93,94,95,96,97,98,99, 2000 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */




/* We are almost always included from features.h. */


/* The GNU libc does not support any K&R compilers or the traditional mode
   of ISO C compilers anymore.  Check for some of the combinations not
   anymore supported.  */


/* Some user header file might have defined this before.  */





/* GCC can always grok prototypes.  For C++ programs we add throw()
   to help it optimize the function calls.  But this works only with
   gcc 2.8.x and egcs.  */




/* This macro will be used for functions which might take C++ callback
   functions.  */


/* Not GCC.  *//* GCC.  */

/* For these things, GCC behaves the ANSI way normally,
   and the non-ANSI way under -traditional.  */




/* This is not a typedef so `const __ptr_t' does the right thing.  */




/* C++ needs to know that types and declarations are C, not C++.  */






/* Support for bounded pointers.  */

/* nothing */
/* nothing */
/* nothing */



/* __asm__ ("xyz") is used throughout the headers to rename functions
   at the assembly language level.  This is wrapped by the __REDIRECT
   macro, in order to support compilers that can do this some other
   way.  When compilers don't support asm-names at all, we have to do
   preprocessor tricks instead (which don't have exactly the right
   semantics, but it's the best we can do).

   Example:
   int __REDIRECT(setpgrp, (__pid_t pid, __pid_t pgrp), setpgid); */







/*
#elif __SOME_OTHER_COMPILER__

# define __REDIRECT(name, proto, alias) name proto; \
	_Pragma("let " #name " = " #alias)
*/


/* GCC has various useful declarations that can be made with the
   `__attribute__' syntax.  All of the ways we use this do fine if
   they are omitted for compilers that don't understand it. */


/* At some point during the gcc 2.96 development the `malloc' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */

/* Ignore */


/* At some point during the gcc 2.96 development the `pure' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */

/* Ignore */


/* At some point during the gcc 2.8 development the `format_arg' attribute
   for functions was introduced.  We don't want to use it unconditionally
   (although this would be possible) since it generates warnings.  */




/* It is possible to compile containing GCC extensions even if GCC is
   run in pedantic mode if the uses are carefully marked using the
   `__extension__' keyword.  But this is not generally available before
   version 2.8.  */


/* __restrict is known in EGCS 1.2 and above. */


/* ISO C99 also allows to declare arrays as non-overlapping.  The syntax is
     array_name[restrict]
   But gcc so far does not support this syntax.  We define a separate macro
   for this which can be enabled if the underlying compiler supports it.  */


/* sys/cdefs.h */



/* If we don't have __REDIRECT, prototypes will be missing if
   __USE_FILE_OFFSET64 but not __USE_LARGEFILE[64]. */


/* !ASSEMBLER */

/* Decide whether we can define 'extern inline' functions in headers.  */


/* This is here only because every header file already includes this one.  */

/* Get the definitions of all the appropriate `__stub_FUNCTION' symbols.
   <gnu/stubs.h> contains `#define __stub_FUNCTION' when FUNCTION is a stub
   which will always return failure (and set errno to ENOSYS).

   We avoid including <gnu/stubs.h> when compiling the C library itself to
   avoid a dependency loop.  stubs.h depends on every object file.  If
   this #include were done for the library source code, then every object
   file would depend on stubs.h.  */

/* This file is automatically generated.
   It defines a symbol `__stub_FUNCTION' for each function
   in the C library which is a stub, meaning it will fail
   every time called, usually setting errno to ENOSYS.  */

































/* features.h  */


 

/* Get size_t and NULL from <stddef.h>.  */




/* Any one of these symbols __need_* means that GNU libc
   wants us just to define one data type.  So don't define
   the symbols that indicate this file's entire job has been done.  */



/* This avoids lossage on SunOS but only if stdtypes.h comes first.
   There's no way to win with the other order!  Sun lossage.  */

/* On 4.3bsd-net2, make sure ansi.h is included, so we have
   one less case to deal with in the following.  */


/* In 4.3bsd-net2, machine/ansi.h defines these symbols, which are
   defined if the corresponding type is *not* defined.
   FreeBSD-2.1 defines _MACHINE_ANSI_H_ instead of _ANSI_H_ */
/* defined(_ANSI_H_) || defined(_MACHINE_ANSI_H_) */

/* Sequent's header files use _PTRDIFF_T_ in some conflicting way.
   Just ignore it.  */


/* On VxWorks, <type/vxTypesBase.h> may have defined macros like
   _TYPE_size_t which will typedef size_t.  fixincludes patched the
   vxTypesBase.h so that this macro is only defined if _GCC_SIZE_T is
   not defined, and so that defining this macro defines _GCC_SIZE_T.
   If we find that the macros are still defined at this point, we must
   invoke them so that the type is defined as expected.  */




/* In case nobody has defined these types, but we aren't running under
   GCC 2.00, make sure that __PTRDIFF_TYPE__, __SIZE__TYPE__, and
   __WCHAR_TYPE__ have reasonable values.  This can happen if the
   parts of GCC is compiled by an older compiler, that actually
   include gstddef.h, such as collect2.  */

/* Signed type of difference of two pointers.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* _STDDEF_H or __need_ptrdiff_t.  */

/* Unsigned type of `sizeof' something.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */

/* BeOS */
/* in case <sys/types.h> has defined it. */












/* BeOS */















typedef unsigned int size_t;
/* __BEOS__ */
/* !(defined (__GNUG__) && defined (size_t)) */
/* __size_t */
/* _SIZET_ */
/* _GCC_SIZE_T */
/* ___int_size_t_h */
/* _SIZE_T_DEFINED */
/* _SIZE_T_DEFINED_ */
/* _BSD_SIZE_T_ */
/* _SIZE_T_ */
/* __SIZE_T */
/* _T_SIZE */
/* _T_SIZE_ */
/* _SYS_SIZE_T_H */
/* _SIZE_T */
/* __size_t__ */

/* _STDDEF_H or __need_size_t.  */


/* Wide character type.
   Locale-writers should change this as necessary to
   be big enough to hold unique values not between 0 and 127,
   and not (wchar_t) -1, for each defined multibyte character.  */

/* Define this type if we are doing the whole job,
   or if we want this type in particular.  */
/* _STDDEF_H or __need_wchar_t.  */



/*  In 4.3bsd-net2, leave these undefined to indicate that size_t, etc.
    are already defined.  */
/*  BSD/OS 3.1 requires the MACHINE_ANSI_H check here.  FreeBSD 2.x apparently
    does not, even though there is a check for MACHINE_ANSI_H above.  */
/* _ANSI_H_ || ( __bsdi__ && _MACHINE_ANSI_H_ ) */

/* __sys_stdtypes_h */

/* A null pointer constant.  */


/* in case <stdio.h> has defined it. */
/* G++ */

/* G++ */
/* NULL not defined and <stddef.h> or need NULL.  */


/* _STDDEF_H was defined this time */

/* !_STDDEF_H && !_STDDEF_H_ && !_ANSI_STDDEF_H && !__STDDEF_H__
	  || __need_XXX was not defined before */



/* Copy N bytes of SRC to DEST.  */
extern void *memcpy (void *__restrict __dest,
		     __const void *__restrict __src, size_t __n)  ;
/* Copy N bytes of SRC to DEST, guaranteeing
   correct behavior for overlapping strings.  */
extern void *memmove (void *__dest, __const void *__src, size_t __n)
      ;

/* Copy no more than N bytes of SRC to DEST, stopping when C is found.
   Return the position in DEST one byte past where C was copied,
   or NULL if C was not found in the first N bytes of SRC.  */

extern void *memccpy (void *__restrict __dest, __const void *__restrict __src,
		      int __c, size_t __n)
      ;
/* SVID.  */


/* Set N bytes of S to C.  */
extern void *memset (void *__s, int __c, size_t __n)  ;

/* Compare N bytes of S1 and S2.  */
extern int memcmp (__const void *__s1, __const void *__s2, size_t __n)
        ;

/* Search N bytes of S for C.  */
extern void *memchr (__const void *__s, int __c, size_t __n)
         ;




/* Copy SRC to DEST.  */
extern char *strcpy (char *__restrict __dest, __const char *__restrict __src)
      ;
/* Copy no more than N characters of SRC to DEST.  */
extern char *strncpy (char *__restrict __dest,
		      __const char *__restrict __src, size_t __n)  ;

/* Append SRC onto DEST.  */
extern char *strcat (char *__restrict __dest, __const char *__restrict __src)
      ;
/* Append no more than N characters from SRC onto DEST.  */
extern char *strncat (char *__restrict __dest, __const char *__restrict __src,
		      size_t __n)  ;

/* Compare S1 and S2.  */
extern int strcmp (__const char *__s1, __const char *__s2)
        ;
/* Compare N characters of S1 and S2.  */
extern int strncmp (__const char *__s1, __const char *__s2, size_t __n)
        ;

/* Compare the collated forms of S1 and S2.  */
extern int strcoll (__const char *__s1, __const char *__s2)
        ;
/* Put a transformation of SRC into no more than N bytes of DEST.  */
extern size_t strxfrm (char *__restrict __dest,
		       __const char *__restrict __src, size_t __n)  ;




/* Duplicate S, returning an identical malloc'd string.  */
extern char *strdup (__const char *__s)    ;


/* Return a malloc'd copy of at most N bytes of STRING.  The
   resultant string is terminated even if no null terminator
   appears before STRING[N].  */




/* Find the first occurrence of C in S.  */
extern char *strchr (__const char *__s, int __c)    ;
/* Find the last occurrence of C in S.  */
extern char *strrchr (__const char *__s, int __c)    ;



/* Return the length of the initial segment of S which
   consists entirely of characters not in REJECT.  */
extern size_t strcspn (__const char *__s, __const char *__reject)
        ;
/* Return the length of the initial segment of S which
   consists entirely of characters in ACCEPT.  */
extern size_t strspn (__const char *__s, __const char *__accept)
        ;
/* Find the first occurrence in S of any character in ACCEPT.  */
extern char *strpbrk (__const char *__s, __const char *__accept)
        ;
/* Find the first occurrence of NEEDLE in HAYSTACK.  */
extern char *strstr (__const char *__haystack, __const char *__needle)
        ;



/* Divide S into tokens separated by characters in DELIM.  */
extern char *strtok (char *__restrict __s, __const char *__restrict __delim)
      ;

/* Divide S into tokens separated by characters in DELIM.  Information
   passed between calls are stored in SAVE_PTR.  */
extern char *__strtok_r (char *__restrict __s,
			 __const char *__restrict __delim,
			 char **__restrict __save_ptr)  ;

extern char *strtok_r (char *__restrict __s, __const char *__restrict __delim,
		       char **__restrict __save_ptr)  ;





/* Return the length of S.  */
extern size_t strlen (__const char *__s)    ;




/* Return a string describing the meaning of the `errno' code in ERRNUM.  */
extern char *strerror (int __errnum)  ;

/* Reentrant version of `strerror'.  If a temporary buffer is required, at
   most BUFLEN bytes of BUF will be used.  */
extern char *__strerror_r (int __errnum, char *__buf, size_t __buflen)  ;
extern char *strerror_r (int __errnum, char *__buf, size_t __buflen)  ;


/* We define this function always since `bzero' is sometimes needed when
   the namespace rules does not allow this.  */
extern void __bzero (void *__s, size_t __n)  ;


/* Copy N bytes of SRC to DEST (like memmove, but args reversed).  */
extern void bcopy (__const void *__src, void *__dest, size_t __n)  ;

/* Set N bytes of S to 0.  */
extern void bzero (void *__s, size_t __n)  ;

/* Compare N bytes of S1 and S2 (same as memcmp).  */
extern int bcmp (__const void *__s1, __const void *__s2, size_t __n)
        ;

/* Find the first occurrence of C in S (same as strchr).  */
extern char *index (__const char *__s, int __c)    ;

/* Find the last occurrence of C in S (same as strrchr).  */
extern char *rindex (__const char *__s, int __c)    ;

/* Return the position of the first bit set in I, or 0 if none are set.
   The least-significant bit is position 1, the most-significant 32.  */
extern int __ffs (int __i)   __attribute__ ((const));
extern int ffs (int __i)   __attribute__ ((const));

/* The following two functions are non-standard but necessary for non-32 bit
   platforms.  */


/* Compare S1 and S2, ignoring case.  */
extern int strcasecmp (__const char *__s1, __const char *__s2)
        ;

/* Compare no more than N chars of S1 and S2, ignoring case.  */
extern int strncasecmp (__const char *__s1, __const char *__s2, size_t __n)
        ;
/* Use BSD.  */




/* Return the next DELIM-delimited token from *STRINGP,
   terminating it with a '\0', and update *STRINGP to point past it.  */
extern char *strsep (char **__restrict __stringp,
		     __const char *__restrict __delim)  ;









 

/* string.h  */


/* 
  Make things simpler when working between char* and xmlChar*  .
   By definition a char is the same size as an xmlChar(unsigned char). 
*/








/* what char is use to separate directories in an URI*/


    /* Handle the differences in path and quote character between
     * win32 and *nix systems */





/* JRF: Although RISC OS native paths use . as a separator, the arguments
        to xsldbg are passed in unix or URI form, and thus the above
        specification is correct. */







/**
 * Load the stylesheet and return it 
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */


    xsltStylesheetPtr loadStylesheet(void);





/**
 * Load the xml data file and return it  
 *
 * @returns The stylesheet after reloading it if successful
 *         NULL otherwise
 */


    xmlDocPtr loadXmlData(void);





/**
 * Load the temporary data file and return it 
 *
 * @returns The temporary file after reloading it if successful,
 *         NULL otherwise
 * @param path The name of temporary file to loa
 */


    xmlDocPtr loadXmlTemporary(const xmlChar * path);




