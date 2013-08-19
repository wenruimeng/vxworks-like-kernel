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


static STATUS qFifoNullRtn();

static QUEUE_BASE qFifoQueue = {
    (FUNCTION)qFifoCreate,
    (FUNCTION)qFifoInit,
    (FUNCTION)qFifoDelete,
    (FUNCTION)qFifoNullRtn,
    (FUNCTION)qFifoPut,
    (FUNCTION)qFifoGet,
    (FUNCTION)qFifoRemove,
    (FUNCTION)qFifoNullRtn,
    (FUNCTION)qFifoNullRtn,
    (FUNCTION)qFifoNullRtn,
    (FUNCTION)qFifoNullRtn,
    (FUNCTION)qFifoNullRtn,
    (FUNCTION)qFifoEach,
    &qFifoQueue
};

QUEUE_ID qFifoQueueId = &qFifoQueue;

Q_FIFO_HEAD* qFifoCreate(void)
{
	return NULL;
}

STATUS qFifoInit(Q_FIFO_HEAD* pFifoHead)
{
	dllInit(pFifoHead);
	return (OK);
}

STATUS qFifoDelete(Q_FIFO_HEAD* pFifoHead)
{
	return (ERROR);
}

void qFifoPut(Q_FIFO_HEAD* pFifoHead, Q_FIFO_NODE* pFifoNode, ULONG key)
{
	if(FIFO_KEY_HEAD == key)
	{/* add to head */
		dllInsert(pFifoHead, (DL_NODE*)NULL, pFifoNode);
	}
	else
	{/* add to last */
		dllAdd(pFifoHead, pFifoNode);
	}
}

/* Get the first one */
Q_FIFO_NODE* qFifoGet(Q_FIFO_HEAD* pFifoHead)
{
	if(DLL_EMPTY(pFifoHead))
	{
		return (NULL);
	}

	return ((Q_FIFO_NODE*)dllGet(pFifoHead));
}

STATUS qFifoRemove(Q_FIFO_HEAD* pFifoHead, Q_FIFO_NODE* pFifoNode)
{
	dllRemove(pFifoHead, pFifoNode);
	return (OK);
}

/* TODO support for each */
Q_FIFO_NODE* qFifoEach(Q_FIFO_HEAD* pFifoHead, FUNCTION routine, int routineArg)
{
	FAST Q_FIFO_NODE* pQNode = (Q_FIFO_NODE*)DLL_FIRST(pFifoHead);

	while((NULL != pQNode) && ((*routine)(pQNode, routineArg)))
	{
		pQNode = (Q_FIFO_NODE*)DLL_NEXT(pQNode);
	}

	return pQNode;
}

static STATUS qFifoNullRtn()
{
    return (OK);
}

