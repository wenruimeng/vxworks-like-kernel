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

static BOOL semLibInstalled = FALSE;
OBJ_CLASS semClass;
CLASS_ID semClassId = &semClass;

static STATUS semInvalid(SEM_ID semId);

FUNCTION semGiveTbl[MAX_SEM_TYPE] = 
	{
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid
	};

FUNCTION semTakeTbl[MAX_SEM_TYPE] = 
	{
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid
	};

FUNCTION semFlushTbl[MAX_SEM_TYPE] = 
	{
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid
	};

FUNCTION semGiveDeferTbl[MAX_SEM_TYPE] = 
	{
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid
	};

FUNCTION semFlushDeferTbl[MAX_SEM_TYPE] = 
	{
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,
		(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid,(FUNCTION)semInvalid
	};



STATUS semLibInit()
{
	if((!semLibInstalled) && (OK == classInit(semClassId, sizeof(SEMAPHORE), 
		OFFSET(SEMAPHORE,objCore), (FUNCTION)NULL, (FUNCTION)NULL, (FUNCTION)semDestroy)))
	{
		semLibInstalled = TRUE;
		return (ERROR);
	}

	return (semLibInstalled ? OK : ERROR);
}

STATUS semGive(SEM_ID semId)
{
	if(kernelState)
	{
		return semGiveDefer(semId);
	}
	else
	{
		return ((*semGiveTbl[semId->semType & SEM_TYPE_MASK])(semId));
	}
}

STATUS semTake(SEM_ID semId, int timeout)
{
	return ((*semTakeTbl[semId->semType & SEM_TYPE_MASK])(semId));
}

STATUS semFlush(SEM_ID semId)
{
	if(kernelState)
	{
		return semFlushDefer(semId);
	}
	else
	{
		return ((*semGiveTbl[semId->semType & SEM_TYPE_MASK])(semId));
	}
}

STATUS semDelete(SEM_ID semId)
{
	return (semDestroy(semId, TRUE));
}

STATUS semTerminal(SEM_ID semId)
{
	return (semDestroy(semId, FALSE));
}

STATUS semDestroy(SEM_ID semId, BOOL dealloc)
{
	int level;

	level = intLock();

	if(!IS_CLASS(semId, semClassId))
	{
		return (ERROR);
	}

	objCoreTerminate(&semId->objCore);

	kernelState = TRUE;

	intUnlock(level);

	kernelSemDelete(semId);

	TASK_SAFE();

	kernelExit();

	if(dealloc)
	{
		objFree(semClassId, (char*)semId);
	}
	
	TASK_UNSAFE();

	return (OK);
}

STATUS semGiveDefer(SEM_ID semId)
{
	if(!IS_CLASS(semId, semClassId))
	{
		return (ERROR);
	}

	if(NULL == semGiveDeferTbl[semId->semType & SEM_TYPE_MASK])
	{
		return (ERROR);
	}

	workQAdd1 (semGiveDeferTbl[semId->semType], (int) semId);

	return (OK);
}

STATUS semFlushDefer(SEM_ID semId)
{
	if(!IS_CLASS(semId, semClassId))
	{
		return (ERROR);
	}

	if(NULL == semFlushTbl[semId->semType & SEM_TYPE_MASK])
	{
		return (ERROR);
	}

	workQAdd1 (semFlushDeferTbl [semId->semType], (int) semId);

	return (OK);
}

STATUS semInvalid(SEM_ID semId)
{
	return (ERROR);
}

STATUS semQInit(SEMAPHORE* pSemaphore, int options)
{
	STATUS status = OK;

	switch(options & SEM_Q_MASK)
	{
	case SEM_Q_FIFO:
		qInit(&pSemaphore->qHead, Q_FIFO, 1,2,3,4,5);
		break;
		
	case SEM_Q_PRI:
		qInit(&pSemaphore->qHead, Q_PRI_LIST, 1,2,3,4,5);
		break;

	default:
		status = ERROR;
		break;
	}

	return status;
}

STATUS semQFlush(SEM_ID semId)
{
	int level = intLock();

	if(!IS_CLASS(semId, semClassId))
	{
		intUnlock(level);
	}

	if(NULL == Q_FIRST(&semId->qHead))
	{
		intUnlock(level);
	}
	else
	{
		kernelState = TRUE;
		intUnlock(level);

		kernelPendQFlush(&semId->qHead);
		kernelExit();
	}

	return (OK);
}

void semQFlushDefer(SEM_ID semId)
{
	
}
