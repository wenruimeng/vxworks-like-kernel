#include "archdefs.h"
#include "sysdefs.h"
#include "mips.h"
#include "taskMipsLib.h"
#include "esfMips.h"

	.globl excNormVec
	.globl excNormVecSize
	.globl excStubInt
	.globl excStub

	.extern sysHashOrder
	.extern intCnt
	.extern areWeNested
	.extern kernelIntStackBase
	.extern errno
	.extern intPrioTable
	.extern intExit

	.globl excNormVec
	.ent excNormVec
	.set noreorder
	.set noat
excNormVec:
	mfc0 k0, C0_CAUSE
	NOPS
	lw k1, areWeNested
	andi k0, M_Cause_ExcMask
	bne k0, zero, 1f
	SSNOP
	la k0, excIntStub
	j k0
	nop
1: la k0, excStub
	j k0
	SSNOP
excNormVecEnd:
	.set at
	.set reorder
	.end excNormVec

	.data
	.align 4
excNormVecSize:
	.word excNormVecEnd-excNormVec
	.text
	
	.globl excIntStub
	.ent excIntStub
excIntStub:
	.set noat
	bne k1, zero, nested
	lw k0, kernelIntStackBase
	subu k0, ESTKSIZE
	sw sp, E_STK_SP(k0)
	move sp, k0
	b save_critical
nested:
	sw sp, E_STK_SP-ESTKSIZE(sp)
	subu sp, ESTKSIZE
save_critical:
	sw AT, E_STK_AT(sp)
	.set at
	sw v0, E_STK_V0(sp)
	addu k1, 1
	sw k1, areWeNested
	lw k1, intCnt
	addu k1, 1
	sw k1, intCnt

intStateSave:
	SW	zero,E_STK_ZERO(sp)	/* init zero reg storage	 */
    SW      v0,E_STK_V0(sp)         /* save func return 0            */
    SW      v1,E_STK_V1(sp)         /* save func return 1            */
    SW      a0,E_STK_A0(sp)         /* save passed param 0           */
    SW      a1,E_STK_A1(sp)         /* save passed param 1           */
    SW      a2,E_STK_A2(sp)         /* save passed param 2           */
    SW      a3,E_STK_A3(sp)         /* save passed param 3           */
    SW      t0,E_STK_T0(sp)         /* save temp reg 0               */
    SW      t1,E_STK_T1(sp)         /* save temp reg 1               */
    SW      t2,E_STK_T2(sp)         /* save temp reg 2               */
    SW      t3,E_STK_T3(sp)         /* save temp reg 3               */
    SW      t4,E_STK_T4(sp)         /* save temp reg 4               */
    SW      t5,E_STK_T5(sp)         /* save temp reg 5               */
    SW      t6,E_STK_T6(sp)         /* save temp reg 6               */
    SW      t7,E_STK_T7(sp)         /* save temp reg 7               */
    SW      t8,E_STK_T8(sp)         /* save temp reg 8               */
    SW      t9,E_STK_T9(sp)         /* save temp reg 9               */
    SW      ra,E_STK_RA(sp)         /* save return address           */
    mflo    t2                      /* read entry lo reg             */
    mfhi    t3                      /* read entry hi reg             */
    SW      t2,E_STK_LO(sp)         /* save entry lo reg             */
    SW      t3,E_STK_HI(sp)         /* save entry hi reg             */

	.set	noreorder
	mfc0	k1, C0_SR		/* read status register          */
	mfc0	k0, C0_EPC		/* read exception pc             */
	mfc0	v0, C0_CAUSE		/* read cause register           */
	NOPS
	sw	k1, E_STK_SR(sp)	/* save status on stack          */
	sw	k0, E_STK_EPC(sp)	/* save EPC on stack             */
	sw	v0, E_STK_CAUSE(sp)	/* save cause on stack           */
	.set	reorder

/*	lw t0, evtAction
	beqz t0, noIntEnt*/

noIntEnt:
	lw k0, errno
	sw k0, E_ERRNO(sp)
	
	/*lw t1, intStubSelect
	bnez t1, excIntStubExtended*/

excIntStubNormal:
	andi t2, v0, M_Status_IMASK0
	and v0, k1, t2
	lw t1, sysHashOrder
	beqz v0, restore
	srl t2, v0, SR_IMASKSHIFT
	addu t2, t2, t1
	lbu t1, 0(t2)
	sll t1, 4
	.set noat
	lui AT, %hi(intPrioTable+8)
	addu AT, t1
	lw t2, %lo(intPrioTable+8)(AT)
	.set at
	or v0, v0, t2
	not v0
	and k1, v0, k1
	
excIntStubCommonExit:
	.set noat
	mtc0 k1, C0_SR
	NOPS
	li AT, ~(M_StatusEXL)
	and k1, AT
	/*or k1, M_StatusEXL*/
	mtc0 k1, C0_SR
	NOPS

	lui AT, %hi(intPrioTable+4)
	addu AT, t1
	lw v0, %lo(intPrioTable+4)(AT)

	lui AT, %hi(intPrioTable+12)
	addu AT, t1
	lw t1, %lo(intPrioTable+12)(AT)
	.set at

	beq t1, zero, call
	move a0, t1

	jal v0

call: move a0, v0
	move a1, sp
	sll v0, 2
	la t0, excBsrTbl
	addu v0, t0
	lw v1, (v0)
	jal v1

restore:
	lw t2, E_ERRNO(sp)
	sw t2, errno
	LW	v1,E_STK_V1(sp)		/* restore func ret 0	         */
	LW	a0,E_STK_A0(sp)		/* restore passed param 0	 */
	LW	a1,E_STK_A1(sp)		/* restore passed param 1	 */
	LW	a2,E_STK_A2(sp)		/* restore passed param 2	 */
	LW	a3,E_STK_A3(sp)		/* restore passed param 3	 */
	LW	t0,E_STK_T0(sp)		/* restore temp reg 0            */
	LW	t4,E_STK_T4(sp)		/* restore temp reg 4            */
	LW	t5,E_STK_T5(sp)		/* restore temp reg 5            */
	LW	t6,E_STK_T6(sp)		/* restore temp reg 6            */
	LW	t7,E_STK_T7(sp)		/* restore temp reg 7            */
	LW	t8,E_STK_T8(sp)		/* restore temp reg 8            */
	LW	ra,E_STK_RA(sp)		/* restore return addr           */
	LW	t9,E_STK_LO(sp)		/* grab entry hi reg             */
	mtlo	t9			/* restore entry hi reg          */
	LW	t9,E_STK_HI(sp)		/* grab entry lo reg             */
	mthi	t9			/* restore entry hi reg          */
	LW	t9,E_STK_T9(sp)		/* restore temp reg 9            */

restoreVolatile:
	lw	t1, E_STK_SR(sp) 	/* read old SR				 */
	mtc0	t1,C0_SR		/* put on processor 		 */
	LW	v0,E_STK_V0(sp) 	/* restore func ret 0			 */
	LW	t3,E_STK_T3(sp) 	/* restore temp reg 3			 */
	LW	t2,E_STK_T2(sp) 	/* restore temp reg 2			 */
	LW	t1,E_STK_T1(sp) 	/* restore temp reg 1			 */

/*  */
	j	intExit 		/* exit kernel, exception frame  */
						/* on interrupt stack */
	SSNOP
	.end excIntStub

	.globl	excStub
	.ent	excStub
excStub:
	/* panic hear */
	mfc0 v0, C0_SR
	NOPS
	li t1, ~M_StatusIE
	and t1, t1, v0
	mtc0 t1, C0_SR
	NOPS
1:	b 1b
	.end excStub

	.globl intSRGet
	.ent intSRGet
intSRGet:
	mfc0 v0, C0_SR
	NOPS
	j ra
	.end intSRGet

	.globl intSRSet
	.ent intSRSet
intSRSet:
	mtc0 a0, C0_SR
	NOPS
	j ra
	.end intSRSet

/*
	.globl timerInit
	.ent timerInit
timerInit:
	li a0, 666666
	mtc0 a0, C0_COMPARE
	NOPS
	li a0, 0
	mtc0 a0, C0_COUNT
	NOPS
	j ra
	.end timerInit
*/

