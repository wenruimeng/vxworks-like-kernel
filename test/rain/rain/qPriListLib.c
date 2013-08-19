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

/* kernel tick */
extern ULONG kernelTicks;

static QUEUE_BASE qPriList = {
	(FUNCTION)qPriListCreate,
    (FUNCTION)qPriListInit,
    (FUNCTION)qPriListDelete,
    (FUNCTION)qPriListTerminate,
    (FUNCTION)qPriListPut,
    (FUNCTION)qPriListGet,
    (FUNCTION)qPriListRemove,
    (FUNCTION)qPriListResort,
    (FUNCTION)qPriListAdvance,
    (FUNCTION)qPriListGetExpired,
    (FUNCTION)qPriListKey,
    (FUNCTION)qPriListCalibrate,
    (FUNCTION)qPriListEach,
    &qPriList
};

QUEUE_ID qPriListId = &qPriList;

Q_PRI_HEAD* qPriListCreate()
{
	return NULL;
}

STATUS qPriListInit(Q_PRI_HEAD* pQPriHead)
{
	dllInit(pQPriHead);
	return (OK);
}

STATUS qPriListDelete(Q_PRI_HEAD* pQpriHead, Q_PRI_NODE* pQPriNode)
{
	return (ERROR);
}

STATUS qPriListTerminate(Q_PRI_HEAD* pQPriHead)
{
	return (ERROR);
}

void qPriListPut(Q_PRI_HEAD* pQPriHead, Q_PRI_NODE* pQPriNode, ULONG key)
{
	FAST Q_PRI_NODE* pQNode = (Q_PRI_NODE*)DLL_FIRST(pQPriHead);

	pQPriNode->key = key;

	while(NULL != pQNode)
	{
		/*	HEAD/number small ----<<<<<----TAIL/number big
			The smaller number is, the higher pri is.      */
		if(key < pQNode->key)
		{
			dllInsert(pQPriHead, DLL_PREVIOUS(&pQNode->node), &pQPriNode->node);
			return;
		}
		pQNode = (Q_PRI_NODE*)DLL_NEXT(&pQNode->node);
	}

	/* add at last */
	dllInsert(pQPriHead, DLL_LAST(pQPriHead), &pQPriNode->node);
}

/* TODO add from tail */

Q_PRI_NODE* qPriListGet(Q_PRI_HEAD* pQPriHead)
{
	if(DLL_EMPTY(pQPriHead))
	{
		return NULL;
	}

	return ((Q_PRI_NODE*)dllGet(pQPriHead));
}

STATUS qPriListRemove(Q_PRI_HEAD* pQPriHead, Q_PRI_NODE* pQPriNode)
{
	dllRemove(pQPriHead, &pQPriNode->node);

	return (OK);
}

/* If the key is updated, then it should be resort */
void qPriListResort(FAST Q_PRI_HEAD* pQPriHead, FAST Q_PRI_NODE* pQPriNode, FAST ULONG newKey)
{
	FAST Q_PRI_NODE* pPrev = (Q_PRI_NODE*)DLL_PREVIOUS(&pQPriNode->node);
	FAST Q_PRI_NODE* pNext = (Q_PRI_NODE*)DLL_NEXT(&pQPriNode->node);

	if( ((NULL == pPrev) || (newKey >= pPrev->key)) && 
		((NULL == pNext) || (newKey <= pNext->key)) )
	{/* just right hear */
		pQPriNode->key = newKey;
	}
	else
	{/* reput in right place */
		qPriListRemove(pQPriHead, pQPriNode);
		qPriListPut(pQPriHead, pQPriNode, newKey);
	}
	
}

void qPriListAdvance(Q_PRI_HEAD* pQPriHead)
{

}

Q_PRI_NODE *qPriListGetExpired(Q_PRI_HEAD* pQPriHead)
{
	FAST Q_PRI_NODE* pQNode = (Q_PRI_NODE*)DLL_FIRST(pQPriHead);

	if((NULL != pQNode) && (pQNode->key <= kernelTicks))
	{
		return ((Q_PRI_NODE*)dllGet(pQPriHead));
	}
	else
	{
		return NULL;
	}
}

void qPriListCalibrate(Q_PRI_HEAD *pQHead, ULONG keyDelta)
{
    FAST Q_PRI_NODE *pQPriNode;

    for (pQPriNode = (Q_PRI_NODE *) DLL_FIRST (pQHead);
		pQPriNode != NULL;
		pQPriNode = (Q_PRI_NODE *) DLL_NEXT (&pQPriNode->node))
	{
        pQPriNode->key += keyDelta;			/* offset key */
	}
}

#define NORMAL_TYPE 0
#define TIMER_TYPE 1
ULONG qPriListKey(Q_PRI_NODE* pQPriNode, int keyType)
{
	if(NORMAL_TYPE == keyType)
	{
		return (pQPriNode->key);
	}
	else
	{
		return (pQPriNode->key - kernelTicks);
	}
}

/* TODO support for it */
Q_PRI_NODE* qPriListEach(Q_PRI_HEAD* pQPriHead, FUNCTION routine, int arg)
{
	FAST Q_PRI_NODE* pQNode = (Q_PRI_NODE*)DLL_FIRST(pQPriHead);

	while((NULL != pQNode) && ((*routine)(pQNode, arg)))
	{
		pQNode = (Q_PRI_NODE*)DLL_NEXT(&pQNode->node);
	}

	return pQNode;
}

