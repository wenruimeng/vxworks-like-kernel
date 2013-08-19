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
#ifndef __Q_JOB_LIB_H_
#define __Q_JOB_LIB_H_


/* qJobPut "priorities" */

#define Q_JOB_PRI_TAIL		0	/* add node to tail of queue */
#define Q_JOB_PRI_HEAD		1	/* add node to head of queue */
#define Q_JOB_PRI_DONT_CARE	1	/* head is a bit faster */


/* HIDDEN */

typedef struct qJobNode			/* Node of a job queue */
{
    struct qJobNode *next;
}Q_JOB_NODE;

typedef struct					/* Head of job queue */
{
    Q_JOB_NODE *first;			/* first node in queue */
    Q_JOB_NODE *last;			/* last node in queue */
    int		count;				/* number of nodes in queue */
    QUEUE_BASE    *pQClass;		/* must be 4th long word */
    Q_HEAD	pendQ;				/* queue of blocked tasks */
}Q_JOB_HEAD;

/* END HIDDEN */


extern QUEUE_ID qJobClassId;		/* job queue class */


#endif

