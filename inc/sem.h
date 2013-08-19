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
 
#ifndef __SEM_H_
#define __SEM_H_

#define MAX_SEM_TYPE 		8
#define SEM_TYPE_MASK		0x7
#define SEM_Q_MASK			0x3

#define SEM_Q_FIFO			0
#define SEM_Q_PRI			1
#define SEM_DELETE_SAFE		 0x04	/* owner delete safe (mutex opt.) */
#define SEM_INVERSION_SAFE	 0x08	/* no priority inversion (mutex opt.) */


#define SEM_TYPE_BINARY		0x0	/* binary semaphore */
#define SEM_TYPE_MUTEX		0x1	/* mutual exclusion semaphore */
#define SEM_TYPE_COUNTING	0x2	/* counting semaphore */

#define SEM_M_Q_GET			0x1		/* semMGiveKernWork() defines */
#define SEM_M_SAFE_Q_FLUSH	0x2
#define SEM_M_PRI_RESORT	0x4
#define SEM_M_SEND_EVENTS	0x8

typedef enum
{
	SEM_EMPTY = 0,
	SEM_FULL
} SEM_B_STATE;

#endif

