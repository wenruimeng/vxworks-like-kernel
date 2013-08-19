#include "includes.h"


/* defines */

#if 1
#define STATUS_INFO			/* define to allow printf() calls */
#endif

#define MAX_MSG		1		/* max number of messages in queue */
#define MSG_SIZE	sizeof (MY_MSG)	/* size of message */
#define DELAY		100		/* 100 ticks */
#define	HIGH_PRI	150		/* priority of high priority task */
#define LOW_PRI		200		/* priority of low priority task */

#define TASK_HIGHPRI_TEXT	"Hello from the 'high priority' task"
#define TASK_LOWPRI_TEXT	"Hello from the 'low priority' task"


/* typedefs */

typedef struct my_msg
	{
	int    childLoopCount;		/* loop count in task sending msg */
	char * buffer;			/* message text */
	} MY_MSG;


/* globals */

SEM_ID		semId;			/* semaphore ID */
MSG_Q_ID	msgQId;			/* message queue ID */
int		highPriId;		/* task ID of high priority task */
int		lowPriId;		/* task ID of low priority task */
int		windDemoId;		/* task ID of windDemo task */


/* forward declarations */

static void taskHighPri (int iteration);
static void taskLowPri (int iteration);



/*******************************************************************************
*
* windDemo - parent task to spawn children 
*
* This task calls taskHighPri() and taskLowPri() to do the
* actual operations of the test and suspends itself.
* Task is resumed by the low priority task.
*
*/

void demo 
    (
    int iteration			/* number of iterations of child code */
    )
    {
    int loopCount = 0; 			/* number of times through windDemo */

#ifdef	STATUS_INFO
    printf ("Entering windDemo\n");
#endif /* STATUS_INFO */

    if (iteration == 0)			/* set default to 10,000 */
	iteration = 10000;

    /* create objects used by the child tasks */

    msgQId	= msgQCreate (MAX_MSG, MSG_SIZE, MSG_Q_FIFO);
    semId	= semBCreate (SEM_Q_PRI, SEM_FULL);

    windDemoId = taskIdSelf ();

    while(loopCount < 4)
	{

	/* spawn child tasks to exercise kernel routines */

    	highPriId = taskSpawn ("tHighPri", HIGH_PRI, 0, 4000, 
			   (FUNCTION) taskHighPri, iteration,2,3,4,5);

    	lowPriId = taskSpawn ("tLowPri", LOW_PRI, 0, 4000, 
			   (FUNCTION) taskLowPri, iteration,2,3,4,5);


    	taskSuspend (0);		/* to be waken up by taskLowPri */ 

#ifdef	STATUS_INFO
    	printf ("\nParent demo has just completed loop number %d\n",
		loopCount);
#endif /* STATUS_INFO */

    	loopCount++;
	}
    }

/*******************************************************************************
*
* taskHighPri - high priority task
*
* This tasks exercises various kernel functions. It will block if the
* resource is not available and relingish the CPU to the next ready task.
*
*/

static void taskHighPri 
(
    int iteration			/* number of iterations through loop */
)
{
    int    ix;				/* loop counter */
    MY_MSG msg;				/* message to send */
    MY_MSG newMsg;			/* message to receive */

    for (ix = 0; ix < iteration; ix++)
	{

	/* take and give a semaphore - no context switch involved */

	semGive (semId);		
	semTake (semId, 100);		/* semTake with timeout */


	/* 
	 * take semaphore - context switch will occur since semaphore 
	 * is unavailable 
	 */

	semTake (semId, WAIT_FOREVER);	/* semaphore not available */

	taskSuspend (0);		/* suspend itself */


	/* build message and send it */

	msg.childLoopCount = ix;
	msg.buffer = TASK_HIGHPRI_TEXT;

	msgQSend (msgQId, (char *) &msg, MSG_SIZE, 0, MSG_PRI_NORMAL);

	/* 
	 * read message that this task just sent and print it - no context 
	 * switch will occur since there is a message already in the queue 
	 */

	msgQReceive (msgQId, (char *) &newMsg, MSG_SIZE, NO_WAIT);

#ifdef	STATUS_INFO
	printf ("%s\n Number of iterations is %d\n", 
		newMsg.buffer, newMsg.childLoopCount);
#endif /* STATUS_INFO */

	/* 
	 * block on message queue waiting for message from low priority task 
	 * context switch will occur since there is no message in the queue
	 * when message is received, print it 
	 */

	msgQReceive (msgQId, (char *) &newMsg, MSG_SIZE, WAIT_FOREVER);

#ifdef	STATUS_INFO
	printf ("%s\n Number of iterations by this task is: %d\n", 
		newMsg.buffer, newMsg.childLoopCount);
#endif /* STATUS_INFO */

	taskDelay(60);
	}
}

/*******************************************************************************
*
* taskLowPri - low priority task
*
* This task runs at a lower priority and is designed to make available
* the resouces that the high priority task is waiting for and subsequently
* unblock the high priority task.
*
*/

static void taskLowPri 
(
    int iteration			/* number of times through loop */
)
{
    int    ix;				/* loop counter */
    MY_MSG msg;				/* message to send */

    for (ix = 0; ix < iteration; ix++)
	{
	semGive (semId);		/* unblock tHighPri */

	taskResume (highPriId);		/* unblock tHighPri */

	/* build message and send it */

	msg.childLoopCount = ix;
	msg.buffer = TASK_LOWPRI_TEXT;
	msgQSend (msgQId, (char *) &msg, MSG_SIZE, 0, MSG_PRI_NORMAL);
	taskDelay (60);
	}

    taskResume (windDemoId);		/* wake up the windDemo task */
}

