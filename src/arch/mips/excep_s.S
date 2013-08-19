/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : EXCEP_S.s                                               *
*                                                                         *
*   AUTHOR      : Michael Anburaj                                         *
*                 URL  : http://geocities.com/michaelanburaj/             *
*                 EMAIL: michaelanburaj@hotmail.com                       *
*                                                                         *
*   PROCESSOR   : MIPS                                                    *
*                                                                         *
*   TOOL-CHAIN  : SDE & Cygnus                                            *
*                                                                         *
*   DESCRIPTION :                                                         *
*   First level exception handling functions.                             *
*                                                                         *
**************************************************************************/


#include "sysdefs.h"
#include "mips.h"
#include "frmwrk.h"
#include "cache.h"
#include "atlas.h" /* remove this */


/* ********************************************************************* */
/* Global definitions */


/* ********************************************************************* */
/* File local definitions */


/* ********************************************************************* */
/* Local functions */
#if 0
        .set noat
        .set noreorder

SLEAF( __vTmonRamVectors )

ram_int_vector:
        /*  This code is copied to the int ram exception vector
         *  locations (2 words are copied).
         */
        j       UCOS_INTHandler
        nop
        
END( __vTmonRamVectors )
#endif

/* ********************************************************************* */
/* Global functions */
#if 0
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

LEAF( EXCEP_vInstallIntInRam )

        /* Install RAM exception vectors */

#define ADDR                      a0  /* also used as arg to sys_cache..() */
#define STATUS_INT                t3
#define RA                        t4

        move    RA, ra

        /* Disable interrupts */
        MFC0(   t1, C0_Status )
        and     STATUS_INT, t1, M_StatusIE
        xor     t1, STATUS_INT        
        MTC0(   t1, C0_Status )

        /**** install tmon ram vectors ****/
        la      t2, ram_int_vector
        jal     setup_tmon_vector
/*        li      ADDR, 0x200*/        /* Int, CauseIV=1 */
		li		ADDR, 0x180

        /* Clear BEV so that ram handlers are used */
        MFC0(   a0, C0_Status)
        li      t2, ~M_StatusBEV
        and     a0, t2
        MTC0(   a0, C0_Status)

        /* Restore interrupt enable state */
        or      a0, STATUS_INT
        MTC0(   a0, C0_Status )

        move    ra, RA

        /* Done */
        jr      ra
        nop


setup_tmon_vector:
        li      t1, KSEG0BASE
        or      ADDR,t1
        
        lw      t1, 0(t2)
        sw      t1, 0(ADDR)
        lw      t1, 4(t2)
        sw      t1, 4(ADDR)

        /* Whenever an instruction has been stored to KSEG0, we must
         * make sure the instruction has been flushed to physical memory
         * and invalidate the corresponding I-Cache line.
         *     sys_dcache_flush_addr modifies only t1.
         *     sys_icache_invalidate_addr modifies only t1.
         */

        move    t2, ra

        jal     DCACHE_vFlushAddr
        nop
        jal     ICACHE_vInvalidateAddr
        move    a1, zero /* No need to flush pipeline */

        jr      t2
        nop

END( EXCEP_vInstallIntInRam )
#endif


/* ********************************************************************* */
