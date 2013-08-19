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
 
#ifndef __CLASS_LIB_H_
#define __CLASS_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _obj_core
{
    struct _obj_class *pObjClass;
} OBJ_CORE;


typedef struct _obj_class
{
	struct _obj_core objCore;

	struct _mem_part* *objPartId;
	unsigned objSize;
	unsigned objAllocCnt;
	unsigned objFreeCnt;
	unsigned objInitCnt;
	unsigned objTerminateCnt;
	
	int coreOffset;
	FUNCTION createRtn;
	FUNCTION initRtn;
	FUNCTION destroyRtn;
	FUNCTION showRtn;
}OBJ_CLASS;

typedef struct _obj_class *CLASS_ID;		/* CLASS_ID */
typedef struct _obj_core *OBJ_ID;

#define IS_CLASS(objId, classId)	(((OBJ_ID)objId)->pObjClass == (CLASS_ID)classId)

STATUS classLibInit();
STATUS classInit(OBJ_CLASS* pObjClass
	, unsigned objSize
	, int coreOffset
	, FUNCTION createRtn
	, FUNCTION initRtn
	, FUNCTION destroyRtn);
CLASS_ID classCreate(unsigned objSize
	, int coreOffset
	, FUNCTION createRtn
	, FUNCTION initRtn
	, FUNCTION destroyRtn);
STATUS classDestroy(CLASS_ID classId);

void objCoreInit(OBJ_CORE* pObjCore, OBJ_CLASS* pObjClass);

void* objAlloc(CLASS_ID classId);
void* objAllocExtra(FAST CLASS_ID classId, unsigned extraBytes, void** ppExtra);
STATUS objFree(CLASS_ID classId, void* pObj);
void objCoreTerminate(OBJ_CORE* pObjCore);


#ifdef __cplusplus
}
#endif

#endif

