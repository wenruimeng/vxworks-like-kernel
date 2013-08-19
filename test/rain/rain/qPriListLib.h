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

#ifndef __Q_PRI_LIST_LIB_H_
#define __Q_PRI_LIST_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef DL_LIST Q_PRI_HEAD;

typedef struct
{
	DL_NODE node;	/* 0: double link node */
	ULONG key;		/* 8: */
}Q_PRI_NODE;

Q_PRI_HEAD* qPriListCreate();
STATUS qPriListInit(Q_PRI_HEAD* pQPriHead);
STATUS qPriListDelete(Q_PRI_HEAD* pQpriHead, Q_PRI_NODE* pQPriNode);
STATUS qPriListTerminate(Q_PRI_HEAD* pQPriHead);
void qPriListPut(Q_PRI_HEAD* pQPriHead, Q_PRI_NODE* pQPriNode, ULONG key);
Q_PRI_NODE* qPriListGet(Q_PRI_HEAD* pQPriHead);
STATUS qPriListRemove(Q_PRI_HEAD* pQPriHead, Q_PRI_NODE* pQPriNode);
void qPriListResort(FAST Q_PRI_HEAD* pQPriHead, FAST Q_PRI_NODE* pQPriNode, FAST ULONG newKey);
void qPriListAdvance(Q_PRI_HEAD* pQPriHead);
Q_PRI_NODE *qPriListGetExpired(Q_PRI_HEAD* pQPriHead);
void qPriListCalibrate(Q_PRI_HEAD *pQHead, ULONG keyDelta);
ULONG qPriListKey(Q_PRI_NODE* pQPriNode, int keyType);
Q_PRI_NODE* qPriListEach(Q_PRI_HEAD* pQPriHead, FUNCTION routine, int arg);

#ifdef __cplusplus
}
#endif

#endif
