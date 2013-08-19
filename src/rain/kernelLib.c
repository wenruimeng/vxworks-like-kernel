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

#define MEM_BLOCK_HDR_SIZE		STACK_ROUND_UP(sizeof(BLOCK_HDR))
#define MEM_BLOCK_FREE_SIZE		STACK_ROUND_UP(sizeof(FREE_BLOCK))
#define RAIN_TCB_SIZE 			STACK_ROUND_UP(sizeof(RAIN_TCB))
#define MEM_BASE_BLOCK_SIZE 	(MEM_BLOCK_HDR_SIZE + MEM_BLOCK_FREE_SIZE)
#define MEM_TOT_BLOCK_SIZE 		((2*MEM_BLOCK_HDR_SIZE) + MEM_BLOCK_FREE_SIZE)

Q_HEAD	  activeQHead = {NULL,0,0,NULL};
Q_HEAD	  tickQHead;
Q_HEAD	  readyQHead;

unsigned rootMemNBytes = 0;
char* kernelIntStackBase = NULL;
char* kernelIntStackEnd = NULL;
char* pRootMemStart = NULL;
int rootTaskId = 0;

void kernelPreInit()
{
	qInit (&activeQHead, Q_FIFO, 1,2,3,4,5);
	qInit (&readyQHead, Q_PRI_LIST, 1,2,3,4,5);
	qInit (&tickQHead, Q_PRI_LIST, 1,2,3,4,5);
	workQInit();
}

void kernelInit(FUNCTION rootRtn
	, unsigned rootMemSize
	, char* pMemPoolStart
	, char* pMemPoolEnd
	, unsigned intStackSize)
{
	union
	{
		double align8;
		RAIN_TCB initTcb;
	}tcbAligned;

	RAIN_TCB* pTcb;
	unsigned rootStackSize;
	unsigned memPoolSize;
	char* pRootStackBase;

	rootMemNBytes = STACK_ROUND_UP(rootMemSize);
	pMemPoolStart = (char*)STACK_ROUND_UP(pMemPoolStart);
	pMemPoolEnd = (char*)STACK_ROUND_DOWN(pMemPoolEnd);
	intStackSize = STACK_ROUND_UP(intStackSize);

	kernelTicks = 0;

	kernelIntStackBase = pMemPoolStart+intStackSize;
	kernelIntStackEnd = pMemPoolStart;
	memset(kernelIntStackEnd, 0, intStackSize);

	rootStackSize = rootMemNBytes - RAIN_TCB_SIZE - MEM_TOT_BLOCK_SIZE;
	pRootMemStart = pMemPoolEnd - rootMemNBytes;

	pRootStackBase = pRootMemStart + rootStackSize + MEM_BASE_BLOCK_SIZE;
	pTcb = (RAIN_TCB*)pRootStackBase;

	taskIdCurrent = (RAIN_TCB*)NULL;
	memset(&tcbAligned.initTcb, 0, sizeof(tcbAligned));

	memPoolSize = (unsigned)((int)pRootMemStart - (int)pMemPoolStart);

	taskInit(pTcb, "tRootTask", 0, KERNEL_UNBREAKABLE | KERNEL_DEALLOC_STACK, 
		pRootStackBase, (int)rootStackSize, (FUNCTION)rootRtn, 
		(int)pMemPoolStart, (int)memPoolSize, 3,4,5);

	rootTaskId = (int)pTcb;

	taskIdCurrent = &tcbAligned.initTcb;

	taskActivate((int)pTcb);
}

void kernelSpawn(FAST RAIN_TCB* pTcb)
{
	Q_PUT(&activeQHead, &pTcb->activeNode, FIFO_KEY_TAIL);
}

void kernelReadyQPut(RAIN_TCB* pTcb)
{
	pTcb->status &= ~KERNEL_SUSPEND;

	taskRtnValueSet(pTcb, OK);

	if(pTcb->status & KERNEL_DELAY)
	{
		pTcb->status &= KERNEL_DELAY;
		Q_REMOVE(&tickQHead, &pTcb->tickNode);
	}

	if(pTcb->status == KERNEL_READY)
	{
		Q_PUT(&readyQHead, pTcb, pTcb->priority);
	}
}

void kernelReadQRemove(Q_HEAD* pQHead, int timeout)
{
	Q_REMOVE(&readyQHead, taskIdCurrent);

	taskCurrent->status |= KERNEL_PEND;
	taskCurrent->pPendQ = pQHead;

	if(WAIT_FOREVER != timeout)
	{
		if ((unsigned)(kernelTicks + timeout) < kernelTicks)    /* rollover? */
        {
            Q_CALIBRATE (&tickQHead, ~kernelTicks + 1);
            kernelTicks = 0;
        }

		Q_PUT (&tickQHead, &taskCurrent->tickNode, timeout + kernelTicks);
		taskCurrent->status |= KERNEL_DELAY;
	}
	
}

void kernelResume(FAST RAIN_TCB* pTcb)
{
	if(KERNEL_SUSPEND == pTcb->status)
	{
		Q_PUT(&readyQHead, pTcb, pTcb->priority);
	}

	pTcb->status &= ~KERNEL_SUSPEND;
}

void kernelSemDelete(FAST SEM_ID semId)
{
	FAST RAIN_TCB* pTcb;
	
	kernelPendQTerminate(&semId->qHead);

	if((SEM_TYPE_MUTEX == semId->semType) && 
		((pTcb = semId->semOwner) != NULL) &&
		(IS_CLASS(&pTcb->objCore, taskClassId)))
	{
		if((semId->options & SEM_INVERSION_SAFE) &&
			((--pTcb->priMutexCnt) == 0) &&
			(pTcb->priority != pTcb->priNormal))
		{
			kernelPrioritySet(pTcb, pTcb->priNormal);
		}

		if((semId->options & SEM_DELETE_SAFE) &&
			((--pTcb->safeCnt) == 0) &&
			(Q_FIRST(&pTcb->safetyQHead) != NULL))
		{
			kernelPendQFlush(&pTcb->safetyQHead);
		}
	}
}

void kernelPendQFlush(Q_HEAD* pQHead)
{	
	FAST RAIN_TCB* pTcb;

	while(NULL != (pTcb = (RAIN_TCB*)Q_GET(pQHead)))
	{
		pTcb->status &= ~KERNEL_PEND;
		taskRtnValueSet(pTcb, OK);

		if(pTcb->status & KERNEL_DELAY)
		{
			pTcb->status &= ~KERNEL_DELAY;
			Q_REMOVE (&tickQHead, &pTcb->tickNode);
		}

		if(pTcb->status == KERNEL_READY)
		{
			Q_PUT(&readyQHead, pTcb, pTcb->priority);
		}
	}
}

void kernelPendQGet(Q_HEAD* pQHead)
{
	FAST RAIN_TCB* pTcb = (RAIN_TCB*)Q_GET(pQHead);

	pTcb->status &= ~KERNEL_PEND;
	taskRtnValueSet(pTcb, OK);
	
	if(pTcb->status & KERNEL_DELAY)
	{
		pTcb->status &= ~KERNEL_DELAY;
		Q_REMOVE (&tickQHead, &pTcb->tickNode);
	}

	if(pTcb->status == KERNEL_READY)
	{
		Q_PUT(&readyQHead, pTcb, pTcb->priority);
	}
}

STATUS kernelPendQPut(FAST Q_HEAD* pQHead, FAST int timeout)
{
	if(NO_WAIT == timeout)
	{
		return (ERROR);
	}

	Q_REMOVE(&readyQHead, taskIdCurrent);
	taskCurrent->status |= KERNEL_PEND;

	taskCurrent->pPendQ = pQHead;

	Q_PUT(pQHead, taskIdCurrent, taskCurrent->priority);

	if(WAIT_FOREVER != timeout)
	{
		/* 如果是负数，则将现有的项 + |负数值| 并将重新计时 */
		if ((unsigned)(kernelTicks + timeout) < kernelTicks)	/* rollover? */
	    {
		    Q_CALIBRATE (&tickQHead, ~kernelTicks + 1);
		    kernelTicks = 0;
	    }

		Q_PUT (&tickQHead, &taskCurrent->tickNode, timeout + kernelTicks);
		taskCurrent->status |= KERNEL_DELAY;
	}

	return (OK);
}

void kernelPendQTerminate(Q_HEAD* pQHead)
{
	FAST RAIN_TCB* pTcb;

	while(NULL != (pTcb = (RAIN_TCB*) Q_GET(pQHead)))
	{
		pTcb->status &= ~KERNEL_PEND;
		pTcb->pPriMutex = NULL;
		pTcb->errorStatus = ERROR; /* TODO some error type */

		taskRtnValueSet(pTcb, ERROR);

		if(pTcb->status & KERNEL_DELAY)
		{
			pTcb->status &= ~KERNEL_DELAY;
			Q_REMOVE (&tickQHead, &pTcb->tickNode);
		}

		if(pTcb->status == KERNEL_READY)
		{
			Q_PUT(&readyQHead, pTcb, pTcb->priority);
		}
	}
}

void kernelSuspend(FAST RAIN_TCB *pTcb)
{
    if (pTcb->status == KERNEL_READY)
    {
		Q_REMOVE (&readyQHead, pTcb);
    }

    pTcb->status |= KERNEL_SUSPEND;		/* update status */
}

STATUS kernelDelete(FAST RAIN_TCB *pTcb)
{
    FAST USHORT mask;
    FAST int ix;
    int  status = OK;		/* status return by windPendQRemove */

    if (pTcb->status == KERNEL_READY)			/* if task is ready */
    {
		Q_REMOVE (&readyQHead, pTcb);			/* remove from queue */
    }
    else
	{
		if (pTcb->status & KERNEL_PEND)			/* if task is pended */
	            status = kernelPendQRemove (pTcb);            /* remove from queue */

		if (pTcb->status & KERNEL_DELAY)			/* if task is delayed */
		    Q_REMOVE (&tickQHead, &pTcb->tickNode);	/* remove from queue */
	}

    Q_REMOVE (&activeQHead, &pTcb->activeNode);		/* deactivate it */

    pTcb->status = KERNEL_DEAD;		       		/* kill it */

    return (status);
}

STATUS kernelPendQRemove(RAIN_TCB *pTcb)
{
	int status = Q_REMOVE (pTcb->pPendQ, pTcb); /* out of pend queue */

	pTcb->status   &= ~KERNEL_PEND;		/* turn off pend bit */
	pTcb->pPriMutex = NULL; 			/* clear pPriMutex */

	if (pTcb->status & KERNEL_DELAY)		/* task was timing out */
	{
		pTcb->status &= ~KERNEL_DELAY;
		Q_REMOVE (&tickQHead, &pTcb->tickNode); /* remove from queue */
	}

	return (status);
}

void kernelTickAnnounce()
{
	FAST Q_NODE *	pNode;	/* node of tick queue */
	FAST RAIN_TCB * pTcb;	/* pointer to task control block */
	int 		status; 	/* status return by Q_REMOVE */

	/* advance and manage the tick queue */

	kernelTicks ++; 					/* advance rel time */
	kernelAbsTicks++;					/* advance abs time */

	Q_ADVANCE (&tickQHead); 			/* advance tick queue */

	while ((pNode = (Q_NODE *) Q_GET_EXPIRED (&tickQHead)) != NULL)
	{
		pTcb = (RAIN_TCB *) ((int)pNode - OFFSET (RAIN_TCB, tickNode));

		if((pTcb->objCore.pObjClass == taskClassId))
		{
			pTcb->status &= ~KERNEL_DELAY;		/* out of delay state */

			if (pTcb->status == KERNEL_READY) 	/* taskDelay done */
			{
				taskRtnValueSet (pTcb, OK); 	/* return OK */
			}
			else if (pTcb->status & KERNEL_PEND)		/* semaphore timeout */
			{
				status = kernelPendQRemove (pTcb);		/* out of semaphore q */

				switch (status)
				{
						/* the semaphore was given in time, return OK */
					/*case ALREADY_REMOVED:
						taskRtnValueSet (pTcb, OK);
						break;*/
					case ERROR:
						taskRtnValueSet (pTcb, ERROR);		/* return ERROR */
						pTcb->errorStatus = ERROR;
						break;
					default:
						taskRtnValueSet (pTcb, ERROR);		/* return ERROR */
						pTcb->errorStatus = ERROR;
				}
			}

			if (pTcb->status == KERNEL_READY) 			/* if ready, enqueue */
				Q_PUT (&readyQHead, pTcb, pTcb->priority);
		}
	}
}

void kernelDelay(FAST int timeout)
{
    Q_REMOVE (&readyQHead, taskIdCurrent);					/* out of ready queue */

    if ((unsigned)(kernelTicks + timeout) < kernelTicks)	/* rollover? */
	{
		Q_CALIBRATE (&tickQHead, ~kernelTicks + 1);
		kernelTicks = 0;
	}

    Q_PUT (&tickQHead, &taskCurrent->tickNode, timeout + kernelTicks);
    taskCurrent->status |= KERNEL_DELAY;					/* set delay status */

    return (OK);

}

/*
	a.semOwner=A
	b.semOwner=B
	B.priMutex=a
	C semTake(b)

	A	B	C
		b	
	a	a	b
*/
void kernelPrioritySet(FAST RAIN_TCB* pTcb, FAST UINT priority)
{
	if((0 == pTcb->pPriMutex) && (pTcb->priority < priority))
	{
		pTcb->priority = priority;
		if(KERNEL_READY == pTcb->status)
		{
			Q_RESORT(&readyQHead, pTcb, priority);
		}
		else if(KERNEL_PEND == pTcb->status)
		{	/* where to set this pPendQ? */
			Q_RESORT(pTcb->pPendQ, pTcb, priority);
		}
		return;
	}

	while(pTcb->priority > priority)
	{
		pTcb->priority = priority;
		if(KERNEL_READY == pTcb->status)
		{
			Q_RESORT(&readyQHead, pTcb, priority);
		}
		else if(KERNEL_PEND == pTcb->status)
		{
			Q_RESORT(pTcb->pPendQ, pTcb, priority);

			if(NULL != pTcb->pPriMutex)/* chain up the inheritance */
			{
				pTcb = pTcb->pPriMutex->semOwner;
			}
		}
	}
}

