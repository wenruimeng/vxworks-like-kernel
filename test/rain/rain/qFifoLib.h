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

#ifndef __Q_FIFO_LIB_H_
#define __Q_FIFO_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#define	FIFO_KEY_HEAD	-1		
#define	FIFO_KEY_TAIL	0		

typedef DL_LIST Q_FIFO_HEAD;
typedef DL_NODE Q_FIFO_NODE;

extern QUEUE_ID qFifoQueueId;

Q_FIFO_HEAD* qFifoCreate(void);
STATUS qFifoInit(Q_FIFO_HEAD* pFifoHead);
STATUS qFifoDelete(Q_FIFO_HEAD* pFifoHead);
void qFifoPut(Q_FIFO_HEAD* pFifoHead, Q_FIFO_NODE* pFifoNode, ULONG key);
Q_FIFO_NODE* qFifoGet(Q_FIFO_HEAD* pFifoHead);
STATUS qFifoRemove(Q_FIFO_HEAD* pFifoHead, Q_FIFO_NODE* pFifoNode);
Q_FIFO_NODE* qFifoEach(Q_FIFO_HEAD* pFifoHead, FUNCTION routine, int routineArg);
static STATUS qFifoNullRtn();

#ifdef __cplusplus
}
#endif

#endif
