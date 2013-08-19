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

OBJ_CLASS classClass;
CLASS_ID classClassId = &classClass;

STATUS classLibInit()
{
	classInit(classClassId
		, sizeof(OBJ_CLASS)
		, OFFSET(OBJ_CLASS, objCore)
		, (FUNCTION)classCreate
		, (FUNCTION)classInit
		, (FUNCTION)classDestroy);
	
	return (OK);
}

STATUS classInit(OBJ_CLASS* pObjClass
	, unsigned objSize
	, int coreOffset
	, FUNCTION createRtn
	, FUNCTION initRtn
	, FUNCTION destroyRtn)
{
	pObjClass->objPartId = memSysPartId;
	pObjClass->objSize = objSize;
	pObjClass->coreOffset = coreOffset;
	pObjClass->createRtn = createRtn;
	pObjClass->initRtn = initRtn;
	pObjClass->destroyRtn = destroyRtn;

	/* The class is an object of classClass */
	objCoreInit(&pObjClass->objCore, classClassId);
	
	return (OK);
}

CLASS_ID classCreate(unsigned objSize
	, int coreOffset
	, FUNCTION createRtn
	, FUNCTION initRtn
	, FUNCTION destroyRtn)
{
	CLASS_ID classId = (CLASS_ID) objAlloc(classClassId);
	
	if(NULL != classId)
	{
		classInit(classId, objSize, coreOffset, createRtn, initRtn, destroyRtn);
	}

	return classId;
}

STATUS classDestroy(CLASS_ID classId)
{
	return (ERROR);
}

void objCoreInit(OBJ_CORE* pObjCore, OBJ_CLASS* pObjClass)
{
	pObjCore->pObjClass = pObjClass;
	pObjClass->objInitCnt++;
}

void* objAlloc(FAST CLASS_ID classId)
{
	return objAllocExtra(classId, 0, (void**)NULL);
}

void* objAllocExtra(FAST CLASS_ID classId, unsigned extraBytes, void** ppExtra)
{
	void* pObj;

	/* TODO cannot call from isr */
	if(!IS_CLASS(classId, classClassId))
	{
		return NULL;
	}

	pObj = memPartAlloc(classId->objPartId, classId->objSize+extraBytes);
	/*printf("pObj:%x %d %s\n", pObj, __LINE__, __FUNCTION__);*/
	if(NULL != pObj)
	{
		classId->objAllocCnt++;
		if(NULL != ppExtra)
		{
			*ppExtra = (void*)((char*)pObj + classId->objSize);
		}
	}

	return pObj;
}

STATUS objFree(CLASS_ID classId, void* pObj)
{
	if((!IS_CLASS(pObj, classId))
		|| (OK != memPartFree(classId->objPartId, pObj)))
	{
		return (ERROR);
	}

	classId->objFreeCnt++;

	return (OK);
}

void objCoreTerminate(OBJ_CORE* pObjCore)
{
	pObjCore->pObjClass->objTerminateCnt++;
	pObjCore->pObjClass = NULL;
}
