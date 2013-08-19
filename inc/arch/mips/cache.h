/**************************************************************************
*                                                                         *
*   PROJECT     : MIPS port for uC/OS-II                                  *
*                                                                         *
*   MODULE      : CACHE.h                                                 *
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
*   External interface for CACHE related functions.                       *
*                                                                         *
**************************************************************************/


#ifndef __CACHE_H__
#define __CACHE_H__

#include "sysdefs.h"

#ifdef __cplusplus
extern "C" {
#endif


/* ********************************************************************* */
/* Module configuration */


/* ********************************************************************* */
/* Interface macro & data definition */

#ifndef _ASSEMBLER_
extern U8  L2CACHE_bEnabled;
extern U8  CPU_b64Bit;
#else
EXTERN( L2CACHE_bEnabled )
EXTERN( CPU_b64Bit )
#endif



/* ********************************************************************* */
/* Interface function definition */

#ifndef _ASSEMBLER_

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
void ICACHE_vInvalidateAddr(U32 wAddr, U32 wPipeFlush);

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
void IPIPE_vFlush(U32 wAddr);

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
void DCACHE_vFlushAddr(U32 wAddr);

#endif /* _ASSEMBLER_ */


/* ********************************************************************* */

#ifdef __cplusplus
}
#endif

#endif /*__CACHE_H__*/
