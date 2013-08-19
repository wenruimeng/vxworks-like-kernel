/*! Janson(http://www.gandancing.com) 2011-2012
 *
 *\file
 *\brief		
 *\details	
 *
 *\author	Janson
 *\version	
 *\date		04Jan12
 *
 *\warning	
 *
 *\history \arg	30Jan12, Janson, Create the file
 *	modify from VxWorks source
 *  Legal Declaration: it is for studying VxWorks only.
 */
#include "archdefs.h"
#include "sysdefs.h"
#include "mips.h"
#include "taskMipsLib.h"
#include "esfMips.h"


	.text
	.set reorder

	.global kernelExit
	.global kernelTaskEntry
	.global intLock
	.global intUnlock

	.extern kernelState
	.extern taskIdCurrent
	.extern readyQHead
	.extern kernelIsIdle
	.extern taskSrDefault
	.extern intCnt
	.extern workQIsEmpty
	.extern areWeNested
	.extern workQDoWork

kernelExitIntWork:
	SETFRAME(kernelExitIntWork,0)
	subu	sp, FRAMESZ(kernelExitIntWork)	/* need some stack */
	SW	ra, FRAMERA(kernelExitIntWork)(sp) 	/* save ra */
	NOPS
	mtc0    t2, C0_SR        				/* UNLOCK INTS */
	jal	workQDoWork							/* empty the work queue */
	LW	ra, FRAMERA(kernelExitIntWork)(sp) 	/* restore ra */
	addu	sp, FRAMESZ(kernelExitIntWork)	/* restore stack */
	
	.ent kernelExitInt
kernelExitInt:
	/* lock int */
	mfc0 t2, C0_SR
	li t1, ~M_StatusIE
	and t1, t1, t2
	mtc0 t1, C0_SR
	NOPS
	lw t0, workQIsEmpty
	beq zero, t0, kernelExitIntWork
	SSNOP
	/* TODO?
	TASK-->--event-->--ISR-->--kernelExit-->
	--kernelExitInt-->--intExit-->(END ISR)-->reschule-->--TASK
	*/
noInst1:
	sw zero, kernelState
	move	v0, zero
	mtc0    t2, C0_SR
	j       ra
	.end kernelExitInt

checkTaskReady:
	lw t1, KERNEL_TCB_STATUS(t0)
	bne zero, t1, saveTaskContext

checkWorkQ:
	NOPS
	mfc0 t2, C0_SR
	li t1, ~M_StatusIE
	and t1, t1, t2
	mtc0 t1, C0_SR
	NOPS
	lw t0, workQIsEmpty
	beq zero, t0, doWorkPreSave
	SSNOP
	
noinst4:
	sw zero, kernelState
	mtc0 t2, C0_SR
	move v0, zero
	j ra

/* kernelExit未保存寄存器之前，需要先为workQDoWork的执行保存数据 */
/* intExit已将所有寄存器保存，所以没有保存ra、sp的必要，没有执行这步 */
doWorkPreSave:
	mtc0	t2, C0_SR					/* UNLOCK INTERRUPTS */
	SETFRAME(doWorkPreSave,0)
	subu	sp, FRAMESZ(doWorkPreSave) 	/* temp stack */
	SW	ra, FRAMERA(doWorkPreSave)(sp) 	/* preserve ra */
	jal 	workQDoWork 				/* empty the work queue */
	LW	ra, FRAMERA(doWorkPreSave)(sp)	/* restore ra */
	addu	sp, FRAMESZ(doWorkPreSave)	/* restore stack */
	b	checkTaskSwitch 				/* back up to test if tasks switched */


	.ent kernelExit
kernelExit:
	lw t0, intCnt
	bne zero, t0, kernelExitInt

checkTaskSwitch:
	lw t0, taskIdCurrent
	lw t1, readyQHead
	beq t0, t1, checkWorkQ

	lw t1, KERNEL_TCB_LOCK_CNT(t0)
	bne zero, t1, checkTaskReady
	
saveTaskContext:
	lw t1, errno
	sw t1, KERNEL_TCB_ERRNO(t0)
	sw ra, KERNEL_TCB_PC(t0)
	NOPS
	mfc0 t1, C0_SR
	sw sp, KERNEL_TCB_SP(t0)
	sw t1, KERNEL_TCB_SR(t0)
	sw zero, KERNEL_TCB_V0(t0)
	mflo t2
	sw t2, KERNEL_TCB_LO(t0)
	mfhi t2
	sw t2, KERNEL_TCB_HI(t0)
	sw s0, KERNEL_TCB_S0(t0)
	sw s1, KERNEL_TCB_S1(t0)
	sw s2, KERNEL_TCB_S2(t0)
	sw s3, KERNEL_TCB_S3(t0)
	sw s4, KERNEL_TCB_S4(t0)
	sw s5, KERNEL_TCB_S5(t0)
	sw s6, KERNEL_TCB_S6(t0)
	sw s7, KERNEL_TCB_S7(t0)
	sw s8, KERNEL_TCB_S8(t0)
	j reschedule

/*checkTaskReady:
	lw t1, KERNEL_TCB_STATUS(t0)
	bne zero, t1, saveTaskContext*/
	.end kernelExit
	
	.ent reschedule
reschedule:
	lw t1, readyQHead
	beq zero, t1, idle
	
switchTasks:
	sw t1, taskIdCurrent

dispatch:
	lw t1, taskIdCurrent
	lw t0, KERNEL_TCB_ERRNO(t1)
	sw t0, errno

	/* lock int */
	mfc0 t0, C0_SR
	li t2, ~M_StatusIE
	and t2, t2, t0
	mtc0 t2, C0_SR		
	NOPS

	lw k0, workQIsEmpty
	lw k1, KERNEL_TCB_SR(t1)
	beq zero, k0, doWorkUnlock
	/*SSNOP
	j noInst3*/
	
/*checkWorkQ:
	NOPS
	mfc0 t2, C0_SR
	li t1, ~M_StatusIE
	and t1, t1, t2
	mtc0 t1, C0_SR
	NOPS
	lw t0, workQIsEmpty
	beq zero, t0, doWorkPreSave
	SSNOP
	j noinst4*/
	
	.set reorder
noInst3:
	lw sp, KERNEL_TCB_SP(t1)
	lw a0, KERNEL_TCB_A0(t1)
	lw a1, KERNEL_TCB_A1(t1)
	lw a2, KERNEL_TCB_A2(t1)
	lw a3, KERNEL_TCB_A3(t1)
	lw v0, KERNEL_TCB_V0(t1)
	lw v1, KERNEL_TCB_V1(t1)
	lw t2, KERNEL_TCB_T2(t1)
	lw t2, KERNEL_TCB_LO(t1)
	mtlo t2
	lw t2, KERNEL_TCB_HI(t1)
	mthi t2
	lw t3, KERNEL_TCB_T3(t1)
	lw t4, KERNEL_TCB_T4(t1)
	lw t5, KERNEL_TCB_T5(t1)
	lw t6, KERNEL_TCB_T6(t1)
	lw t7, KERNEL_TCB_T7(t1)
	lw s0, KERNEL_TCB_S0(t1)
	lw s1, KERNEL_TCB_S1(t1)
	lw s2, KERNEL_TCB_S2(t1)
	lw s3, KERNEL_TCB_S3(t1)
	lw s4, KERNEL_TCB_S4(t1)
	lw s5, KERNEL_TCB_S5(t1)
	lw s6, KERNEL_TCB_S6(t1)
	lw s7, KERNEL_TCB_S7(t1)
	lw t8, KERNEL_TCB_T8(t1)
	lw t9, KERNEL_TCB_T9(t1)
	lw s8, KERNEL_TCB_S8(t1)
	lw ra, KERNEL_TCB_RA(t1)

	.set noreorder

	sw zero, kernelState
	lw k0, KERNEL_TCB_PC(t1)
	.set noat 
	lw AT, KERNEL_TCB_AT(t1)
	lw t2, KERNEL_TCB_T2(t1)
	lw t0, KERNEL_TCB_T0(t1)
	lw t1, KERNEL_TCB_T1(t1)
	mtc0 k0, C0_EPC
	/* why? */
	ori k1, M_StatusEXL
	mtc0 k1, C0_SR
	NOPS
ct:	eret
	.set at
	.set reorder

/*noinst4:
 	sw zero, kernelState
	mtc0 t2, C0_SR
	move v0, zero
	j ra
	
doWorkPreSave:
	NOPS
	mtc0 t2, C0_SR*/
	.end reschedule

	.ent idle
idle:
	lw	t2, taskSrDefault		/* load default SR value */
	li	t0, 1					/* load idle flag to reg */
	NOPS
	mtc0	t2, C0_SR			/* UNLOCK INTERRUPTS */
	sw	t0, kernelIsIdle		/* set idle flag for spyLib */
idleLoop:
	lw	t1, workQIsEmpty		/* if work queue is still empty */
	bne zero, t1, idleLoop		/* keep hanging around */
	sw	zero, kernelIsIdle		/* unset idle flag for spyLib */
	j	doWork					/* go do the work */
	.end idle
	
doWorkUnlock:
	NOPS
	mtc0	t0, C0_SR		/* UNLOCK interrupts */
doWork:
	jal workQDoWork 		/* empty the work queue */
	lw	t0, taskIdCurrent	/* put taskIdCurrent into t0 */
	lw	t1, readyQHead		/* get highest task to t1 */
	beq zero, t1, idle		/* nobody is ready so spin */
	beq t0, t1, dispatch	/* if the same dispatch */
	j	switchTasks 		/* not same, do switch */
	

	.ent kernelTaskEntry
kernelTaskEntry:
	lw t0, taskIdCurrent
	lw t1, KERNEL_TCB_ENTRY(t0)
	jal t1
	move a0, v0
	jal exit
	.end kernelTaskEntry

	.globl intExit
	.ent intExit
intExit:
	SETFRAME(intExit,3)
	subu sp, FRAMESZ(intExit)		/* get some work space */
	SW t0, FRAMER0(intExit)(sp) 	/* store registers which are used */
	SW t1, FRAMER1(intExit)(sp)
	SW t2, FRAMER2(intExit)(sp)
	
noIntExit:
	lw t0, intCnt	
	subu t0, 1
	sw t0, intCnt			/* decrement intCnt */
	lw t1, areWeNested 		/* load nested boolean */
	subu t1, 1				/* decrement */
	sw t1, areWeNested 		/* store nested boolean */
	lw t2, kernelState
	bne t2, zero, intRte
	bne t1, zero, intRte
	lw t0, taskIdCurrent
	lw t2, readyQHead
	beq t0, t2, intRte

	lw	t1, KERNEL_TCB_LOCK_CNT(t0)	/* is task preemption allowed */
	beq zero, t1, saveIntContext	/* if yes, then save context */
	lw	t1, KERNEL_TCB_STATUS(t0) 	/* is task ready to run */
	bne zero, t1, saveIntContext	/* if no, then save context */

intRte:
	LW	t0, FRAMER0(intExit)(sp)		/* restore registers used locally */
	LW	t1, FRAMER1(intExit)(sp)
	LW	t2, FRAMER2(intExit)(sp)
	addu	sp, FRAMESZ(intExit)		/* recover stack work space */

	.set	noreorder
	.set	noat
	lw	k0, E_STK_EPC(sp)	/* get the exception program counter */
	LW	AT, E_STK_AT(sp)	/* restore AT reg		*/
	LW	sp, E_STK_SP(sp)	/* restore the task stack pointer,
								no need to pop temp stack now */
	mtc0	k0,C0_EPC		/* return to previous context */
	NOPS
	eret					/* RESTORE INTERRUPTS */
	
	.set	at
	.set	reorder
saveIntContext:
	/* interrupts are still locked out */
	li	t1, 1							/* kernelState = TRUE; */
	sw	t1, kernelState 		
	lw	k0, taskIdCurrent				/* tcb to be fixed up */
	lw	t0, errno
	sw	t0, KERNEL_TCB_ERRNO(k0)		/* save errno */
	LW	t0, FRAMER0(intExit)(sp)		/* restore working registers */
	LW	t1, FRAMER1(intExit)(sp)			
	LW	t2, FRAMER2(intExit)(sp)
	SW	t0, KERNEL_TCB_T0(k0) 			/* and save in TCB */
	SW	t1, KERNEL_TCB_T1(k0) 		
	SW	t2, KERNEL_TCB_T2(k0) 		
	move	t0, k0						/* use t0 as taskIdCurrent */
	addu	sp, FRAMESZ(intExit)		/* recover stack work space */
	lw	t2, E_STK_EPC(sp)				/* get the exception PC */
	LW	t1, E_STK_SP(sp)				/* get the process SP */
	sw	t2, KERNEL_TCB_PC(t0) 			/* store exception PC in TCB */
	SW	t1, KERNEL_TCB_SP(t0) 			/* store regs in tcb */
	.set	noat
	LW	AT, E_STK_AT(sp)
	SW	AT, KERNEL_TCB_AT(t0) 
	.set	at
	mfc0	k1, C0_SR
	NOPS
	and k1,~M_StatusEXL
	sw	k1, KERNEL_TCB_SR(t0) 			/* store sr in TCB */
	move	sp, t1						/* work off task stack */
	mtc0	k1, C0_SR					/* UNLOCK INTERRUPTS */
/*
*	A window of vulnerabilty opens up here on the R3000.  We need to
*	have epc,sp,and AT restored and have begun working off the task stack 
*	by now.  This is because intCnt == 0, and if we get interrupted
*	excIntStub will reset and muck the stack.
*/
										/* store registers starting */
	SW	t3, KERNEL_TCB_T3(t0) 			/*	  with remaining  temp	*/
	SW	t4, KERNEL_TCB_T4(t0) 			/*	  registers so work 	*/
	SW	t5, KERNEL_TCB_T5(t0) 			/*	  are available 		*/
	SW	t6, KERNEL_TCB_T6(t0)
	SW	t7, KERNEL_TCB_T7(t0)
	SW	a0, KERNEL_TCB_A0(t0) 			/* save remaining registers  */
	SW	a1, KERNEL_TCB_A1(t0) 			/*	  in TCB				 */
	SW	a2, KERNEL_TCB_A2(t0)
	SW	a3, KERNEL_TCB_A3(t0)
	SW	v0, KERNEL_TCB_V0(t0)
	SW	v1, KERNEL_TCB_V1(t0)
	mflo	v0
	mfhi	v1
	SW	v0, KERNEL_TCB_LO(t0)
	SW	v1, KERNEL_TCB_HI(t0)
0:
	SW	s0, KERNEL_TCB_S0(t0)
	SW	s1, KERNEL_TCB_S1(t0)
	SW	s2, KERNEL_TCB_S2(t0)
	SW	s3, KERNEL_TCB_S3(t0)
	SW	s4, KERNEL_TCB_S4(t0)
	SW	s5, KERNEL_TCB_S5(t0)
	SW	s6, KERNEL_TCB_S6(t0)
	SW	s7, KERNEL_TCB_S7(t0)
	SW	t8, KERNEL_TCB_T8(t0)
	SW	t9, KERNEL_TCB_T9(t0)
	SW	s8, KERNEL_TCB_S8(t0)
	SW	ra, KERNEL_TCB_RA(t0) 	
	j	reschedule			/* goto rescheduler */

	.end intExit


	.ent intLock
intLock:
	mfc0 v0, C0_SR
	NOPS
	li t1, ~M_StatusIE
	and t1, t1, v0
	mtc0 t1, C0_SR
	NOPS
	j ra
	.end intLock

	.ent intUnlock
intUnlock:
	mtc0 a0, C0_SR
	NOPS
	j ra
	.end intUnlock

	.globl kLogChar
	.ent kLogChar
kLogChar:
1:	li t0, 0xb40002f8
	lb t1, 5(t0)
	andi t1, t1, 0x20
	beqz t1, 1b
	sb a0, 0(t0)
	j ra
	.end kLogChar
