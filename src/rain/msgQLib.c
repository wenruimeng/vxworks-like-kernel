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

/* locals */

static OBJ_CLASS msgQClass;

/* globals */
CLASS_ID msgQClassId = &msgQClass;

static BOOL msgQLibInstalled;	/* protect from muliple inits */

static STATUS msgQDestroy(MSG_Q_ID msgQId, BOOL dealloc);
STATUS msgQInit(FAST MSG_Q* pMsgQ, int maxMsgs, int maxMsgLength, int options, void* pMsgPool);

STATUS msgQLibInit (void)
{
    /* initialize shared memory objects function pointers */

    if (!msgQLibInstalled)
	{
		if (classInit (msgQClassId, sizeof (MSG_Q), OFFSET (MSG_Q, objCore),
			       (FUNCTION)msgQCreate, (FUNCTION)msgQInit,
			       (FUNCTION)msgQDestroy) == OK)
		{
			    msgQLibInstalled = TRUE;
		}
	}

    return ((msgQLibInstalled) ? OK : ERROR);
}

MSG_Q_ID msgQCreate(int maxMsgs, int maxMsgLength, int options)
{
    MSG_Q_ID	msgQId;
    void *	pPool;		/* pointer to memory for messages */
    UINT	size = (UINT) maxMsgs * MSG_NODE_SIZE (maxMsgLength);

    /* TODO restrict ISR from calling */
	
    if ((!msgQLibInstalled) && (msgQLibInit () != OK))
		return (NULL);			/* package init problem */

    if ((msgQId = (MSG_Q_ID)objAllocExtra (msgQClassId, size, &pPool)) == NULL)
		return (NULL);

    if (msgQInit (msgQId, maxMsgs, maxMsgLength, options, pPool) != OK)
	{
		objFree (msgQClassId, (char *) msgQId);
		return (NULL);
	}

    return ((MSG_Q_ID) msgQId);
}

STATUS msgQDelete(MSG_Q_ID msgQId)
{
    return (msgQDestroy (msgQId, TRUE));
}


STATUS msgQInit(FAST MSG_Q* pMsgQ, int maxMsgs, int maxMsgLength, int options, void* pMsgPool)
{
	FAST int nodeSize = MSG_NODE_SIZE(maxMsgLength);
	FAST int ix;
	FAST QUEUE_ID msgQType;

	if ((!msgQLibInstalled) && (msgQLibInit () != OK))
	{
		return (ERROR); 			/* package init problem */
	}

	memset((char*)pMsgQ, 0, sizeof(*pMsgQ));

	switch(options & MSG_Q_TYPE_MASK)
	{
	case MSG_Q_FIFO:
		msgQType = Q_FIFO;
		break;
	case MSG_Q_PRIORITY:
		msgQType = Q_PRI_LIST;
		break;
	default:
		msgQType = Q_FIFO;
		break;
	}

	if((qInit(&pMsgQ->msgQ, qJobClassId, msgQType, 2,3,4,5) != OK) ||
		(qInit(&pMsgQ->freeQ, qJobClassId, msgQType, 2,3,4,5) != OK))
	{
		return (ERROR);
	}

	for(ix = 0; ix < maxMsgs; ix++)
	{
		qJobPut(pMsgQ, &pMsgQ->freeQ, (Q_JOB_NODE*)pMsgPool, Q_JOB_PRI_DONT_CARE);
		pMsgPool = (void*)((char*)pMsgPool + nodeSize);
	}

	/*printf("pMsgQ->freeQ.first:0x%x\n", pMsgQ->freeQ.first);*/

	pMsgQ->options = options;
	pMsgQ->maxMsgs = maxMsgs;
	pMsgQ->maxMsgLength = maxMsgLength;

	objCoreInit(&pMsgQ->objCore, msgQClassId);

	return (OK);
}

STATUS msgQSend(FAST MSG_Q_ID msgQId, char* buffer, FAST UINT nBytes, int timeout, int priority)
{
	FAST MSG_NODE* pMsg;

	if(!INT_CONTEX())
	{
		TASK_LOCK();
	}
	else
	{
		/* ISR not pend */
		if(0 != timeout)
		{
			return (ERROR);
		}
	}

restart:
	if(!IS_CLASS(msgQId, msgQClassId))
	{
		if(!INT_CONTEX())
		{
			TASK_UNLOCK();
		}
		return (ERROR);
	}

	if(nBytes > msgQId->maxMsgLength)
	{
		if(!INT_CONTEX())
		{
			TASK_UNLOCK();
		}
		return (ERROR);
	}

	pMsg = (MSG_NODE*)qJobGet(msgQId, &msgQId->freeQ, timeout);
	
	if(pMsg == (MSG_NODE*)NONE)
	{
		/* TODO recalc timeout and why? */
		goto restart;
	}

	if(pMsg == NULL)
	{
		msgQId->sendTimeouts++;
		if(!INT_CONTEX())
		{
			TASK_UNLOCK();
		}
		return (ERROR);
	}

	pMsg->msgLength = nBytes;
	memcpy(MSG_NODE_DATA(pMsg), buffer, (int)nBytes);

	if(OK != qJobPut(msgQId, &msgQId->msgQ, &pMsg->node, priority))
	{
		if(!INT_CONTEX())
		{
			TASK_UNLOCK();
		}
		return (ERROR);
	}

	if(!INT_CONTEX())
	{
		TASK_UNLOCK();
	}

	return (OK);
}

int msgQReceive(FAST MSG_Q_ID msgQId, char* buffer, UINT maxNBytes, int timeout)
{
	FAST MSG_NODE* pMsg;
	FAST int bytesReturned;

	/* TODO int restrict */

	if((int)maxNBytes < 0)
	{
		return (ERROR);
	}

	TASK_LOCK();
restart:
	if(!IS_CLASS(msgQId, msgQClassId))
	{
		TASK_UNLOCK();
		return (ERROR);
	}

	pMsg = (MSG_NODE*)qJobGet(msgQId, &msgQId->msgQ, timeout);

	if(pMsg == (MSG_NODE*)NONE)
	{
		/* timeout recalc */
		goto restart;
	}

	if(pMsg == NULL)
	{
		msgQId->recvTimeouts++;
		TASK_UNLOCK();
		return (ERROR);
	}

	bytesReturned = MIN(pMsg->msgLength, maxNBytes);
	memcpy(buffer, MSG_NODE_DATA(pMsg), bytesReturned);

	qJobPut(msgQId, &msgQId->freeQ, &pMsg->node, Q_JOB_PRI_DONT_CARE);

	TASK_UNLOCK();

	return (bytesReturned);
}

static STATUS msgQDestroy(MSG_Q_ID msgQId, BOOL dealloc)
{
	Q_JOB_NODE* pNode;
	FAST int timeout;
	FAST int nMsgs;
	
	/* TODO isr not allowed */

	TASK_SAFE();
	TASK_LOCK();

	if(!IS_CLASS(msgQId, msgQClassId))
	{
		TASK_UNLOCK();
		TASK_UNSAFE();
		return (ERROR);
	}

	objCoreTerminate(&msgQId->objCore);

	TASK_UNLOCK();

	timeout = NO_WAIT;
	nMsgs = 0;

	while(nMsgs < msgQId->maxMsgs)
	{
		while(((pNode = qJobGet(msgQId, &msgQId->freeQ, timeout)) != NULL) &&
			(pNode != (Q_JOB_NODE*)NONE))
		{
			nMsgs++;
		}

		while(((pNode = qJobGet(msgQId, &msgQId->msgQ, timeout)) != NULL) &&
			(pNode != (Q_JOB_NODE*)NONE))
		{
			nMsgs++;
		}

		timeout = 1;
	}

	kernelState = TRUE;

	qJobTerminate(&msgQId->msgQ);
	qJobTerminate(&msgQId->freeQ);

	kernelExit();

	if(dealloc)
	{
		objFree(msgQClassId, (char*)msgQId);
	}

	TASK_UNSAFE();

	return (OK);
}

