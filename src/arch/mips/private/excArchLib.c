#include "includes.h"
#include "esfMips.h"
#include "cache.h"

#define	HI_WORD(w)		(((U32)(w) >> 16) & 0xffff)
#define	LO_WORD(w)		(((U32)(w)) & 0xffff)

int areWeNested = 0;

static void excIntHandle (int vecNum, ESFMIPS * pEsf);
static void excExcHandle (int vecNum, ESFMIPS * pEsf, REG_SET * pRegs);
static FUNCTION intHandleCreate(FUNCTION routine, int parameter);

extern ULONG intSRGet();
extern void intSRSet(ULONG status);

VOIDFUNCPTR excBsrTbl[] = 
    {
    excIntHandle, 	/* 0 - interrupt exception	*/
    excExcHandle, 	/* 1 - tlb mod exception	*/
    excExcHandle, 	/* 2 - tlb load exception	*/
    excExcHandle,	/* 3 - tlb store exception	*/
    excExcHandle, 	/* 4 - address load exception	*/
    excExcHandle, 	/* 5 - address store exception	*/
    excExcHandle, 	/* 6 - instr. bus error exception*/
    excExcHandle,	/* 7 - data bus error exception	*/
    excExcHandle, 	/* 8 - system call exception	*/
    excExcHandle, 	/* 9 - break point exception	*/
    excExcHandle, 	/* 10 - rsvd instruction exception*/
    excExcHandle,	/* 11 - coprocessor unusable excptn*/
    excExcHandle, 	/* 12 - overflow exception	*/
    excExcHandle,	/* 13 - trap exception 		*/
    excExcHandle, 	/* 14 - VCEI exception 		*/
    excExcHandle,	/* 15 - FPE exception 		*/
    excExcHandle,	/* 16 - reserved entry		*/
    excExcHandle,	/* 17 - reserved entry		*/
    excExcHandle,	/* 18 - reserved entry		*/
    excExcHandle,	/* 19 - reserved entry		*/
    excExcHandle,	/* 20 - reserved entry		*/
    excExcHandle,	/* 21 - reserved entry		*/
    excExcHandle,	/* 22 - reserved entry		*/
    excExcHandle,	/* 23 - watch exception		*/
    excExcHandle,	/* 24 - reserved entry		*/
    excExcHandle,	/* 25 - reserved entry		*/
    excExcHandle,	/* 26 - reserved entry		*/
    excExcHandle,	/* 27 - reserved entry		*/
    excExcHandle,	/* 28 - reserved entry		*/
    excExcHandle,	/* 29 - reserved entry		*/
    excExcHandle,	/* 30 - reserved entry		*/
    excExcHandle,	/* 31 - VCED exception		*/
    excIntHandle,	/* 32 - software trap 0 	*/
    excIntHandle,	/* 33 - software trap 1 	*/
    excIntHandle,	/* 34 - autovec VME irq7 interrupt 	*/
    excIntHandle,	/* 35 - autovec VME irq6 interrupt 	*/
    excIntHandle,	/* 36 - autovec VME irq5 interrupt 	*/
    excIntHandle,	/* 37 - autovec VME irq4 interrupt 	*/
    excIntHandle,	/* 38 - autovec VME irq3 interrupt	*/	
    excIntHandle,	/* 39 - autovec VME irq2 interrupt	*/
    excIntHandle,	/* 40 - autovec VME irq1 interrupt 	*/
    excIntHandle,	/* 41 - spare interrupt	*/
    excIntHandle,	/* 42 - uart 0 interrupt	*/
    excIntHandle,	/* 43 - uart 1 interrupt	*/
    excIntHandle,	/* 44 - msg pending interrupt	*/
    excIntHandle,	/* 45 - spare interrupt		*/
    excIntHandle,	/* 46 - spare interrupt		*/
    excIntHandle,	/* 47 - hw bp interrupt		*/
    excIntHandle,	/* 48 - spare interrupt		*/
    excIntHandle,	/* 49 - spare interrupt		*/
    excIntHandle,	/* 50 - timer 0 interrupt	*/
    excIntHandle,	/* 51 - timer 1 interrupt	*/
    excIntHandle,	/* 52 - spare exception		*/
    excIntHandle,	/* 53 - spare exception		*/
    excExcHandle,	/* 54 - unimplemented FPA oper	*/
    excExcHandle,	/* 55 - invalid FPA operation	*/
    excExcHandle,	/* 56 - FPA div by zero		*/
    excExcHandle,	/* 57 - FPA overflow exception	*/
    excExcHandle,	/* 58 - FPA underflow exception	*/
    excExcHandle,	/* 59 - FPA inexact operation	*/
    excIntHandle,	/* 60 - bus error interrupt	*/
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 70 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 80 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 90 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 100 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 110 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 120 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 130 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 140 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 150 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 160 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 170 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 180 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 190 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 200 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 210 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 220 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 230 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 240 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 250 */
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,
    excIntHandle,	/* 255 */
    };

static UINT intConnectCode []	=	/* intConnect stub */
    {
/*
*  0x0:		3c08xxxx	lui	t0,xxxx		* msh routine address
*  0x4:		3c04yyyy	lui	a0,yyyy		* msh parameter load
*  0x8:		3508zzzz	ori	t0,t0,zzzz	* lsh routine address
*  0xc:		01000008	jr	t0		* jump to routine
*  0x10:	3484pppp	ori	a0,a0,pppp	* lsh load in BD slot
*/
     0x3c080000,	/* msh routine address runtime load */
     0x3c040000,	/* msh parameter load runtime load */
     0x35080000,	/* lsh routine address runtime load */
     0x01000008,	/* jump to routine */
     0x34840000,	/* lsh load in BD slot runtime load */
    };

static void excIntHandle (int vecNum, ESFMIPS * pEsf)
{
	/* TODO do any error print hear */
}

static void excExcHandle (int vecNum, ESFMIPS * pEsf, REG_SET * pRegs)
{
	/* TODO do any error print hear */

}



STATUS excVecInit()
{
	extern int excNormVec[];
	extern int excNormVecSize;

	ULONG srValue;

	memcpy((void*)E_VEC, excNormVec, excNormVecSize);
	ICACHE_vInvalidateAddr(E_VEC, excNormVecSize);

	srValue = intSRGet();
    srValue &= ~M_StatusBEV;
    intSRSet(srValue);

    return (OK);
}

STATUS intConnect(VOIDFUNCPTR* vector, VOIDFUNCPTR* routine, int parameter)
{
	FUNCTION intDrvRtn = intHandleCreate((FUNCTION)routine, parameter);

	if(NULL == intDrvRtn)
	{
		return (ERROR);
	}

	intVecSet((FUNCTION*)vector, (FUNCTION)intDrvRtn);

	return (OK);
}

static FUNCTION intHandleCreate(FUNCTION routine, int parameter)
{
	FAST UINT* pCode;

	pCode = memPartAlloc(memSysPartId, sizeof(intConnectCode));
	if(NULL != pCode)
	{
		memcpy(pCode, intConnectCode, sizeof(intConnectCode));

		pCode[0] |= HI_WORD(routine);
		pCode[1] |= HI_WORD(parameter);
		pCode[2] |= LO_WORD(routine);
		pCode[4] |= LO_WORD(parameter);

		ICACHE_vInvalidateAddr(pCode, sizeof(intConnectCode));	
	}

	return ((FUNCTION)pCode);
}

void intVecBaseSet(FUNCTION *baseAddr)
{

}

FUNCTION* intVecBaseGet()
{
	return ((FUNCTION*)excBsrTbl);
}

FUNCTION* intVecGet(FUNCTION* vector)
{
	return (*(FUNCTION*)((int)(vector)+(int)intVecBaseGet()));
}

void intVecSet(FUNCTION* vector, FUNCTION function)
{
	FUNCTION* newVector;

	newVector = (FUNCTION*)((int)vector+(int)intVecBaseGet());

	*newVector = function;
}

int intRegsLock(REG_SET* pRegs)
{
	int oldSr = pRegs->sr;
	pRegs->sr &= ~M_StatusIE;
	return oldSr;
}

void intRegsUnlock(REG_SET* pRegs, int oldSr)
{
	pRegs = (pRegs->sr & ~M_StatusIE) | (oldSr & M_StatusIE);
}
