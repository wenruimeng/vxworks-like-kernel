/**************************************************************************
*                                                                         *
*   PROJECT     : TMON (Transparent monitor)                              *
*                                                                         *
*   MODULE      : STDIO.h                                                 *
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


#ifndef __STDIO_H__
#define __STDIO_H__

/* sysdefs.h is necessary for size_t declaration*/
#include "sysdefs.h"
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */

typedef struct {
  /* Note: the FILE struct members are not publicly accessible. Please
     use only the API calls for forwards compatibility */
  unsigned char* _start;
  unsigned char* _current;
  unsigned char* _eof;
  char _name[512];
  char _mode;
} FILE;


/* ********************************************************************* */
/* Interface function definition */

extern int printf(const char *, ...),
           sprintf(char *, const char *, ...),
           unformat(int (*)(), char *,int **),
           sscanf(char *s, char *fmt, ...);

extern char *index(char *s, int  c);

extern FILE* fopen(const char* filename, const char* options);
extern int   fclose(FILE* file);
extern size_t fread(void *ptr, size_t size, size_t nitems, FILE *stream);
extern size_t fwrite(const void *ptr, size_t size, size_t nitems, FILE *stream);

extern int vsprintf(char *buf, char *fmt, va_list ap);


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /* __STDIO_H__ */
