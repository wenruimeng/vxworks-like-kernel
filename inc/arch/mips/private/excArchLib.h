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
#ifndef __EXC_ARCH_LIB_H_
#define __EXC_ARCH_LIB_H_

#define E_VEC (KSEG0BASE+0x180)

typedef void (*VOIDFUNCPTR)();

STATUS excVecInit();

STATUS intConnect(VOIDFUNCPTR* vector, VOIDFUNCPTR* routine, int parameter);

void intVecBaseSet(FUNCTION *baseAddr);

FUNCTION* intVecBaseGet();

FUNCTION* intVecGet(FUNCTION* vector);

void intVecSet(FUNCTION* vector, FUNCTION function);

int intRegsLock(REG_SET* pRegs);

void intRegsUnlock(REG_SET* pRegs, int oldSr);

#endif
