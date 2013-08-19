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

STATUS qInit(Q_HEAD* pQHead, QUEUE_BASE* pQBase, int arg1, int arg2, int arg3, int arg4, int arg5)
{
	if(!IS_QUEUE(pQBase))
	{
		return (ERROR);
	}

	pQHead->qBase = pQBase;

	return ( (*(pQBase->initRtn))(pQHead, arg1, arg2, arg3, arg4, arg5) );
}


