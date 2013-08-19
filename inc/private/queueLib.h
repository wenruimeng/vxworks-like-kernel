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

#ifndef __QUEUE_LIB_H_
#define __QUEUE_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

/* as like a c++ abstract class */
typedef struct _queue_base
{
	FUNCTION createRtn;
	FUNCTION initRtn;
	FUNCTION deleteRtn;
	FUNCTION terminalRtn;
	FUNCTION putRtn;
	FUNCTION getRtn;
	FUNCTION removeRtn;
	FUNCTION resortRtn;
	FUNCTION advanceRtn;
	FUNCTION getExpiredRtn;
	FUNCTION keyRtn;
	FUNCTION calibrateRtn;
	FUNCTION eachRtn;
	struct _queue_base* qBase;
}QUEUE_BASE;

typedef QUEUE_BASE* QUEUE_ID;

extern QUEUE_ID qFifoQueueId;
extern QUEUE_ID qPriListId;

#define Q_FIFO 			qFifoQueueId
#define Q_PRI_LIST 		qPriListId

/* 16 Bytes */
typedef struct _q_node
{
	UINT qPriv1;
	UINT qPriv2;
	UINT qPriv3;
	UINT qPriv4;
}Q_NODE;

typedef struct _q_head
{
	Q_NODE* pFirstNode;
	UINT qPriv1;
	UINT qPriv2;
	QUEUE_BASE* qBase;
}Q_HEAD;

#define IS_QUEUE(pQBase)	((pQBase)->qBase == (pQBase))

#define Q_FIRST(pQHead)							\
    ((Q_NODE *)(((Q_HEAD *)(pQHead))->pFirstNode))

#define Q_PUT(pQHead,pQNode,key)					\
		(*(((Q_HEAD *)(pQHead))->qBase->putRtn))				\
		(((Q_HEAD *)(pQHead)), ((Q_NODE *)(pQNode)), (key))

#define Q_GET(pQHead)							\
    ((Q_NODE *)((*(((Q_HEAD *)(pQHead))->qBase->getRtn))		\
     ((Q_HEAD *)(pQHead))))

#define Q_REMOVE(pQHead,pQNode)						\
    (*(((Q_HEAD *)(pQHead))->qBase->removeRtn))			\
    (((Q_HEAD *)(pQHead)), ((Q_NODE *)(pQNode)))

#define Q_RESORT(pQHead,pQNode,newKey)					\
    (*(((Q_HEAD *)(pQHead))->qBase->resortRtn))			\
    (((Q_HEAD *)(pQHead)), ((Q_NODE *)(pQNode)), ((ULONG)(newKey)))

#define Q_ADVANCE(pQHead)						\
    (*(((Q_HEAD *)(pQHead))->qBase->advanceRtn))			\
    (((Q_HEAD *)(pQHead)))

#define Q_GET_EXPIRED(pQHead)						\
    ((Q_NODE *)((*(((Q_HEAD *)(pQHead))->qBase->getExpiredRtn))	\
     ((Q_HEAD *)(pQHead))))

#define Q_KEY(pQHead,pQNode,keyType)					\
    (*(((Q_HEAD *)(pQHead))->qBase->keyRtn))				\
    (((Q_NODE *)(pQNode)), ((int)(keyType)))

#define Q_CALIBRATE(pQHead,keyDelta)					\
    (*(((Q_HEAD *)(pQHead))->qBase->calibrateRtn))			\
    (((Q_HEAD *)(pQHead)), ((int)(keyDelta)))

#define Q_EACH(pQHead,routine,routineArg)				\
    ((Q_NODE *)((*(((Q_HEAD *)(pQHead))->qBase->eachRtn))		\
    (((Q_HEAD *)(pQHead)),((FUNCTION)(routine)),((int)(routineArg)))))

STATUS qInit(Q_HEAD* pQHead, QUEUE_BASE* pQBase, int arg1, int arg2, int arg3, int arg4, int arg5);


#ifdef __cplusplus
}
#endif

#endif

