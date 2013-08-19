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
#ifndef __SEM_C_LIB_H_
#define __SEM_C_LIB_H_

STATUS semCLibInit (void);
SEM_ID semCCreate(int options, int initialCount);
STATUS semCInit(SEMAPHORE *pSemaphore, int options, int initialCount);


#endif


