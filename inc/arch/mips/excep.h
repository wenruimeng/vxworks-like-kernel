/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : EXCEP.h                                                 *
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
*   External interface header for First level  Exception handling module. *
*                                                                         *
**************************************************************************/

#ifndef __EXCEP_H__
#define __EXCEP_H__


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */

#ifndef _ASSEMBLER_
/* ********************************************************************* */
/* Interface function definition */

/*
*********************************************************************************************
*                                       EXCEP_vInstallIntInRam
*
* Description: This routine is used to install the INT handler in RAM.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void EXCEP_vInstallIntInRam(void);

#endif


/* ********************************************************************* */

#endif /*__EXCEP_H__*/
