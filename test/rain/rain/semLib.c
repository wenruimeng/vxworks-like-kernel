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

STATUS semInvalid(SEM_ID semId);

FUNCTION semGiveTbl [MAX_SEM_TYPE] =		/* semGive() methods by type */
    {
    (FUNCTION) semInvalid, (FUNCTION) semInvalid, (FUNCTION) semInvalid,
    (FUNCTION) semInvalid, (FUNCTION) semInvalid, (FUNCTION) semInvalid,
    (FUNCTION) semInvalid, (FUNCTION) semInvalid
    };

FUNCTION semTakeTbl [MAX_SEM_TYPE] =		/* semTake() methods by type */
    {
    (FUNCTION) semInvalid, (FUNCTION) semInvalid, (FUNCTION) semInvalid,
    (FUNCTION) semInvalid, (FUNCTION) semInvalid, (FUNCTION) semInvalid,
    (FUNCTION) semInvalid, (FUNCTION) semInvalid
    };

FUNCTION semDeleteTbl [MAX_SEM_TYPE] =		/* semTake() methods by type */
    {
    (FUNCTION) semInvalid, (FUNCTION) semInvalid, (FUNCTION) semInvalid,
    (FUNCTION) semInvalid, (FUNCTION) semInvalid, (FUNCTION) semInvalid,
    (FUNCTION) semInvalid, (FUNCTION) semInvalid
    };

/* locals */

LOCAL BOOL	semLibInstalled;		/* protect from muliple inits */

/* globals */

OBJ_CLASS semClass;				/* semaphore object class */
CLASS_ID  semClassId = &semClass;		/* semaphore class ID */

STATUS semDestroy(SEM_ID semId, BOOL dealloc);

STATUS semLibInit (void)
    {
    if ((!semLibInstalled) &&
	(classInit (semClassId, sizeof(SEMAPHORE), OFFSET(SEMAPHORE,objCore),
         (FUNCTION) NULL, (FUNCTION) NULL, (FUNCTION) semDestroy) == OK))
	{
	semLibInstalled = TRUE;
	}

    return ((semLibInstalled) ? OK : ERROR);
    }

STATUS semTake
    (
    SEM_ID semId,       /* semaphore ID to take */
    int timeout         /* timeout in ticks */
    )
	{
		if(!IS_CLASS(semId, semClassId))
		{
			return (ERROR);
		}
		return ((* (semTakeTbl[semId->semType & SEM_TYPE_MASK])) (semId, timeout));
	}

STATUS semGive
    (
    SEM_ID semId        /* semaphore ID to give */
    )
	{
		if(!IS_CLASS(semId, semClassId))
		{
			return (ERROR);
		}
		return((* semGiveTbl[semId->semType & SEM_TYPE_MASK]) (semId));
	}

STATUS semDelete
    (
    SEM_ID semId        /* semaphore ID to delete */
    )
    {
    return (semDestroy (semId, TRUE));
    }

STATUS semDestroy(SEM_ID semId, BOOL dealloc)
{
	if(!IS_CLASS(semId, semClassId))
	{
		return (ERROR);
	}
	return((* semDeleteTbl[semId->semType & SEM_TYPE_MASK]) (semId, dealloc));
}

STATUS semInvalid
    (
    SEM_ID semId        /* semId of invalid operation */
    )
    {
    if (!IS_CLASS (semId, semClassId))		/* verify semaphore */
	return (ERROR);

    return (ERROR);
    }