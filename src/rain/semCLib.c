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

static BOOL	semCLibInstalled;

STATUS semCGive(SEM_ID semId);
void semCGiveDefer(SEM_ID semId);
STATUS semCTake(SEM_ID semId, int timeout);
STATUS semCCoreInit(SEMAPHORE *pSemaphore, int options, int initialCount);

STATUS semCLibInit (void)
{
    if (!semCLibInstalled)
	{
		semGiveTbl [SEM_TYPE_COUNTING]		= (FUNCTION) semCGive;
		semTakeTbl [SEM_TYPE_COUNTING]		= (FUNCTION) semCTake;
		semFlushTbl [SEM_TYPE_COUNTING]		= (FUNCTION) semQFlush;
		semGiveDeferTbl [SEM_TYPE_COUNTING]	= (FUNCTION) semCGiveDefer;
		semFlushDeferTbl [SEM_TYPE_COUNTING]	= (FUNCTION) semQFlushDefer;

		if (semLibInit () == OK)
		    semCLibInstalled = TRUE;
	}

    return ((semCLibInstalled) ? OK : ERROR);
}

SEM_ID semCCreate(int options, int initialCount)
{
    SEM_ID semId;

    if ((!semCLibInstalled) && (semCLibInit () != OK))	/* initialize package */
    {
		return (NULL);
    }

    if ((semId = (SEM_ID) objAlloc (semClassId)) == NULL)
    {
		return (NULL);
    }

    /* initialize allocated semaphore */

    if (semCInit (semId, options, initialCount) != OK)
	{
		objFree (semClassId, (char *) semId);
		return (NULL);
	}

    return (semId);
}


STATUS semCInit(SEMAPHORE *pSemaphore, int options, int initialCount)
{
    if ((!semCLibInstalled) && (semCLibInit () != OK))	/* initialize package */
    {
		return (ERROR);
    }

    if (semQInit (pSemaphore, options) != OK)		/* initialize queue */
    {
		return (ERROR);
    }

    return (semCCoreInit (pSemaphore, options, initialCount));
}


STATUS semCCoreInit(SEMAPHORE *pSemaphore, int options, int initialCount)
{
    if ((options & SEM_INVERSION_SAFE) || (options & SEM_DELETE_SAFE))
	{
		return (ERROR);
	}

    pSemaphore->semCount = initialCount;		/* initialize count */
    pSemaphore->recurse  = 0;				/* no recursive takes */
    pSemaphore->options  = options;			/* stow away options */
    pSemaphore->semType	 = SEM_TYPE_COUNTING;		/* type is counting */

    /* initialize the semaphore object core information */
    objCoreInit (&pSemaphore->objCore, semClassId);

    return (OK);
}


STATUS semCGive(SEM_ID semId)
{
	int level = intLock();

	if(!IS_CLASS(semId, semClassId))
	{
		intUnlock(level);
		return (ERROR);
	}

	if(NULL == Q_FIRST(&semId->qHead))
	{
		semId->semCount++;
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

void semCGiveDefer(SEM_ID semId)
{
	if(NULL == Q_FIRST(&semId->qHead))
	{
		semId->semCount++;
	}
	else
	{
		kernelPendQGet(&semId->qHead);
	}
}

STATUS semCTake(SEM_ID semId, int timeout)
{
	int level;
	STATUS status;

	/* ISR not allowed */
again:
	level = intLock();
	if(!IS_CLASS(semId, semClassId))
	{
		intUnlock(level);
		return (ERROR);
	}

	if(semId->semCount > 0)
	{
		semId->semCount--;
		intUnlock(level);
		return (OK);
	}

	kernelState = TRUE;
	intUnlock(level);

	if(kernelPendQPut(&semId->qHead, timeout) != OK)
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

