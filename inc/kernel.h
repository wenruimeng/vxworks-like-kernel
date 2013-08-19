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

#ifndef __KERNEL_H_
#define __KERNEL_H_

#if KERNEL
#include "kernel_priv.h"
#else


typedef int CLASS_ID;
typedef int PART_ID;
extern CLASS_ID classClassId;


extern STATUS 	classLibInit (void);
extern STATUS 	classMemPartIdSet (CLASS_ID classId, PART_ID memPartId);
extern void 	classShowInit (void);
extern STATUS 	classShow (CLASS_ID classId, int level);


extern CLASS_ID memPartClassId;
extern PART_ID memSysPartId;

extern STATUS memPartLibInit(char* pPool, unsigned poolSize);
extern void memPartInit(PART_ID partId, char* pPool, unsigned poolSize);
extern void* memPartAlloc(FAST PART_ID partId, unsigned bytes);
extern STATUS memPartFree(PART_ID partId, char* pBlock);
extern PART_ID memPartCreate(char* pPool, unsigned poolSize);
extern STATUS memPartDestroy(PART_ID partId);
extern void memAddToPool(FAST char *pPool, FAST unsigned poolSize);

extern void* malloc(unsigned bytes);
extern void free(void* p);

/*#define KERNEL_TCB_LOCK_CNT 0x50
#define KERNEL_TCB_SAFE_CNT 0x5C*/
#include "task.h"
extern STATUS taskLibInit();
extern int taskSpawn
    (
    char          *name, 
    int           priority,
    int           options,
    int           stackSize,
    FUNCTION      entryPt,
    int           arg1,
    int           arg2,
    int           arg3,
    int           arg4,
    int           arg5
    );
extern int taskCreate(char* name
	, int priority
	, int options
	, int stacksize
	, FUNCTION entry
	, int arg1
	, int arg2
	, int arg3
	, int arg4
	, int arg5);
extern STATUS taskInit(FAST int tid
	, char* name
	, int priority
	, int options
	, char* pStackBase
	, int stackSize
	, FUNCTION entry
	, int arg1
	, int arg2
	, int arg3
	, int arg4
	, int arg5);
extern STATUS taskActivate(int tid);
extern STATUS taskResume(int tid);
extern STATUS taskDestroy(int  tid, BOOL dealloc, int  timeout, BOOL forceDestroy);
extern STATUS taskDelay(int ticks);
extern void taskRtnValueSet(int tid, int returnValue);
extern void* taskStackAlloc(int tid, unsigned bytes);
extern void exit(int code);
extern STATUS taskUnlock (void);
extern STATUS taskLock(void);
extern STATUS taskSafe (void);
extern STATUS taskUnsafe (void);
extern int intLock();
extern void intUnlock(int);
int taskIdSelf ();

typedef int QUEUE_ID;
extern QUEUE_ID qFifoQueueId;
extern QUEUE_ID qPriListId;
#define Q_FIFO 			qFifoQueueId
#define Q_PRI_LIST 		qPriListId

typedef int SEM_ID;
extern CLASS_ID semClassId;
#include "sem.h"
extern STATUS semLibInit();
extern STATUS semGive(SEM_ID semId);
extern STATUS semTake(SEM_ID semId, int timeout);
extern STATUS semFlush(SEM_ID semId);
extern STATUS semDelete(SEM_ID semId);
extern STATUS semTerminal(SEM_ID semId);
extern STATUS semDestroy(SEM_ID semId, BOOL dealloc);
extern STATUS semBLibInit();
extern SEM_ID semBCreate(int options, SEM_B_STATE initialState);
extern STATUS semMLibInit (void);
extern SEM_ID semMCreate(int options);
extern STATUS semCLibInit (void);
extern SEM_ID semCCreate(int options, int initialCount);

typedef int MSG_Q_ID;	/* message queue ID */
extern CLASS_ID msgQClassId;
#include "msgq.h"
extern MSG_Q_ID msgQCreate(int maxMsgs, int maxMsgLength, int options);
extern STATUS msgQLibInit (void);
extern STATUS msgQSend(FAST MSG_Q_ID msgQId, char* buffer, FAST UINT nBytes, int timeout, int priority);
extern int msgQReceive(FAST MSG_Q_ID msgQId, char* buffer, UINT maxNBytes, int timeout);
extern STATUS msgQDelete(MSG_Q_ID msgQId);

extern void kernelPreInit();
extern void kernelInit(FUNCTION rootRtn
	, unsigned rootMemSize
	, char* pMemPoolStart
	, char* pMemPoolEnd
	, unsigned intStackSize);


typedef void (*VOIDFUNCPTR)();
extern STATUS excVecInit();
extern STATUS intConnect(VOIDFUNCPTR* vector, VOIDFUNCPTR* routine, int parameter);
extern void intVecBaseSet(FUNCTION *baseAddr);
extern FUNCTION* intVecBaseGet();
extern FUNCTION* intVecGet(FUNCTION* vector);
extern void intVecSet(FUNCTION* vector, FUNCTION function);

void tickAnnounce();
void tickSet(ULONG ticks);
ULONG tickGet();

#endif

#endif

