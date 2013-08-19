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
#ifndef __SEM_M_LIB_H_
#define __SEM_M_LIB_H_

STATUS semMLibInit (void);
SEM_ID semMCreate(int options);
STATUS semMInit(SEMAPHORE* pSemaphore, int options);

#endif
