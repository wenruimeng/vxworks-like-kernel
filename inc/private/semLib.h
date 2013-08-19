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
#ifndef __SEM_LIB_H_
#define __SEM_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "sem.h"


typedef struct _semaphore
{
	OBJ_CORE objCore;
	U8 semType;
	U8 options;
	U16 recurse;
	Q_HEAD qHead;

	union
	{
		UINT count;
		struct _rainTcb* pOwner;
	}state;
	
}SEMAPHORE;

typedef struct _semaphore* SEM_ID;

#define semCount state.count
#define semOwner state.pOwner

extern FUNCTION semGiveTbl[];
extern FUNCTION semTakeTbl[];
extern FUNCTION semFlushTbl[];
extern FUNCTION semGiveDeferTbl[];
extern FUNCTION semFlushDeferTbl[];

extern CLASS_ID semClassId;

STATUS semLibInit();
STATUS semGive(SEM_ID semId);
STATUS semTake(SEM_ID semId, int timeout);
STATUS semFlush(SEM_ID semId);
STATUS semDelete(SEM_ID semId);
STATUS semTerminal(SEM_ID semId);
STATUS semDestroy(SEM_ID semId, BOOL dealloc);
STATUS semGiveDefer(SEM_ID semId);
STATUS semFlushDefer(SEM_ID semId);
STATUS semQInit(SEMAPHORE* pSemaphore, int options);
STATUS semQFlush(SEM_ID semId);
void semQFlushDefer(SEM_ID semId);

#ifdef __cplusplus
}
#endif

#endif

