/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : INIT.s                                                  *
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
*   Startup code for RAM application over TMON.                           *
*                                                                         *
**************************************************************************/

/* Updated by jannson copy and modify from uC/OS-II */

#include "sysdefs.h"
#include "mips.h"
#include "archdefs.h"
#include "frmwrk.h"

	
      .text

/*
*********************************************************************************************
*                                       _start
*
* Description: This routine initializes sp and gp registers, clears bss section and starts
*              the application C code.
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    : Function linked to application start address.
*********************************************************************************************
*/
      .globl _start
      .ent   _start
	
_start:	

      .set noreorder

      /* Setup sp (top of memory) and gp */
      la    gp, _gp
      MFC0(v0,C0_STATUS)
      and v1,v0,0xfffffffe
      MTC0(v1,C0_STATUS)
      la    sp, _freemem
      li    t0, _SYS_STKSIZE
      addu  sp, t0
      
      /* Align sp to 16 byte boundary (required by Cygnus) */
      li    t0, ~0xf
      and   sp, t0
      
      /* Clear bss */

      la    t0, _fbss	/* First address */
      la    t1, _end	/* Last  address */
1:
      sw    zero, 0(t0)
      bne   t0, t1, 1b
      addiu t0, 4

      /* Get ready to jump to main */
      move  s0, ra
      la    t0, C_vMain
      /* Jump to main */
      jal   t0
      nop

      /* Return to monitor */
      jr    s0
      nop	

      .set reorder

      .end   _start


/*
*********************************************************************************************
*                                       __main, _gccmain
*
* Description: This routine initializes sp and gp registers, clears bss section and starts
*              the application C code.
*
* Arguments  : none
*
* Return     : none.
*
* Note(s)    : Function linked to application start address.
*********************************************************************************************
*/
/************************************************************************
 *
 *                          __main, _gccmain
 *  Description :
 *  -------------
 *
 *  Dummy functions called by main() function.
 *
 *  GNU-gcc 2.8.1 : main() calls __main
 *  GNU-gcc 2.9   : main() calls __gccmain
 *	
 *  Return values :
 *  ---------------
 *
 *  None
 *
 ************************************************************************/

      .set noreorder
		
      .globl __main
      .ent   __main

__main:	
      jr	ra
      nop

      .end   __main


	
      .globl __gccmain
      .ent   __gccmain

__gccmain:
      jr	ra
      nop

      .end   __gccmain
	
	
/* ********************************************************************* */
