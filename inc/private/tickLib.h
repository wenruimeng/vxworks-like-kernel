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
#ifndef __TICKLIB_H_
#define __TICKLIB_H_

void tickAnnounce();
void tickSet(ULONG ticks);
ULONG tickGet();

extern UINT64 kernelAbsTicks;
extern ULONG kernelTicks;

#endif
