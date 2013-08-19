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

BOOL semBLibInstalled = FALSE;

static STATUS semBCoreInit(SEMAPHORE* pSemaphore, int options, SEM_B_STATE initialState);

STATUS semBLibInit()
{
	if(!semBLibInstalled)
	{
		semGiveTbl[SEM_TYPE_BINARY] = (FUNCTION)semBGive;
		semTakeTbl[SEM_TYPE_BINARY] = (FUNCTION)semBTake;
		semFlushTbl[SEM_TYPE_BINARY] = (FUNCTION)semQFlush;
		semGiveDeferTbl[SEM_TYPE_BINARY] = (FUNCTION)semBGiveDefer;
		semFlushDeferTbl[SEM_TYPE_BINARY] = (FUNCTION)semQFlushDefer;
	}

	if(OK == semLibInit())
	{
		semBLibInstalled = TRUE;
	}

	return (semBLibInstalled ? OK : ERROR);
}

SEM_ID semBCreate
(
    int         options,                /* semaphore options */
    SEM_B_STATE initialState            /* initial semaphore state */
)
{
    SEM_ID semId;

    if ((!semBLibInstalled) && (semBLibInit () != OK))	/* initialize package */
    {
		return (NULL);
    }

    if ((semId = (SEM_ID) objAlloc (semClassId)) == NULL)
    {
		return (NULL);
    }

    /* initialize allocated semaphore */

    if (semBInit (semId, options, initialState) != OK)
	{
		objFree (semClassId, (char *) semId);
		return (NULL);
	}

    return (semId);
}


STATUS semBInit(SEMAPHORE* pSemaphore, int options, SEM_B_STATE initialState)
{
	if((!semBLibInstalled)&&(OK!=semBLibInit()))
	{
		return (ERROR);
	}

	if(OK != semQInit(pSemaphore, options))
	{
		return (ERROR);
	}

	return semBCoreInit(pSemaphore, options, initialState);
}

static STATUS semBCoreInit(SEMAPHORE* pSemaphore, int options, SEM_B_STATE initialState)
{
	switch(initialState)
	{
	case SEM_EMPTY:
		pSemaphore->semOwner = taskCurrent;
		break;
	case SEM_FULL:
		pSemaphore->semOwner = NULL;
		break;
	default:
		return (ERROR);
	}

	pSemaphore->recurse = 0;
	pSemaphore->options = options;
	pSemaphore->semType = SEM_TYPE_BINARY;

	objCoreInit(&pSemaphore->objCore, semClassId);

	return (OK);
}

STATUS semBGive(SEM_ID semId)
{
	/* RAIN_TCB* pOwner; */
	int level = intLock();
	
	if(!IS_CLASS(semId, semClassId))
	{
		intUnlock(level);
		return (ERROR);
	}

	/*pOwner = semId->semOwner;*/

	if(NULL == (semId->semOwner = (RAIN_TCB*)Q_FIRST(&semId->qHead)))
	{
		intUnlock(level);
	}
	else
	{
		kernelState = TRUE;
		intUnlock(level);

		kernelPendQGet(&semId->qHead);

		kernelExit();
	}

	return (OK);
}

STATUS semBTake(SEM_ID semId, int timeout)
{
	int level;
	int status;

	/* TODO ISR no be allowed */

again:
	level = intLock();

	if(!IS_CLASS(semId, semClassId))
	{
		intUnlock(level);
		return (ERROR);
	}

	if(NULL == semId->semOwner)
	{
		semId->semOwner = (RAIN_TCB*)taskIdCurrent;
		intUnlock(level);

		return (OK);
	}

	/* semId have been take by other task */
	kernelState = TRUE;
	intUnlock(level);

	if(OK != kernelPendQPut(&semId->qHead, timeout))
	{
		kernelExit();

		return (ERROR);
	}

	/* test for interrupt in kernelState=TRUE of semGive */
	/*{
		int j = 1000000;
		while(j >= 0)
		{
			j--;
		}
	}*/
	
	if(RESTART == (status = kernelExit()))
	{
		goto again;
	}

	return (status);
}

void semBGiveDefer(SEM_ID semId)
{
	if(NULL != (semId->semOwner = (RAIN_TCB*)Q_FIRST(&semId->qHead)))
	{
		kernelPendQGet(&semId->qHead);
	}
}
