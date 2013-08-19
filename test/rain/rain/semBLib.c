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

LOCAL BOOL	semBLibInstalled;		/* protect from muliple inits */

STATUS semBCoreInit();
STATUS semBGive(SEM_ID semId);
STATUS semBTake(SEM_ID semId, int timeout);
STATUS semBDelete(SEM_ID semId, BOOL dealloc);

STATUS semBLibInit (void)
{
    if (!semBLibInstalled)
	{
	semGiveTbl[SEM_TYPE_BINARY] = (FUNCTION) semBGive;
	semTakeTbl[SEM_TYPE_BINARY] = (FUNCTION) semBTake;
	semDeleteTbl[SEM_TYPE_BINARY] = (FUNCTION)semBDelete;

	if (semLibInit () == OK)
	    semBLibInstalled = TRUE;
	}

    return ((semBLibInstalled) ? OK : ERROR);
}

SEM_ID semBCreate(int options, SEM_B_STATE initialState)
{
	SEM_ID semId;

    if ((!semBLibInstalled) && (semBLibInit () != OK))	/* initialize package */
	return (NULL);

    if ((semId = (SEM_ID) objAlloc (semClassId)) == NULL)
	return (NULL);

    /* initialize allocated semaphore */

    if (semBInit (semId, options, initialState) != OK)
	{
	objFree (semClassId, (char *) semId);
	return (NULL);
	}

    return (semId);
}

STATUS semBInit
    (
    SEMAPHORE  *pSemaphore,             /* pointer to semaphore to initialize */
    int         options,                /* semaphore options */
    SEM_B_STATE initialState            /* initial semaphore state */
    )
    {
    if ((!semBLibInstalled) && (semBLibInit () != OK))	/* initialize package */
	return (ERROR);

	pSemaphore->handle = CreateEvent(NULL, FALSE, initialState, NULL);

    return (semBCoreInit (pSemaphore, options, initialState));
    }

STATUS semBCoreInit
    (
    SEMAPHORE  *pSemaphore,             /* pointer to semaphore to initialize */
    int         options,                /* semaphore options */
    SEM_B_STATE initialState            /* initial semaphore state */
    )
    {
    pSemaphore->semType = SEM_TYPE_BINARY;		/* type is binary */

    objCoreInit (&pSemaphore->objCore, semClassId); /* initialize core */

    return (OK);
    }

STATUS semBGive(SEM_ID semId)
{
	if(SetEvent(semId->handle))
	{
		return (OK);
	}

	return (ERROR);
}

STATUS semBTake(SEM_ID semId, int timeout)
{
	if(WAIT_OBJECT_0 == WaitForSingleObject(semId->handle, timeout))
	{
		return (OK);
	}

	return (ERROR);
}

STATUS semBDelete(SEM_ID semId, BOOL dealloc)
{
	CloseHandle(semId->handle);
	if(dealloc)
	{
		objFree (semClassId, (char *) semId);
	}
}