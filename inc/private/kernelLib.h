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

#ifndef __KERNEL_LIB_H_
#define __KERNEL_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

void kernelPreInit();

void kernelInit(FUNCTION rootRtn
	, unsigned rootMemSize
	, char* pMemPoolStart
	, char* pMemPoolEnd
	, unsigned intStackSize);

void kernelSpawn(FAST RAIN_TCB* pTcb);
void kernelResume(FAST RAIN_TCB* pTcb);
void kernelReadyQPut(RAIN_TCB* pTcb);
void kernelReadQRemove(Q_HEAD* pQHead, int timeout);
void kernelSemDelete(FAST SEM_ID semId);
void kernelPendQFlush(Q_HEAD* pQHead);
void kernelPendQGet(Q_HEAD* pQHead);
STATUS kernelPendQPut(FAST Q_HEAD* pQHead, FAST int timeout);
void kernelPendQTerminate(Q_HEAD* pQHead);
STATUS kernelPendQRemove(RAIN_TCB *pTcb);
STATUS kernelDelete(FAST RAIN_TCB *pTcb);
void kernelSuspend(FAST RAIN_TCB *pTcb);
void kernelTickAnnounce();
void kernelDelay(FAST int timeout);
void kernelPrioritySet(FAST RAIN_TCB* pTcb, FAST UINT priority);
void kernelSemDelete(FAST SEM_ID semId);

#ifdef __cplusplus
}
#endif


#endif
