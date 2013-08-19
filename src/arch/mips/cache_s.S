/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : CACHE_S.s                                               *
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
*   CPU cache related functions.                                          *
*                                                                         *
**************************************************************************/


#include <sysdefs.h>
#include <mips.h>
#include "cache.h"


/* ********************************************************************* */
/* Global definitions */


/* ********************************************************************* */
/* File local definitions */


/* ********************************************************************* */
/* Local functions */


/* ********************************************************************* */
/* Global functions */

	.set noreorder

/*
*********************************************************************************************
*                                       ICACHE_vInvalidateAddr
*
* Description: Invalidate I cache line containing specified address.
*
*              We bypass the cache operations if CPU is running uncached.
*              (We assume Config.k0 == K_CacheAttrU is the
*              only uncached mode used).
*
* Arguments  : a0 holds the address, MUST be KSEG0/KSEG1
*              a1 != 0 => flush pipeline after the invalidation.
*
* Return     : none.
*
* Note(s)    : This function is called from user2tmon() in application context,
*              possibly in 64 bit mode and with invalid gp.
*              if a1 == 0, ICACHE_vInvalidateAddr modifies only t1.
*********************************************************************************************
*/
LEAF(ICACHE_vInvalidateAddr)

    	MFC0( t1, C0_Config )
	andi  t1, M_ConfigK0
	xori  t1, K_CacheAttrU << S_ConfigK0
	beq   t1, zero, 2f
	nop
	
	la	t1, L2CACHE_bEnabled
	lb	t1, 0(t1)
	beq	t1, zero, 1f
	nop

	/*  This secondary cache operation will also invalidate $I,
	 *  so we don't need to invalidate $I.
	 */
SET_MIPS3()
	SCACHE_ADDR_HIT_WB_INVALIDATE_OP(a0)
SET_MIPS0()
	b	2f
	nop
1:		
SET_MIPS3()
	ICACHE_ADDR_INVALIDATE_OP(a0,t1)
SET_MIPS0()
2:
	bne	a1, zero, IPIPE_vFlush
	nop
	jr      ra
	nop

END(ICACHE_vInvalidateAddr)

/*
*********************************************************************************************
*                                       IPIPE_vFlush
*
* Description: Flush pipeline
*
* Arguments  : a0 holds the index
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
LEAF(IPIPE_vFlush)
	MFC0(	t0, C0_Status )
	
	/* Set STATUS in a well defined state so that we can perform
	 * a controlled eret (one without modifying shadow registers
	 * in case of a MIPS32/MIPS64 release 2 CPU).
	 *
	 * Note that we may end up here due a user application calling
	 * e.g. SYSCON through the application interface. So, STATUS
	 * is not necessarily in a well known state at this point.
	 *
	 * We need to make sure STATUS has the following settings :
	 *
	 * ERL = 0  (so EPC is used rather than ErrorEPC)
	 * BEV = 1  (so shadowsets are not shifted)
	 * IE  = 0  (so interrupts are disabled)
	 * KSU = 00 (so that we end up in kernel mode after eret)
	 */


	/* First set KSU=00, IE=0 */
	ori	t1, t0, (M_StatusKSU | M_StatusIE)
	xori	t1,     (M_StatusKSU | M_StatusIE)
	MTC0(	t1, C0_Status )
	/* Now set BEV */	
	li	t2, M_StatusBEV
	or	t1, t2
	MTC0(	t1, C0_Status )
	/* Finally, clear ERL */
	ori	t1, M_StatusERL
	xori	t1, M_StatusERL
	MTC0(	t1, C0_Status )

	la	t1, CPU_b64Bit
	lb	t1, 0(t1)
	bne	t1, zero, 1f
	nop

	/* 32 bit CPU */
	MFC0(   t1, C0_EPC ) /* Store EPC */

	/* Now setup EPC and perform eret */	
	la	t2, after_eret32
	MTC0(	t2, C0_EPC )

SET_MIPS3()
	eret
SET_MIPS0()

after_eret32 :
	MTC0(   t1, C0_EPC ) /* Restore EPC */
	b	2f
	nop
	
1:	
	/* 64 bit CPU */
SET_MIPS3()
	DMFC0(  t1, C0_EPC ) /* Store EPC */

	/* Now setup EPC and perform eret */	
	la	t2, after_eret64
	DMTC0(	t2, C0_EPC )

	eret

after_eret64 :
	DMTC0(  t1, C0_EPC ) /* Restore EPC */
SET_MIPS0()
	
2:	
	/* Restore STATUS and return */
	MTC0(	t0, C0_Status )
	jr	ra
	nop

END(IPIPE_vFlush)

/*
*********************************************************************************************
*                                       DCACHE_vFlushAddr
*
* Description: Flush D cache line containing specified address.
*
*              We bypass the cache operations if CPU is running uncached.
*              (We assume Config.k0 == K_CacheAttrU is the
*              only uncached mode used).
*
* Arguments  : a0 holds the address, MUST be KSEG0/KSEG1.
*
* Return     : none.
*
* Note(s)    : This function is called from user2tmon() in application context,
*              possibly in 64 bit mode and with invalid gp.
*              DCACHE_vFlushAddr modifies only t1.
*********************************************************************************************
*/
LEAF(DCACHE_vFlushAddr)

	MFC0( t1, C0_Config )
	andi  t1, M_ConfigK0
	xori  t1, K_CacheAttrU << S_ConfigK0
	beq   t1, zero, 2f
	nop
	
	la	t1, L2CACHE_bEnabled
	lb	t1, 0(t1)
	beq	t1, zero, 1f
	nop

	/*  This secondary cache operation will also flush $D,
	 *  so we don't need to flush $D.
	 */
SET_MIPS3()
	SCACHE_ADDR_HIT_WB_INVALIDATE_OP(a0)
SET_MIPS0()
	b	2f
	nop
1:		
SET_MIPS3()
	cache	DCACHE_ADDR_HIT_WRITEBACK_INVALIDATE, 0(a0)
SET_MIPS0()
2:
	sync
	jr	ra
	lb	zero, 0(a0)

END(DCACHE_vFlushAddr)

/*
*********************************************************************************************
*                                       
*
* Description: 
*
* Arguments  : 
*
* Return     : none.
*
* Note(s)    : 
*********************************************************************************************
*/
//LEAF()

//END()

/* ********************************************************************* */ 
