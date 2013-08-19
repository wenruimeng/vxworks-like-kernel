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

#ifndef __TASK_H_
#define __TASK_H_


#define _STACK_ALIGN_SIZE		16
#define STACK_ROUND_UP(x)	ROUND_UP(x, _STACK_ALIGN_SIZE)
#define STACK_ROUND_DOWN(x)	ROUND_DOWN(x, _STACK_ALIGN_SIZE)

#define KERNEL_UNBREAKABLE	  	0x0002	/* INTERNAL: breakpoints ignored */
#define KERNEL_DEALLOC_STACK  	0x0004	/* INTERNAL: deallocate stack */

#define KERNEL_READY		0x00	/* ready to run */
#define KERNEL_SUSPEND		0x01	/* explicitly suspended */
#define KERNEL_PEND			0x02	/* pending on semaphore */
#define KERNEL_DELAY		0x04	/* task delay (or timeout) */
#define KERNEL_DEAD			0x08	/* dead task */

#define WAIT_FOREVER 	-1
#define NO_WAIT 		0

#define MAX_TASK_ARGS 5

extern CLASS_ID taskClassId;
extern int taskIdCurrent;

/* TODO */
/*#define TASK_LOCK()							     						\
	do										 							\
	{																	\
		char* __tmp = (char*)taskIdCurrent+KERNEL_TCB_LOCK_CNT;			\
		*((volatile UINT*)__tmp)++;										\
	} while (0)

#define TASK_UNLOCK()	taskUnlock ()

#define TASK_SAFE()							 							\
	do										 							\
	{																	\
		char* __tmp = (char*)taskIdCurrent+KERNEL_TCB_SAFE_CNT;			\
		*((volatile UINT*)__tmp)++;										\
	} while (0)

#define TASK_UNSAFE()	taskUnsafe ()*/

/* move it to somewhere else */
extern UINT intCnt;
#define INT_CONTEX()	(intCnt > 0)


#endif 

