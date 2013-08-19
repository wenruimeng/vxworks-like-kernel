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

#ifndef __SEM_B_LIB_H__
#define __SEM_B_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

STATUS semBLibInit();
SEM_ID semBCreate(int options, SEM_B_STATE initialState);

STATUS semBInit(SEMAPHORE* pSemaphore, int options, SEM_B_STATE initialState);
STATUS semBGive(SEM_ID semId);
STATUS semBTake(SEM_ID semId, int timeOut);
void semBGiveDefer(SEM_ID semId);

#ifdef __cplusplus
}
#endif

#endif

