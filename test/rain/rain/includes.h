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

#ifndef __INCLUDES_H_
#define __INCLUDES_H_

#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <stdio.h>
#include <tchar.h>
#include "windows.h"

#define U64 unsigned long long
#define U32 unsigned int
#define U16 unsigned short
#define U8  unsigned char
#define S64 signed long long
#define S32 int
#define S16 short int
#define S8  signed char
#define bool U8
#define UINT unsigned int
#define ULONG unsigned long
#define FAST register

/*typedef enum _status
{
	OK=0,
	ERROR=1
}STATUS;

typedef enum _bool
{
	FALSE=0,
	TRUE=1
}BOOL;*/

#define OK 0
#ifdef ERROR
#undef ERROR
#define ERROR 1
#endif
#define RESTART 2
typedef int STATUS;

typedef int (*FUNCTION)(void*);

#define LOCAL  static

#define OFFSET(structure, member)	/* byte offset of member in structure*/\
		((int) &(((structure *) 0) -> member))

#define MEMBER_SIZE(structure, member)	/* size of a member of a structure */\
		(sizeof (((structure *) 0) -> member))

#define NELEMENTS(array)		/* number of elements in an array */ \
		(sizeof (array) / sizeof ((array) [0]))

#define ROUND_UP(x, align)	(((int) (x) + (align - 1)) & ~(align - 1))
#define ROUND_DOWN(x, align)	((int)(x) & ~(align - 1))
#define ALIGNED(x, align)	(((int)(x) & (align - 1)) == 0)

#include "dllList.h"
#include "classLib.h"
#include "queueLib.h"
#include "qFifoLib.h"
#include "qPriListLib.h"
#include "semLib.h"
#include "memPartLib.h"

#endif