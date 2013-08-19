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

static STATUS qJobNullRtn (void);
static void qJobPendQGet (MSG_Q_ID msgQId, Q_JOB_HEAD *pQHead);

STATUS qJobInit
(
    FAST Q_JOB_HEAD *pQHead,    /* queue head to initialize */
    QUEUE_ID pendQType        /* queue class for task pend queue */
);

STATUS qJobTerminate
(
    Q_JOB_HEAD *pQHead          /* queue head to terminate */
);


Q_JOB_HEAD *qJobCreate
(
    QUEUE_ID pendQType        /* queue class for task pend queue */
)
{
    FAST Q_JOB_HEAD *pQHead = (Q_JOB_HEAD *) malloc (sizeof (Q_JOB_HEAD));

    if (pQHead == NULL)
	return (NULL);

    if (qJobInit (pQHead, pendQType) != OK)
	{
	free ((char *) pQHead);
	return (NULL);
	}

    return (pQHead);
}

STATUS qJobInit
(
    FAST Q_JOB_HEAD *pQHead,    /* queue head to initialize */
    QUEUE_ID pendQType        /* queue class for task pend queue */
)
{
    pQHead->first	= NULL;
    pQHead->last	= NULL;
    pQHead->count	= 0;

    qInit (&pQHead->pendQ, pendQType, 1, 2, 3, 4, 5);

    return (OK);
}

STATUS qJobDelete
(
    Q_JOB_HEAD *pQHead
)
{
    qJobTerminate (pQHead);
    free ((char *) pQHead);
    return OK;
}

STATUS qJobTerminate
(
    Q_JOB_HEAD *pQHead          /* queue head to terminate */
)
{
    kernelPendQTerminate (&pQHead->pendQ);

    return (OK);
}

STATUS qJobPut(MSG_Q_ID msgQId, Q_JOB_HEAD* pQHead, Q_JOB_NODE* pNode, int key)
{
	FAST int level;

	if(0 == key)/* add tail */
	{
		pNode->next = NULL;
		level = intLock();

		if(pQHead->first == NULL)
		{
			pQHead->last = pQHead->first = pNode;
		}
		else
		{
			pQHead->last->next = pNode;
			pQHead->last = pNode;
		}
	}
	else	/* add head */
	{
		level = intLock();
		if((pNode->next = pQHead->first) == NULL)
		{
			pQHead->last = pNode;
		}

		pQHead->first = pNode;
	}
	
	pQHead->count++;

	if(kernelState)
	{
		intUnlock(level);
		workQAdd2((FUNCTION)qJobPendQGet, (int)msgQId, (int)pQHead);
	}
	else
	{
		if(Q_FIRST(&pQHead->pendQ) == NULL)
		{
			intUnlock(level);
		}
		else
		{
			kernelState = TRUE;
			intUnlock(level);

			kernelPendQGet(&pQHead->pendQ);

			kernelExit();
		}
	}
	return (OK);
	
}

static void qJobPendQGet(MSG_Q_ID msgQId, Q_JOB_HEAD *pQHead)
{
	if(Q_FIRST(&pQHead->pendQ) != NULL)
	{
		kernelPendQGet(&pQHead->pendQ);
	}
}

Q_JOB_NODE* qJobGet(MSG_Q_ID msgQId, FAST Q_JOB_HEAD* pQHead, FAST int timeout)
{
	int level;
	int status;
	Q_JOB_NODE* pNode;

	level = intLock();

	/* loop and wait */
	while((pNode = pQHead->first) == NULL)
	{
		if(timeout == 0)
		{
			intUnlock(level);
			return (NULL);
		}

		kernelState = TRUE;
		intUnlock(level);

		kernelPendQPut(&pQHead->pendQ, timeout);

		if((status = kernelExit()) == RESTART)
		{
			return ((Q_JOB_NODE*)NULL);
		}

		if(OK != status)
		{
			return (NULL);
		}

		if(!IS_CLASS(msgQId, msgQClassId))
		{
			return (NULL);
		}

		level = intLock();
	}

	pQHead->first = pNode->next;
	pQHead->count--;

	intUnlock(level);

	return (pNode);
}

Q_JOB_NODE *qJobEach
(
    Q_JOB_HEAD *pQHead,         /* queue head of queue to call routine for */
    FUNCTION     routine,        /* the routine to call for each table entry */
    int         routineArg      /* arbitrary user-supplied argument */
)
{
    FAST Q_JOB_NODE *pNode;
    FAST int level;

    level = intLock ();			/* LOCK INTERRUPTS */

    pNode = pQHead->first;

    while ((pNode != NULL) && ((* routine) (pNode, routineArg)))
	pNode = pNode->next;		/* next node */

    intUnlock (level);			/* UNLOCK INTERRUPTS */

    return (pNode);			/* return node we ended with */
}

static STATUS qJobNullRtn (void)
{
    return (OK);
}

static QUEUE_BASE qJobClass =
{
    (FUNCTION)qJobCreate,
    (FUNCTION)qJobInit,
    (FUNCTION)qJobDelete,
    (FUNCTION)qJobTerminate,
    (FUNCTION)qJobPut,
    (FUNCTION)qJobGet,
    (FUNCTION)qJobNullRtn,	/* no remove routine */
    (FUNCTION)qJobNullRtn,
    (FUNCTION)qJobNullRtn,
    (FUNCTION)qJobNullRtn,
    (FUNCTION)qJobNullRtn,
    (FUNCTION)qJobNullRtn,
    /*(FUNCTION)qJobInfo,*/
    (FUNCTION)qJobEach,
    &qJobClass
};

QUEUE_ID qJobClassId = &qJobClass;


