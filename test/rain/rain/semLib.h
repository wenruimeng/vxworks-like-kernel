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

#ifndef _SEM_LIB_H_
#define _SEM_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_SEM_TYPE	8

#define SEM_TYPE_BINARY		0x0	/* binary semaphore */
#define SEM_TYPE_MUTEX		0x1	/* mutual exclusion semaphore */
#define SEM_TYPE_COUNTING	0x2	/* counting semaphore */
#define SEM_TYPE_MASK		0x7

#define SEM_Q_FIFO	1
#define SEM_FULL	1
#define SEM_EMPTY	0
#define WAIT_FOREVER INFINITE

#define SEM_B_STATE int

typedef struct _SEMAPHORE
{
	OBJ_CORE objCore;
	int semType;
	void* handle;
	char* extraBytes;
}SEMAPHORE;

typedef struct _SEMAPHORE *SEM_ID;

extern FUNCTION semGiveTbl [MAX_SEM_TYPE];
extern FUNCTION semTakeTbl [MAX_SEM_TYPE];
extern FUNCTION semDeleteTbl [MAX_SEM_TYPE];

extern CLASS_ID  semClassId;

STATUS semBLibInit();
STATUS semCLibInit();
STATUS semMLibInit();
SEM_ID semBCreate(int options, SEM_B_STATE initialState);
STATUS semBInit(SEM_ID semId, int options, SEM_B_STATE initialState);
SEM_ID semCCreate(int options, int initialCount);
STATUS semCInit(SEM_ID semId, int options, int initialCount);
SEM_ID semMCreate(int options);
STATUS semMInit(SEM_ID semId, int options);
STATUS semTake(SEM_ID semId, int timeout);
STATUS semGive(SEM_ID semId);
STATUS semDelete(SEM_ID semId);

#ifdef __cplusplus
}
#endif

#endif
