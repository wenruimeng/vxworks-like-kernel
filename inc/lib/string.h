/**************************************************************************
*                                                                         *
*   PROJECT     : TMON (Transparent monitor)                              *
*                                                                         *
*   MODULE      : STRING.h                                                *
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


#ifndef __STRING_H__
#define __STRING_H__

#include "sysdefs.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */


/* ********************************************************************* */
/* Interface function definition */

extern char
	*strcpy(char *, const char *),
	*strncpy(char *, const char *, size_t),
	*strcat(char *, const char *),
	*strncat(char *, const char *, size_t),
	*strchr(const char *, int),
	*strstr(const char *, const char *),
	*strrchr(char *, int),
	*strpbrk(char *, char *),
	*strtok(char *, char *),
	*strxcpy(char *, char *, int),
	*strerror(int);
extern unsigned long
   strtoul(const char *,char **,int);
extern long 
   strtol(const char *,char **,int);
extern int
	strcmp(const char *, const char *),
	strncmp(const char *, const char *, size_t),
	strlen(const char *),
	strspn(const char *, const char *),
	strcspn(const char *, const char *),
	strcasecmp(const char *, const char *);
extern void
	*memccpy(void *, const void *, int, size_t),
	*memchr(const void *, int, size_t),
	*memcpy(void *, const void *, size_t),
	*memset(void *, int, size_t),
	*memmove(void *, const void *, size_t);
extern int
	memcmp(const void *, const void *, size_t);


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /* __STRING_H__ */
