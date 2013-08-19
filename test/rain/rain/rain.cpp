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

// rain.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "includes.h"

#define MEM_LEN 1024*10

char memBuf[MEM_LEN];

/*extern PART_ID memSysPartId;*/
#ifdef __cplusplus
extern "C" {
#endif

extern void testQueue();

#ifdef __cplusplus
}
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	classLibInit();
	memPartLibInit(memBuf, MEM_LEN);

	char* a1 = (char*)memPartAlloc(memSysPartId, 10);
	char* a2 = (char*)memPartAlloc(memSysPartId, 45);
	memPartFree(memSysPartId, a1);
	memPartFree(memSysPartId, a2);

	a1 = (char*)memPartAlloc(memSysPartId, 10);
	a2 = (char*)memPartAlloc(memSysPartId, 45);

	memPartFree(memSysPartId, a2);
	memPartFree(memSysPartId, a1);

	a1 = (char*)memPartAlloc(memSysPartId, 10);
	a2 = (char*)memPartAlloc(memSysPartId, 12);
	char* a3 = (char*)memPartAlloc(memSysPartId, 45);

	memPartFree(memSysPartId, a2);

	char* a4 = (char*)memPartAlloc(memSysPartId, 12);

	testQueue();

	SEM_ID semId = semMCreate(0);

	int c = 0;
	semTake(semId, WAIT_FOREVER);
	c++;
	semGive(semId);

	semDelete(semId);

	gets(a1);
	return 0;
}

