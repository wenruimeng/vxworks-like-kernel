/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : FRMWRK.h                                                *
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
*   External interface & Module configuration header for FRMWRK           *
*   Infrastructure module.                                                *
*                                                                         *
**************************************************************************/


#ifndef __FRMWRK_H__
#define __FRMWRK_H__

#include "sysdefs.h"


/* ********************************************************************* */
/* Module configuration */

#define __mLogo "MIPS_RAIN"

#define __mDate "30Jan12"             /* Revision date */
#define __mVer  "1.00"                 /* Revision number */

#define __mAuthor "Janson, http://www.gandancing.com/"
#define __mProcessor "4Kc (MIPS 4K processor)"

#define __nConsolPort         0        /* Consol UART port number */
#define __nConsolBaud         115200   /* Consol Baud rate */
#define __nConsolFifoEn       False     /* FIFO Enable flag */

#define MCLK (80000000)                /* 80Mhz */

#ifndef _SYS_STKSIZE
#define _SYS_STKSIZE 20*1024
#endif
#ifndef _EXC_STKSIZ
#define _EXC_STKSIZE 20*1024
#endif


/* ********************************************************************* */
/* Interface macro & data definition */

#define C_abRTMem


#ifndef _ASSEMBLER_

/* ********************************************************************* */
/* Interface function definition */

/*
*********************************************************************************************
*                                       FRMWRK_vRamTest
*
* Description: This routine is used do RAM test.
*
* Arguments  : pbID   - Test identification string.
*              wSAddr - RAM starting address.
*              wEAddr - RAM end address.
*
* Return     : none.
*
* Note(s)    : Do not test the following areas using this routine:
*                       1. Stack
*                       2. Software vector table
*                       3. C Variables (RW & ZI)
*********************************************************************************************
*/
void FRMWRK_vRamTest(U8 *pbID,U32 wSAddr,U32 wEAddr);

/*
*********************************************************************************************
*                                       FRMWRK_vStartTicker
*
* Description: This routine starts Timer1 (TC1) in the Prescale mode for OS Tick.
*
* Arguments  : wTicksPerSec - Time ticks per second.
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
void FRMWRK_vStartTicker(U32 wTicksPerSec); /* wTicksPerSec = 10...10000 ticks per sec */

/*
*********************************************************************************************
*                                       APP_vMain
*
* Description: This is the Application main entry function.
*
* Arguments  : none.
*
* Return     : none.
*
* Note(s)    : This function needs to be implemented in the Application.
*********************************************************************************************
*/
void APP_vMain(void);

#endif /* _ASSEMBLER_ */


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /*__FRMWRK_H__*/
