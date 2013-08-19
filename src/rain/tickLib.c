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

UINT64 kernelAbsTicks;
ULONG kernelTicks = 0;

void tickAnnounce()
{
	if(kernelState)
	{
		workQAdd0(kernelTickAnnounce);
	}
	else
	{
		kernelState = TRUE;
		
		kernelTickAnnounce();

		kernelExit();
	}
}

void tickSet(ULONG ticks)
{
	int key = intLock();
	kernelAbsTicks = ticks;
	intUnlock(key);
}

ULONG tickGet()
{
	return (ULONG)(kernelAbsTicks & 0xFFFFFFFFull);
}
