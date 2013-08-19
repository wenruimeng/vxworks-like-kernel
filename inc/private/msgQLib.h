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
#ifndef __MSG_Q_LIB_H_
#define __MSG_Q_LIB_H_

/* generic status codes */

#define S_msgQLib_INVALID_MSG_LENGTH		(M_msgQLib | 1)
#define S_msgQLib_NON_ZERO_TIMEOUT_AT_INT_LEVEL	(M_msgQLib | 2)
#define S_msgQLib_INVALID_QUEUE_TYPE		(M_msgQLib | 3)

#include "msgq.h"

#define MSG_NODE_SIZE(msgLength) \
	(MEM_ROUND_UP((sizeof (MSG_NODE) + msgLength)))
	
#define MSG_NODE_DATA(pNode)   (((char *) pNode) + sizeof (MSG_NODE))

/* message queue typedefs */

typedef struct _msg_q		/* MSG_Q */
{
    OBJ_CORE		objCore;	/* object management */
    Q_JOB_HEAD		msgQ;		/* message queue head */
    Q_JOB_HEAD		freeQ;		/* free message queue head */
    int			options;	/* message queue options */
    int			maxMsgs;	/* max number of messages in queue */
    int			maxMsgLength;	/* max length of message */
    int			sendTimeouts;	/* number of send timeouts */
    int			recvTimeouts;	/* number of receive timeouts */
} MSG_Q;

typedef struct			/* MSG_NODE */
{
    Q_JOB_NODE		node;		/* queue node */
    int			msgLength;	/* number of bytes of data */
} MSG_NODE;


typedef struct _msg_q *MSG_Q_ID;	/* message queue ID */

typedef struct			/* MSG_Q_INFO */
{
	int     numMsgs;		/* OUT: number of messages queued */
	int     numTasks;		/* OUT: number of tasks waiting on msg q */

	int     sendTimeouts;	/* OUT: count of send timeouts */
	int     recvTimeouts;	/* OUT: count of receive timeouts */

	int     options;		/* OUT: options with which msg q was created */
	int     maxMsgs;		/* OUT: max messages that can be queued */
	int     maxMsgLength;	/* OUT: max byte length of each message */

	int     taskIdListMax;	/* IN: max tasks to fill in taskIdList */
	int *   taskIdList;		/* PTR: array of task ids waiting on msg q */

	int     msgListMax;		/* IN: max msgs to fill in msg lists */
	char ** msgPtrList;		/* PTR: array of msg ptrs queued to msg q */
	int *   msgLenList;		/* PTR: array of lengths of msgs */

} MSG_Q_INFO;

extern CLASS_ID msgQClassId;

MSG_Q_ID msgQCreate(int maxMsgs, int maxMsgLength, int options);
STATUS msgQLibInit (void);
STATUS msgQSend(FAST MSG_Q_ID msgQId, char* buffer, FAST UINT nBytes, int timeout, int priority);
int msgQReceive(FAST MSG_Q_ID msgQId, char* buffer, UINT maxNBytes, int timeout);
STATUS msgQDelete(MSG_Q_ID msgQId);

#endif

