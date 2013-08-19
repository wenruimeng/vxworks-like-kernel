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

LOCAL BOOL	semCLibInstalled;		/* protect from muliple inits */

STATUS semCCoreInit
    (
    SEMAPHORE   *pSemaphore,            /* pointer to semaphore to init */
    int         options,                /* semaphore options */
    int         initialCount            /* initial count */
    );
STATUS semCGive(SEM_ID semId);
STATUS semCTake(SEM_ID semId, int timeout);
STATUS semCDelete(SEM_ID semId, BOOL dealloc);

STATUS semCLibInit(void)
{
    if (!semCLibInstalled)
	{
	semGiveTbl[SEM_TYPE_COUNTING] = (FUNCTION) semCGive;
	semTakeTbl[SEM_TYPE_COUNTING] = (FUNCTION) semCTake;
	semDeleteTbl[SEM_TYPE_COUNTING] = (FUNCTION)semCDelete;

	if (semLibInit () == OK)
	    semCLibInstalled = TRUE;
	}

    return ((semCLibInstalled) ? OK : ERROR);
}

SEM_ID semCCreate(int options, int initialCount)
{
	SEM_ID semId;

    if ((!semCLibInstalled) && (semCLibInit () != OK))	/* initialize package */
		return (NULL);

	if ((semId = (SEM_ID) objAlloc (semClassId)) == NULL)
		return (NULL);

	if (semCInit (semId, options, initialCount) != OK)
	{
		objFree(semClassId, (char *) semId);
		return (NULL);
	}

	return (semId);
}

STATUS semCCoreInit
    (
    SEMAPHORE   *pSemaphore,            /* pointer to semaphore to init */
    int         options,                /* semaphore options */
    int         initialCount            /* initial count */
    )
    {
    pSemaphore->semType	 = SEM_TYPE_COUNTING;		/* type is counting */

    /* initialize the semaphore object core information */
    objCoreInit (&pSemaphore->objCore, semClassId);

    return (OK);
    }

STATUS semCInit
    (
    SEMAPHORE   *pSemaphore,            /* pointer to semaphore to init */
    int         options,                /* semaphore options */
    int         initialCount            /* initial count */
    )
    {
    if ((!semCLibInstalled) && (semCLibInit () != OK))	/* initialize package */
		return (ERROR);

	pSemaphore->handle = CreateSemaphore(NULL, 0, initialCount, NULL);

    return (semCCoreInit (pSemaphore, options, initialCount));
    }

STATUS semCGive(SEM_ID semId)
{
	if(ReleaseSemaphore(semId->handle, 1, NULL))
	{
		return (OK);
	}

	return (ERROR);
}

STATUS semCTake(SEM_ID semId, int timeout)
{
	if(WAIT_OBJECT_0 == WaitForSingleObject(semId->handle, timeout))
	{
		return (OK);
	}

	return (ERROR);
}

STATUS semCDelete(SEM_ID semId, BOOL dealloc)
{
	CloseHandle(semId->handle);
	if(dealloc)
	{
		objFree (semClassId, (char *) semId);
	}
}
