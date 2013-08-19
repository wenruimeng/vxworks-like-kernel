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

LOCAL BOOL	semMLibInstalled;	/* protect from muliple inits */

STATUS semMGive(SEM_ID semId);
STATUS semMTake(SEM_ID semId);
STATUS semMDelete(SEM_ID semId, BOOL dealloc);

STATUS semMLibInit (void)
{
    if (!semMLibInstalled)
	{
	semGiveTbl [SEM_TYPE_MUTEX] = (FUNCTION) semMGive;
	semTakeTbl [SEM_TYPE_MUTEX] = (FUNCTION) semMTake;
	semDeleteTbl[SEM_TYPE_MUTEX] = (FUNCTION)semMDelete;

	if (semLibInit () == OK)
	    semMLibInstalled = TRUE;
	}

    return ((semMLibInstalled) ? OK : ERROR);
}

STATUS semMCoreInit
    (
    SEMAPHORE   *pSemaphore,    /* pointer to mutex semaphore to initialize */
    int         options         /* mutex semaphore options */
    )
    {
    pSemaphore->semType  = SEM_TYPE_MUTEX;	/* type is mutex */

    objCoreInit (&pSemaphore->objCore, semClassId);

    return (OK);
    }

SEM_ID semMCreate(int options)
{
	SEM_ID semId;

	if ((!semMLibInstalled) && (semMLibInit () != OK))	/* initialize package */
		return (ERROR);

	if ((semId = (SEM_ID) objAllocExtra(semClassId, sizeof(CRITICAL_SECTION), NULL)) == NULL)
		return (NULL);

	semId->extraBytes = (char*)semId + (OFFSET(SEMAPHORE, extraBytes) + sizeof(void*));

	if(semMLibInit(semId) != OK)
	{
		objFree(semClassId, (char *) semId);
		return (ERROR);
	}

	return semId;
}

STATUS semMInit(SEMAPHORE* pSemaphore, int options)
{
	if ((!semMLibInstalled) && (semMLibInit () != OK))	/* initialize package */
		return (ERROR);

	if(NULL == pSemaphore->extraBytes)
	{
		pSemaphore->extraBytes = (char*)memPartAlloc(memSysPartId, sizeof(CRITICAL_SECTION));
	}

	pSemaphore->handle = pSemaphore->extraBytes;

	InitializeCriticalSection(pSemaphore->handle);


}

STATUS semMGive(SEM_ID semId)
{
	/*if(!IS_CLASS(semId, semClassId))
	{
		return (ERROR)
	}*/

	LeaveCriticalSection((LPCRITICAL_SECTION)semId->handle);
	return (OK);
}

STATUS semMTake(SEM_ID semId)
{
	EnterCriticalSection((LPCRITICAL_SECTION)semId->handle);
	return (OK);
}

STATUS semMDelete(SEM_ID semId, BOOL dealloc)
{
	CloseHandle(semId->handle);
	
	if(semId->extraBytes != (char*)semId + (OFFSET(SEMAPHORE, extraBytes) + sizeof(void*)))
	{
		memPartFree(memSysPartId, semId->extraBytes);
	}

	if(dealloc)
	{
		objFree (semClassId, (char *) semId);
	}
}