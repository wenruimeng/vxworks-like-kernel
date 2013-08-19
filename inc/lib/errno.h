/**************************************************************************
*                                                                         *
*   PROJECT     : TMON (Transparent monitor)                              *
*                                                                         *
*   MODULE      : ERRNO.h                                                 *
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


#ifndef __ERRNO_H__
#define __ERRNO_H__

#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */

/* NOTE: In general, error codes should be handled through the "syserror"
 * system -- see "syserror.h".
 * The errno.h include file is only included for std c compatability reasons.
 */


/*
 * Error codes for standard library functions, 
 * - add new ones when required
 */

#define	ERANGE		34	/* Result not representable		*/

extern int errno;


/* ********************************************************************* */
/* Interface function definition */


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif	/* __ERRNO_H__ */
