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
#include "includes.h"

BOOL taskLibInstalled = FALSE;
OBJ_CLASS taskClass;
CLASS_ID taskClassId = &taskClass;
extern int errno;

/* it identify that kernel is sheduling, 
	only ISR can change the context */
/* mutex to enter kernel state */
BOOL kernelState = FALSE;

int taskIdCurrent = 0;
BOOL kernelIsIdle = FALSE;
UINT intCnt = 0;
UINT taskSrDefault = M_StatusCU0 | M_StatusCU1 | M_Status_IMASK0 | M_StatusIE;

extern STATUS kernelExit();
extern void kernelTaskEntry(RAIN_TCB* pTcb);

static void taskRegsInit(RAIN_TCB* pTcb, char* pStackBase);
static void taskArgsSet(RAIN_TCB* pTcb, char* pStackBase, int pArgs[]);

STATUS taskLibInit()
{
	if((!taskLibInstalled) && 
		(OK == (classInit(taskClassId, STACK_ROUND_UP(sizeof(RAIN_TCB)+16),
			OFFSET(RAIN_TCB, objCore), (FUNCTION)taskCreate
			, (FUNCTION)taskInit, (FUNCTION)taskDestroy))) )
	{
		taskLibInstalled = TRUE;
	}

	return ((taskLibInstalled)? OK : ERROR);
}

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
    )
{
		
    int tid = taskCreate(name, priority, options, stackSize, entryPt, arg1,
                         arg2, arg3, arg4, arg5);

    if (tid == (int)NULL)			/* create failed */
	return (ERROR);

    taskActivate (tid);				/* activate task */
	
    return (tid);				/* return task ID */
}


int taskCreate(char* name
	, int priority
	, int options
	, int stackSize
	, FUNCTION entry
	, int arg1
	, int arg2
	, int arg3
	, int arg4
	, int arg5)
{
	char* pTaskMem;
	char* pStackBase;
	RAIN_TCB* pTcb;

	/* must have a name */
	if(NULL == name)
	{
		return (ERROR);
	}

	stackSize = STACK_ROUND_UP(stackSize);
	pTaskMem = (char*)objAllocExtra(taskClassId, (unsigned)stackSize, (void**)NULL);

	pStackBase = pTaskMem + stackSize;
	pTcb = (RAIN_TCB*)pStackBase; /* grows down from tcb */

	options |= KERNEL_DEALLOC_STACK;

	if(OK != taskInit(pTcb, name, priority, options, pStackBase, stackSize, entry,
		arg1, arg2, arg3, arg4, arg5))
	{
		objFree(taskClassId, pTaskMem);
		return ((int)NULL);
	}

	return ((int)pTcb);
}

STATUS taskInit(FAST RAIN_TCB* pTcb
	, char* name
	, int priority
	, int options
	, char* pStackBase	/* high address is the begin of stack */
	, int stackSize
	, FUNCTION entry
	, int arg1
	, int arg2
	, int arg3
	, int arg4
	, int arg5)
{
	int pArgs[MAX_TASK_ARGS];
	unsigned taskNameLen = 0;
	char* taskName;
	
	/* TODO cannot create from ISR */
	
	if((!taskLibInstalled) && (OK != taskLibInit()))
	{
		return (ERROR);
	}

	pArgs[0] = arg1;
	pArgs[1] = arg2;
	pArgs[2] = arg3;
	pArgs[3] = arg4;
	pArgs[4] = arg5;

	pTcb->name = NULL;
	pTcb->options = options;
	pTcb->status = KERNEL_SUSPEND;
	pTcb->priority = priority;
	pTcb->priNormal = priority;
	pTcb->priMutexCnt = 0;
	pTcb->lockCnt = 0;
	pTcb->tslice = 0;
	pTcb->pPendQ = NULL;
	pTcb->safeCnt = 0;

	qInit(&pTcb->safetyQHead, Q_PRI_LIST, 1, 2, 3, 4, 5);

	pTcb->pStackBase = pStackBase;
	pTcb->pStackLimit = pStackBase - stackSize;
	pTcb->pStackEnd = pTcb->pStackLimit;

	memset(pTcb->pStackEnd, 0, stackSize);

	taskRegsInit(pTcb, pStackBase);
	
	pTcb->entry = entry;
	pTcb->errorStatus = 0;
	pTcb->exitCode = 0;
	pTcb->taskTicks = 0;
	pTcb->taskIncTicks = 0;
	
	pTcb->reserved1 = (int)NULL;
	pTcb->reserved2 = (int)NULL;
	pTcb->spare1 = (int)NULL;
	pTcb->spare2 = (int)NULL;
	pTcb->spare3 = (int)NULL;
	pTcb->spare4 = (int)NULL;

	taskArgsSet(pTcb, pStackBase, pArgs);

	kernelState = TRUE;

	/* add to active queue */
	kernelSpawn(pTcb);

	objCoreInit(&pTcb->objCore, taskClassId);	/* it's a taskclass */

	/* kernelState will be set to FALSE hear,
		and then tasks can run. */
	kernelExit();

	if(NULL != name)
	{
		taskNameLen = (unsigned)(strlen(name)+1);
		taskName = taskStackAlloc((int)pTcb, taskNameLen);
		strcpy(taskName, name);
	}

	pTcb->name = taskName;

	return (OK);
}

STATUS taskActivate(int tid)
{
	return taskResume(tid);
}

STATUS taskResume(int tid)
{
	if(kernelState)
	{
		if((0 == tid) || (!TASK_ID_VERIFY(tid)))
		{
			return (ERROR);
		}

		workQAdd1 ((FUNCTION)kernelResume, tid);
		return (OK);
	}

	if(0 == tid)
	{
		return (OK);
	}

	if(!TASK_ID_VERIFY(tid))
	{
		return (ERROR);
	}

	kernelResume((RAIN_TCB*)tid);

	kernelExit();

	return (OK);
}

STATUS taskDestroy(int  tid, BOOL dealloc, int  timeout, BOOL forceDestroy)
{
	FAST int	  ix;		/* delete hook index */
    FAST RAIN_TCB *pTcb;	/* convenient pointer to WIND_TCB */
    FAST int      lock;		/* to lock interrupts */
    int		  status;	/* windDelete return status */

	/* TODO */
    /*if (INT_RESTRICT () != OK)
	return (ERROR);*/

    if (tid == 0)
    {
		pTcb = taskCurrent;					/* suicide */
    }
    else
    {
		pTcb = (RAIN_TCB *) tid;			/* convenient pointer */
    }

again:
    lock = intLock ();						/* LOCK INTERRTUPTS */

    if (!TASK_ID_VERIFY(pTcb))				/* TRUE/FALSE valid task ID? */
	{
		intUnlock(lock);					/* UNLOCK INTERRUPTS */
		return (ERROR);
	}

    while ((pTcb->safeCnt > 0) ||
	   ((pTcb->status == KERNEL_READY) && (pTcb->lockCnt > 0)))
	{
		kernelState = TRUE;						/* KERNEL ENTER */

		intUnlock (lock);						/* UNLOCK INTERRUPTS */

		if ((forceDestroy) || (pTcb == taskIdCurrent))
		{

		    pTcb->safeCnt = 0;					/* unprotect */
		    pTcb->lockCnt = 0;					/* unlock */

		    if (Q_FIRST (&pTcb->safetyQHead) != NULL)	/* flush safe queue */
	        {
				kernelPendQFlush (&pTcb->safetyQHead);
	        }

		    kernelExit ();								/* KERNEL EXIT */
		}
		else											/* wait to destroy */
		{
		    if (kernelPendQPut (&pTcb->safetyQHead, timeout) != OK)
			{
				kernelExit ();							/* KERNEL EXIT */
				return (ERROR);
			}

		    switch (kernelExit())
			{
			case RESTART :
			    /* Always go back and reverify, this is because we have
			     * been running in a signal handler for who knows how long.
			     */
			    /*timeout = SIG_TIMEOUT_RECALC(timeout);*/
			    goto again;

			case ERROR :						/* timed out */
			    return (ERROR);

			default :							/* we were flushed */
			    break;
			}

		    /* All deleters of safe tasks block here.  When the safeCnt goes
		     * back to zero (or we taskUnlock) the deleters will be unblocked
		     * and the highest priority task among them will be elected to
		     * complete the deletion.  All unelected deleters will ultimately
		     * find the ID invalid, and return ERROR when they proceed from
		     * here.  The complete algorithm is summarized below.
		     */
		}

		lock = intLock ();					/* LOCK INTERRTUPTS */

		if(!TASK_ID_VERIFY (pTcb))
	    {
	    	intUnlock (lock);				/* UNLOCK INTERRUPTS */
	    	errno = 1; 						/* TODO */
	    	return (ERROR);
	    }
	}

    TASK_SAFE ();							/* protect deleter */

    pTcb->safeCnt++;						/* reprotect victim */

    if (pTcb != taskCurrent)				/* if not a suicide */
	{
		kernelState = TRUE;						/* ENTER KERNEL */
		intUnlock (lock);						/* UNLOCK INTERRUPTS */
		kernelSuspend (pTcb);					/* suspend victim */
		kernelExit ();							/* EXIT KERNEL */
	}
    else
    {
		intUnlock (lock);						/* UNLOCK INTERRUPTS */
    }

    /* run the delete hooks in the context of the deleting task */

	/* TODO no hook for delete */
    /*for (ix = 0; ix < VX_MAX_TASK_DELETE_RTNS; ++ix)
	if (taskDeleteTable[ix] != NULL)
	    (*taskDeleteTable[ix]) (pTcb);*/

    TASK_LOCK ();					/* LOCK PREEMPTION */

    if ((dealloc) && (pTcb->options & KERNEL_DEALLOC_STACK))
    {
		extern int rootTaskId;
		extern char* pRootMemStart;
		extern unsigned rootMemNBytes;
		
		if (pTcb == (RAIN_TCB *) rootTaskId)
		    memAddToPool(pRootMemStart, rootMemNBytes);		/* add root into pool */
		else
		    objFree (taskClassId, (char *) pTcb - 16);
	}

    lock = intLock();					/* LOCK INTERRUPTS */

    objCoreTerminate(&pTcb->objCore);			/* INVALIDATE TASK PS:对象判定与修改都需要禁中断 */

    kernelState = TRUE;					/* KERNEL ENTER */

    intUnlock(lock);					/* UNLOCK INTERRUPTS */

    status = kernelDelete(pTcb);				/* delete task */

    /* no failure notification until we have a better solution */

    /*if (pTcb->pSmObjTcb != NULL)
	{
		if (status == OK)	
	    {
	    	(*smObjTcbFreeRtn) (pTcb->pSmObjTcb);	
	    }
	}*/

    if (Q_FIRST(&pTcb->safetyQHead) != NULL)		/*flush any deleters */
    {
        kernelPendQFlush(&pTcb->safetyQHead);
   	}

    kernelExit();					/* KERNEL EXIT */

    /* we won't get here if we committed suicide */

    taskUnlock();					/* UNLOCK PREEMPTION */
    taskUnsafe();					/* TASK UNSAFE */

    return (OK);
}

STATUS taskDelay(int ticks)
{
	STATUS status;
	int tid = (int)taskIdCurrent;

	/* TODO ISR not allowed */

	kernelState = TRUE;
	if(NO_WAIT == ticks)
	{
		Q_REMOVE(&readyQHead, taskIdCurrent);
		Q_PUT(&readyQHead, taskIdCurrent, taskCurrent->priority);
	}
	else
	{
		kernelDelay(ticks);
	}

	if(RESTART == (status = kernelExit()))
	{
		status = ERROR;
	}

	return (status);
	
}

STATUS taskSuspend(int tid)
{
    if (kernelState)				/* defer work if in kernel */
	{
		if ((tid == 0) || (!TASK_ID_VERIFY ((void *)tid)))
		{
		    return (ERROR);
		}

		workQAdd1 ((FUNCTION)kernelSuspend, tid);	/* add work to kernel work q */
		return (OK);
	}

    if (tid == 0)
    {
		tid = taskIdCurrent;
    }

    kernelState = TRUE;				/* KERNEL ENTER */

    if(!TASK_ID_VERIFY ((void *)tid))	/* verify task */
	{
		kernelExit();				/* KERNEL EXIT */
		return (ERROR);
	}

    kernelSuspend ((RAIN_TCB *) tid);		/* suspend a task */

    kernelExit ();				/* KERNEL EXIT */

    return (OK);
    }


static void taskRegsInit(RAIN_TCB* pTcb, char* pStackBase)
{
	FAST int idx;
	extern ULONG _gp;	/* global pointer value. from compiler. */

	pTcb->regs.sr = taskSrDefault;
	pTcb->regs.pc = (INSTR*)kernelTaskEntry;	/* global task entry hear */
	pTcb->regs.lo = 0;
	pTcb->regs.hi = 0;
	pTcb->regs.cause = 0;
	pTcb->regs.fpcsr = 0;

	for(idx = 31; idx >= 0; idx--)
	{
		pTcb->regs.gpreg[idx] = 0;
	}

	pTcb->regs.gpReg = (_RType)&_gp;
	pTcb->regs.spReg = (_RType)((int)pStackBase - (MAX_TASK_ARGS * sizeof(_RType)));
}

void taskRtnValueSet(RAIN_TCB *pTcb, int returnValue)
{
    pTcb->regs.v0Reg = returnValue;
}

static void taskArgsSet(RAIN_TCB* pTcb, char* pStackBase, int pArgs[])
{
	FAST int idx;
	FAST _RType *sp = (_RType*)pStackBase;

	for(idx = MAX_TASK_ARGS-1; idx >= 0; idx--)
	{
		*--sp = (_RType)pArgs[idx];
	}

	/* a0Reg is a macro */
	/* a0~a3 is used for argument is MIPS */
	pTcb->regs.a0Reg = pArgs[0];
	pTcb->regs.a1Reg = pArgs[1];
	pTcb->regs.a2Reg = pArgs[2];
	pTcb->regs.a3Reg = pArgs[3];
}

void* taskStackAlloc(int tid, unsigned bytes)
{
	char* pStackPrev;
	RAIN_TCB* pTcb = taskTcb(tid);

	bytes = STACK_ROUND_UP(bytes);

	if((NULL == pTcb) ||
		(bytes > (pTcb->pStackBase - pTcb->pStackLimit )))
	{/* not enough */
		return (NULL);
	}

	pStackPrev = pTcb->pStackLimit;
	pTcb->pStackLimit += bytes;

	return ((void*)pTcb->pStackLimit);
}

RAIN_TCB* taskTcb(int tid)
{
	if(0 == tid)
	{
		tid = taskIdCurrent;
	}

	if(!TASK_ID_VERIFY(tid))
	{
		return NULL;
	}

	return ((RAIN_TCB*)tid);
}

int taskIdSelf ()
{
    return (taskIdCurrent);
}


void exit(int code)
{
	taskCurrent->exitCode = code;
	taskLock();
	taskCurrent->options |= KERNEL_UNBREAKABLE;
	taskUnlock();

	taskDestroy (0, TRUE, WAIT_FOREVER, FALSE);	

}

STATUS taskUnlock (void)
{
	FAST RAIN_TCB *pTcb = taskCurrent;

	if ((pTcb->lockCnt > 0) && ((-- pTcb->lockCnt) == 0)) 	/* unlocked? */
	{
		kernelState = TRUE; 								/* KERNEL ENTER */

		if ((Q_FIRST (&pTcb->safetyQHead) != NULL) && (pTcb->safeCnt == 0))
		{
				kernelPendQFlush(&pTcb->safetyQHead);		/* flush safe queue */
		}

		kernelExit ();										/* KERNEL EXIT */
	}

	return (OK);
}

STATUS taskLock(void)
{
	TASK_LOCK();											/* increment lock cnt */

	return (OK);
}

STATUS taskSafe (void)
{
	TASK_SAFE ();											/* increment safe cnt */

	return (OK);
}

STATUS taskUnsafe (void)
{
    FAST RAIN_TCB *pTcb = taskCurrent;

    if ((pTcb->safeCnt > 0) && ((-- pTcb->safeCnt) == 0)) 	/* unsafe? */
	{
		kernelState = TRUE;									/* KERNEL ENTER */

        if (Q_FIRST (&pTcb->safetyQHead) != NULL)			/* deleter waiting? */
        {
            kernelPendQFlush (&pTcb->safetyQHead);
        }

		kernelExit ();										/* KERNEL EXIT */
	}

    return (OK);
}

