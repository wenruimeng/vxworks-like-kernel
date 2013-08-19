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
#ifndef __TASKLIB_H_
#define __TASKLIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "task.h"

#define TASK_LOCK()							     \
	do										 \
	{									 		\
		taskCurrent->lockCnt++;				\
	} while (0)

#define TASK_UNLOCK()	taskUnlock ()

#define TASK_SAFE()							 \
	do										 \
	{									 	\
		taskCurrent->safeCnt++;				\
	} while (0)
	
#define TASK_UNSAFE()	taskUnsafe ()


typedef struct _rainTcb
{
	Q_NODE qNode;			/* start from 0x0 */
	Q_NODE tickNode;		/* 0x10 */
	Q_NODE activeNode;		/* 0x20 */

	OBJ_CORE objCore;		/* 0x30 */
	char* name;				/* 0x34 */
	int options;			/* 0x38 */
	UINT status;			/* 0x3C */
	/* ERROR NUMBER hear 0x40 */
	UINT priority;			/* 0x44 */
	UINT priNormal;			/* 0x48 */
	UINT priMutexCnt;		/* 0x4C */
	struct _semaphore* pPriMutex;	/* 0x50 */

	volatile UINT lockCnt;			/* 0x54 */
	UINT tslice;			/* 0x58 */

	Q_HEAD* pPendQ;			/* 0x5C */

	volatile UINT safeCnt;			/* 0x60 */
	Q_HEAD safetyQHead;		/* 0x64 */

	FUNCTION entry;			/* 0x74 */

	char* pStackBase;		/* 0x78 */
	char* pStackLimit;		/* 0x7C */
	char* pStackEnd;		/* 0x80 */

	int errorStatus;		/* 0x84 */
	int exitCode;			/* 0x88 */

	UINT taskTicks;			/* 0x8C */
	UINT taskIncTicks;		/* 0x90 */

    int reserved1;			/* 0x94 */
    int reserved2;			/* 0x98 */
    int spare1;				/* 0x9C */
    int spare2;				/* 0xA0 */
    int spare3;				/* 0xA4 */
    int spare4;				/* 0xA8 */

	EXC_INFO excInfo;		/* 0xAC-4 */
    REG_SET regs;			/* 0xCC-4 */

	
}RAIN_TCB;

extern CLASS_ID taskClassId;
/*extern int taskIdCurrent;*/
extern BOOL kernelState;

extern Q_HEAD	  activeQHead;
extern Q_HEAD	  tickQHead;
extern Q_HEAD	  readyQHead;


#define taskCurrent					((RAIN_TCB*)taskIdCurrent)
/* Used TRUE or FALSE */
#define TASK_ID_VERIFY(tid)			(((RAIN_TCB*)NULL != (RAIN_TCB*)(tid)) && \
	(IS_CLASS(&((RAIN_TCB*)(tid))->objCore, taskClassId)))

STATUS taskLibInit();

int taskSpawn
    (
    char          *name,        /* name of new task (stored at pStackBase) */
    int           priority,     /* priority of new task */
    int           options,      /* task option word */
    int           stackSize,    /* size (bytes) of stack needed plus name */
    FUNCTION      entryPt,      /* entry point of new task */
    int           arg1,         /* 1st of 10 req'd task args to pass to func */
    int           arg2,
    int           arg3,
    int           arg4,
    int           arg5
    );


int taskCreate(char* name
	, int priority
	, int options
	, int stacksize
	, FUNCTION entry
	, int arg1
	, int arg2
	, int arg3
	, int arg4
	, int arg5);

STATUS taskInit(FAST RAIN_TCB* pTcb
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

STATUS taskActivate(int tid);

STATUS taskResume(int tid);

STATUS taskDestroy(int  tid, BOOL dealloc, int  timeout, BOOL forceDestroy);

STATUS taskDelay(int ticks);

void taskRtnValueSet(RAIN_TCB *pTcb, int returnValue);

void* taskStackAlloc(int tid, unsigned bytes);

RAIN_TCB* taskTcb(int tid);

int taskIdSelf ();
STATUS taskSuspend(int tid);


void exit(int code);

STATUS taskUnlock (void);
STATUS taskLock(void);
STATUS taskSafe (void);
STATUS taskUnsafe (void);


extern int intLock();
extern void intUnlock(int);
#ifdef __cplusplus
}
#endif

#endif

