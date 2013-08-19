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

#define WIND_JOBS_MAX	64

volatile BOOL workQIsEmpty = TRUE;	/* TRUE if it is empty */
volatile U8 workQReadIx;
volatile U8 workQWriteIx;
int pJobPool[WIND_JOBS_MAX * 4];

extern int errno;

void workQInit()
{
	workQIsEmpty = TRUE;
	workQReadIx = 0;
	workQWriteIx = 0;
}

void workQAdd0(FUNCTION func)
{
    int level = intLock ();		/* LOCK INTERRUPTS */
    FAST JOB *pJob = (JOB *) &pJobPool [workQWriteIx];

    workQWriteIx += 4;			/* advance write index */

    if (workQWriteIx == workQReadIx)
	workQPanic ();				/* leave interrupts locked */

    intUnlock (level);			/* UNLOCK INTERRUPTS */

    workQIsEmpty = FALSE;		/* we put something in it */

    pJob->function = func;		/* fill in function */
}

void workQAdd1(FUNCTION func, int arg1)
    {
    int level = intLock ();		/* LOCK INTERRUPTS */
    FAST JOB *pJob = (JOB *) &pJobPool [workQWriteIx];

    workQWriteIx += 4;			/* advance write index */

    if (workQWriteIx == workQReadIx)
	workQPanic ();				/* leave interrupts locked */

    intUnlock (level);			/* UNLOCK INTERRUPTS */

    workQIsEmpty = FALSE;		/* we put something in it */

    pJob->function = func;		/* fill in function */
    pJob->arg1 = arg1;			/* fill in argument */
    }

void workQAdd2(FUNCTION func, int arg1, int arg2)
    {
    int level = intLock ();		/* LOCK INTERRUPTS */
    FAST JOB *pJob = (JOB *) &pJobPool [workQWriteIx];

    workQWriteIx += 4;			/* advance write index */

    if (workQWriteIx == workQReadIx)
	workQPanic ();			/* leave interrupts locked */

    intUnlock (level);			/* UNLOCK INTERRUPTS */

    workQIsEmpty = FALSE;		/* we put something in it */

    pJob->function = func;		/* fill in function */
    pJob->arg1 = arg1;			/* fill in arguments */
    pJob->arg2 = arg2;
    }

void workQDoWork (void)
    {
    FAST JOB *pJob;
    int oldErrno = errno;			/* save errno */

    while (workQReadIx != workQWriteIx)
	{
        pJob = (JOB *) &pJobPool [workQReadIx];	/* get job */

	/* increment read index before calling function, because work function
	 * could be windTickAnnounce () that calls this routine as well.
	 */

		workQReadIx += 4;

        (FUNCTION *)(pJob->function) (pJob->arg1, pJob->arg2);

		workQIsEmpty = TRUE;			/* leave loop with empty TRUE */
	}

    errno = oldErrno;				/* restore _errno */
    }

void workQPanic (void)
{
		while(1);
}

