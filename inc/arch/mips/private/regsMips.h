/* regsMips.h - MIPS R3000 registers */

/* Copyright 1984-2001 Wind River Systems, Inc. */

/*
 * This file has been developed or significantly modified by the
 * MIPS Center of Excellence Dedicated Engineering Staff.
 * This notice is as per the MIPS Center of Excellence Master Partner
 * Agreement, do not remove this notice without checking first with
 * WR/Platforms MIPS Center of Excellence engineering management.
 */

/*
modification history
--------------------
01l,16jul01,ros  add CofE comment
01k,30dec97,dbt  added common names for registers
01j,16jan96,mem  added cause and fpcsr to REG_SET.
01i,18oct93,cd   added R4000 support.
01h,22sep92,rrr  added support for c++
01g,09jul92,ajm  added defines for ASMLANGUAGE signals
01f,03jun92,ajm  updated file name references to match real name
01e,26may92,rrr  the tree shuffle
01d,12mar92,yao  added SR_OFFSET, PC_OFFSET, GREG_BASE,GREG_OFFSET.
01c,04oct91,rrr  passed through the ansification filter
		  -changed ASMLANGUAGE to _ASMLANGUAGE
		  -changed copyright notice
01b,27may91,ajm  reordered REG_SET to mirror R3KESF
01a,01apr91,ajm  derived from 01d of 68K.
*/

#ifndef __INCregsMipsh
#define __INCregsMipsh

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	_ASSEMBLER_

typedef struct		/* REG_SET - MIPS architecture register set */
    {
    ULONG sr;			/* process status register */
    INSTR *pc;			/* program counter - really epc! */
    _RType lo;			/* divide lo register */
    _RType hi;			/* divide hi register */
    _RType gpreg[32];		/* data registers */
    /* cause & fpcsr are only set for wdb traps, from ESFMIPS */
    ULONG cause;               /* cause reg for branch bit in getNpc() */
    ULONG fpcsr;               /* fp cause reg for getNpc() */
    } REG_SET;

/* some common names for registers */

#define zeroReg		gpreg[0]	/* wired zero */
#define atReg		gpreg[1]	/* assembler temp */
#define v0Reg		gpreg[2]	/* function return 0 */
#define v1Reg		gpreg[3]	/* function return 1 */
#define a0Reg		gpreg[4]	/* parameter 0 */
#define a1Reg		gpreg[5]	/* parameter 1 */
#define a2Reg		gpreg[6]	/* parameter 2 */
#define a3Reg		gpreg[7]	/* parameter 3 */
#define t0Reg		gpreg[8]	/* caller saved 0 */
#define t1Reg		gpreg[9]	/* caller saved 1 */
#define t2Reg		gpreg[10]	/* caller saved 2 */
#define t3Reg		gpreg[11]	/* caller saved 3 */
#define t4Reg		gpreg[12]	/* caller saved 4 */
#define t5Reg		gpreg[13]	/* caller saved 5 */
#define t6Reg		gpreg[14]	/* caller saved 6 */
#define t7Reg		gpreg[15]	/* caller saved 7 */
#define s0Reg		gpreg[16]	/* callee saved 0 */
#define s1Reg		gpreg[17]	/* callee saved 1 */
#define s2Reg		gpreg[18]	/* callee saved 2 */
#define s3Reg		gpreg[19]	/* callee saved 3 */
#define s4Reg		gpreg[20]	/* callee saved 4 */
#define s5Reg		gpreg[21]	/* callee saved 5 */
#define s6Reg		gpreg[22]	/* callee saved 6 */
#define s7Reg		gpreg[23]	/* callee saved 7 */
#define t8Reg		gpreg[24]	/* caller saved 8 */
#define t9Reg		gpreg[25]	/* caller saved 8 */
#define k0Reg		gpreg[26]	/* kernal reserved 0 */
#define k1Reg		gpreg[27]	/* kernal reserved 1 */
#define gpReg		gpreg[28]	/* global pointer */
#define spReg		gpreg[29]	/* stack pointer */
#define s8Reg		gpreg[30]	/* callee saved 8 */
#define fpReg           s8Reg           /* gcc says this is frame pointer */
#define raReg		gpreg[31]	/* return address */
#define reg_pc		pc		/* program counter */
#define reg_sp		spReg		/* stack pointer */
#define reg_fp		fpReg		/* frame pointer */	

#define SR_OFFSET	(0*4)		/* sr register offset */
#define PC_OFFSET	(1*4)		/* pc register offset */

#endif	/* _ASMLANGUAGE */

#define GREG_BASE	(2*4+2*_RTypeSize)
#define GREG_OFFSET(n)  (GREG_BASE + (n)*_RTypeSize)

#define	SRREG		(0*4)			/* process status register */
#define PCREG		(1*4)			/* program counter */
#define	LOREG		(2*4+0*_RTypeSize)	/* lo register */
#define	HIREG		(2*4+1*_RTypeSize)	/* hi register */
#define ZEROREG		GREG_OFFSET(0)		/* wired zero */
#define ATREG		GREG_OFFSET(1)		/* assembler temp */
#define V0REG		GREG_OFFSET(2)		/* function return 0 */
#define V1REG		GREG_OFFSET(3)		/* function return 1 */
#define A0REG		GREG_OFFSET(4)		/* parameter 0 */
#define A1REG		GREG_OFFSET(5)		/* parameter 1 */
#define A2REG		GREG_OFFSET(6)		/* parameter 2 */
#define A3REG		GREG_OFFSET(7)		/* parameter 3 */
#define T0REG		GREG_OFFSET(8)		/* caller saved 0 */
#define T1REG		GREG_OFFSET(9)		/* caller saved 1 */
#define T2REG		GREG_OFFSET(10)		/* caller saved 2 */
#define T3REG		GREG_OFFSET(11)		/* caller saved 3 */
#define T4REG		GREG_OFFSET(12)		/* caller saved 4 */
#define T5REG		GREG_OFFSET(13)		/* caller saved 5 */
#define T6REG		GREG_OFFSET(14)		/* caller saved 6 */
#define T7REG		GREG_OFFSET(15)		/* caller saved 7 */
#define S0REG		GREG_OFFSET(16)		/* callee saved 0 */
#define S1REG		GREG_OFFSET(17)		/* callee saved 1 */
#define S2REG		GREG_OFFSET(18)		/* callee saved 2 */
#define S3REG		GREG_OFFSET(19)		/* callee saved 3 */
#define S4REG		GREG_OFFSET(20)		/* callee saved 4 */
#define S5REG		GREG_OFFSET(21)		/* callee saved 5 */
#define S6REG		GREG_OFFSET(22)		/* callee saved 6 */
#define S7REG		GREG_OFFSET(23)		/* callee saved 7 */
#define T8REG		GREG_OFFSET(24)		/* caller saved 8 */
#define T9REG		GREG_OFFSET(25)		/* caller saved 8 */
#define K0REG		GREG_OFFSET(26)		/* kernal reserved 0 */
#define K1REG		GREG_OFFSET(27)		/* kernal reserved 1 */
#define GPREG		GREG_OFFSET(28)		/* global pointer */
#define SPREG		GREG_OFFSET(29)		/* stack pointer */
#define S8REG		GREG_OFFSET(30)		/* callee saved 8 */
#define RAREG		GREG_OFFSET(31)		/* return address */

#ifdef __cplusplus
}
#endif

#endif /* __INCregsMipsh */
