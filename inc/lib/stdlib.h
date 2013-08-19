/**************************************************************************
*                                                                         *
*   PROJECT     : TMON (Transparent monitor)                              *
*                                                                         *
*   MODULE      : STDLIB.h                                                *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : MIPS 4Kc (32 bit RISC) - ATLAS board                    *
*                                                                         *
*   TOOL-CHAIN  : SDE & Cygnus                                            *
*                                                                         *
*   DESCRIPTION :                                                         *
*   This is a std header file.                                            *
*                                                                         *
**************************************************************************/


#ifndef __STDLIB_H__
#define __STDLIB_H__

/* sysdefs.h is necessary for size_t declaration*/
#include "sysdefs.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************* */
/* Module configuration */

/* ISO C 7.10.2 - Pseudo-random sequence generation functions */
/* Maximum value returned by rand().  */
#define RAND_MAX  2147483647


/* ********************************************************************* */
/* Interface macro & data definition */


/* ********************************************************************* */
/* Interface function definition */

extern void qsort( char *base, int  n, int  size, int  (*compar)() );

extern int atoi(char *s);

extern int rand( void );
extern void srand( unsigned int /* seed */ );

/* POSIX 1003.1 section 8.3.8 rand_r() */
extern int rand_r( unsigned int * /* seed */ );

/* Allocate SIZE bytes of memory.  */
extern void *malloc (size_t __size);
/* Allocate NMEMB elements of SIZE bytes each, all initialized to 0.  */
extern void *calloc (size_t __nmemb, size_t __size);

/* Re-allocate the previously allocated block
   in PTR, making the new block SIZE bytes long.  */
extern void *realloc (void *__ptr, size_t __size);
/* Free a block allocated by `malloc', `realloc' or `calloc'.  */
extern void free (void *__ptr);


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /* __STDLIB_H__ */
