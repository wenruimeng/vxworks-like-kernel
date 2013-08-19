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

int semMGiveKernWork = 0;

static BOOL	semMLibInstalled;	/* protect from muliple inits */

STATUS semMPendQPut(FAST SEM_ID semId, int timeout);
STATUS semMGive(FAST SEM_ID semId);
STATUS semMTake(FAST SEM_ID semId, int timeout);
STATUS semMCoreInit(SEMAPHORE *pSemaphore, int options);
STATUS semMGiveKern(FAST SEM_ID semId);

STATUS semMLibInit (void)
{
    if (!semMLibInstalled)
	{
		semGiveTbl [SEM_TYPE_MUTEX]		= (FUNCTION)semMGive;
		semTakeTbl [SEM_TYPE_MUTEX]		= (FUNCTION)semMTake;

		if (semLibInit () == OK)
		{
	    	semMLibInstalled = TRUE;
		}
	}

    return ((semMLibInstalled) ? OK : ERROR);
}

SEM_ID semMCreate(int options)
{
    SEM_ID semId;

    if ((!semMLibInstalled) && (semMLibInit () != OK))	/* initialize package */
    {
        return (NULL);
    }

    if ((semId = (SEM_ID) objAlloc (semClassId)) == NULL)
    {
		return (NULL);
    }

    /* initialize allocated semaphore */

    if (semMInit (semId, options) != OK)
	{
		objFree (semClassId, (char *) semId);
		return (NULL);
	}

    return (semId);
}


STATUS semMInit(SEMAPHORE* pSemaphore, int options)
{
	if((!semMLibInstalled) && (semMLibInit () != OK))
	{
		return (ERROR);
	}

    if ((options & SEM_INVERSION_SAFE) && ((options & SEM_Q_MASK)==SEM_Q_FIFO))
	{
		return (ERROR);
	}

    if (semQInit (pSemaphore, options) != OK)
    {
		return (ERROR);
    }

    return (semMCoreInit (pSemaphore, options));
}

STATUS semMCoreInit(SEMAPHORE *pSemaphore, int options)
{
    pSemaphore->semOwner = NULL;		/* no owner */
    pSemaphore->recurse	 = 0;			/* init take recurse count */
    pSemaphore->options	 = options;		/* initialize options */
    pSemaphore->semType  = SEM_TYPE_MUTEX;	/* type is mutex */

    objCoreInit (&pSemaphore->objCore, semClassId);

    return (OK);
}


/* TODO 
STATUS semMGiveForce(FAST SEM_ID semId);*/

STATUS semMGive(FAST SEM_ID semId)
{
	FAST int level;
	FAST int kernelWork = 0;

	/* TODO int not allowed */

	level = intLock();

	if(!IS_CLASS(semId, semClassId))
	{
		intUnlock(level);
		return (ERROR);
	}

	if(taskCurrent != semId->semOwner)
	{
		intUnlock(level);
		return (ERROR);
	}

	if(semId->recurse > 0)
	{
		semId->recurse--;
		intUnlock(level);
		return (OK);
	}

	if((semId->options & SEM_INVERSION_SAFE) && 
		(--taskCurrent->priMutexCnt == 0))
	{
		kernelWork |= SEM_M_PRI_RESORT;
	}

	/* change the semOwner hear */
	if((semId->semOwner = (RAIN_TCB*)Q_FIRST(&semId->qHead)) != NULL)
	{
		kernelWork |= SEM_M_Q_GET;
	}

	if((semId->options & SEM_DELETE_SAFE) && 
		(--taskCurrent->safeCnt == 0) &&
		(Q_FIRST(&taskCurrent->safetyQHead) != NULL))
	{
		kernelWork |= SEM_M_SAFE_Q_FLUSH;
	}

	if(kernelWork == 0)
	{
		intUnlock(level);
		return (OK);
	}

	kernelState = TRUE;
	intUnlock(level);

	semMGiveKernWork = kernelWork;

	return (semMGiveKern(semId));
}

STATUS semMTake(FAST SEM_ID semId, int timeout)
{
	int level;
	int status;

	/* TODO no int */

again:
	level = intLock();

	if(!IS_CLASS(semId, semClassId))
	{
		intUnlock(level);
		return (ERROR);
	}

	if(NULL == semId->semOwner)
	{
		semId->semOwner = taskCurrent;

		if(semId->options & SEM_INVERSION_SAFE)
		{
			taskCurrent->priMutexCnt++;
		}

		if(semId->options & SEM_DELETE_SAFE)
		{
			taskCurrent->safeCnt++;
		}

		intUnlock(level);
		return (OK);
	}

	if(semId->semOwner == taskCurrent)
	{
		semId->recurse++;
		intUnlock(level);
		return (OK);
	}

	kernelState = TRUE;
	intUnlock(level);

	if(OK != semMPendQPut(semId, timeout))
	{
		kernelExit();

		return (ERROR);
	}

	if((status == kernelExit()) == RESTART)
	{
		goto again;
	}

	return status;
}

STATUS semMGiveKern(FAST SEM_ID semId)
{
	if(semMGiveKernWork & SEM_M_Q_GET)
	{
		kernelPendQGet(&semId->qHead);
		semId->semOwner->pPriMutex = NULL;

		if(semId->options & SEM_DELETE_SAFE)
		{
			semId->semOwner->safeCnt++;
		}

		if(semId->options & SEM_INVERSION_SAFE)
		{
			semId->semOwner->priMutexCnt++;
		}
	}

	if(semMGiveKernWork & SEM_M_PRI_RESORT)
	{
		kernelPrioritySet(taskCurrent, taskCurrent->priNormal);
	}

	if(semMGiveKernWork & SEM_M_SAFE_Q_FLUSH)
	{
		kernelPendQFlush(&taskCurrent->safetyQHead);
	}

	return kernelExit();
}

STATUS semMPendQPut(FAST SEM_ID semId, int timeout)
{
	if(OK != kernelPendQPut(&semId->qHead, timeout))
	{
		return (ERROR);
	}

	if(semId->options & SEM_INVERSION_SAFE)
	{
		taskCurrent->pPriMutex = semId;

		if(taskCurrent->priority < semId->semOwner->priority)
		{
			kernelPrioritySet(semId->semOwner, taskCurrent->priority);
		}
	}

	return (OK);
}
