/* excMipsLib.h - header for exception library */

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
02o,16jul01,ros  add CofE comment
02n,19jan99,dra  added CW4000, CW4011, VR4100, VR5000 and VR5400 support.
02m,18dec96,tam  added defines for break instruction.
02l,15jul96,cah  added support for R4650
02k,30may96,kkk  added exception codes for R3000/R4000.
02j,15oct93,cd   added R4000 support.
02i,13aug93,yao  added exception code.  added ifndef _ASMLANGUAGE.
02h,08jul93,yao  added one more instruction to exception vector for saving
		 cause register (SPR #2362).  updated document for the vector
		 structure.
02g,18aug93,jmm  added external declaration for excExcepHook
02f,22sep92,rrr  added support for c++
02e,03jun92,ajm  updated file name referenced to match real name
02d,26may92,rrr  the tree shuffle
02c,04oct91,rrr  passed through the ansification filter
		  -changed copyright notice
02b,20sep90,ajm  added floating point status reg, and error address and
		  and id regs
02a,28may90,ajm  ported to MIPS r3000
01a,28may88,dnw  written
*/

#ifndef __INCexcMipsLibh
#define __INCexcMipsLibh

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASSEMBLER_
/* variable declarations */

/*extern FUNCPTR  excExcepHook;*/   /* add'l rtn to call when exceptions occur */

/* generic exception information -
 * kept in the tcb of tasks that have caused exceptions */

typedef struct
    {
    USHORT valid;		/* indicators that following fields are valid */
    USHORT vecNum;		/* vector number */
    USHORT eid;			/* error id number */
    USHORT pad;			/* word alignment pad */
    INSTR *epc;			/* exception program counter */
    ULONG badVa;		/* access address */
    ULONG statusReg;		/* status register */
    ULONG causeReg;		/* cause register */
    ULONG fpcsr;		/* float status register */
    ULONG ear;			/* error address register */
    } EXC_INFO;
#endif

/* exception info valid bits */

#define EXC_VEC_NUM		0x01	/* vector number valid */
#define EXC_EPC			0x02	/* pc valid */
#define EXC_ACCESS_ADDR		0x04	/* access address valid */
#define EXC_STATUS_REG		0x08	/* status register valid */
#define EXC_CAUSE_REG		0x10	/* cause register valid */
#define EXC_FP_STATUS_REG	0x20	/* fp status register valid */
#define EXC_ERROR_ADDR		0x40	/* error address valid */

/* exception code */

#define EXC_CODE_INT	0		/* externel interrupt */
#define EXC_CODE_TLBM	1		/* TLB modification exception */

#define EXC_CODE_TLBL	2		/* TLB miss exception (load or 
					 * instruction fetch) */
#define EXC_CODE_IBND   2               /* R4650: IBound exception */
#define EXC_CODE_TLBS	3		/* TLB miss exception (store) */
#define EXC_CODE_DBND   3               /* R4650: DBound exception */

#define EXC_CODE_ADEL	4		/* address error exception (load/
					 * instruction fetch ) */
#define EXC_CODE_ADES	5		/* address error exception (store) */
#define EXC_CODE_BEI 	6		/* bus error(for instruction fetch) */
#define EXC_CODE_BED	7		/* bus error (data load or store) */
#define EXC_CODE_SYS	8		/* system call exception */
#define EXC_CODE_BP 	9		/* breakpoint exception */
#define EXC_CODE_RI	10		/* reserved instruction exception */
#define EXC_CODE_FPU	11		/* coprocessor unusable exception */
#define EXC_CODE_OVF	12		/* arithmetic overflow exception */

#define EXC_CODE_TRAP	13		/* trap exception */
#define EXC_CODE_VCEI	14		/* instr virtual coherency exception */
#define EXC_CODE_FPE	15		/* floating point exception */
#define EXC_CODE_WATCH	23		/* watchpoint exception */
#define EXC_CODE_VCED	31		/* data virtual coherency exception */

#define EXC_CODE_FPE_INVALID	55	/* invalid fpe operation */
#define EXC_CODE_FPE_DIV0	56	/* divide by zero */
#define EXC_CODE_FPE_OVERFLOW	57	/* overflow */
#define EXC_CODE_FPE_UNDERFLOW	58	/* underflow */
#define EXC_CODE_FPE_INEXACT	59	/* inexact result */

/* break instruction info */

#define EXC_BRK_CODE_MSK    0x03ffffc0	/* break code field mask */
#define EXC_BRK_DIV_BY_ZERO 0x1c00	/* integer div. by zero */
#define EXC_BRK_INT_OVF     0x1800	/* integer overflow */


#ifdef __cplusplus
}
#endif

#endif /* __INCexcMipsLibh */
