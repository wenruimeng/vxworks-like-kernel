#include "includes.h"

#define MAX_MESSAGES 5
#define MAX_MESSAGE_LENGTH 50
#define PRI 90

static MSG_Q_ID msgQueueId;
static MSG_Q_ID msgQueueIdNew;

static int vxworks_training4;

static void taskOne(void)
{
	char message[] = "Received Message from taskOne.";
	char msgBuf[MAX_MESSAGE_LENGTH];
	int i;

	for (i = 0; i < MAX_MESSAGES; i++)
	{
		if (msgQSend(msgQueueId, message, MAX_MESSAGE_LENGTH,
			WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
		{
			printf("msgQsend in taskOne failed.\n");
		}
		else
		{
			printf("taskOne sent a message.\n");
		}
	
		if (msgQReceive(msgQueueIdNew, msgBuf, MAX_MESSAGE_LENGTH,
			WAIT_FOREVER) == ERROR)
		{
			printf("msgQReceive in taskOne failed.\n");
		}
		else
		{
			printf("taskOne %s\n",msgBuf);
		}
	}
	
	msgQDelete(msgQueueIdNew);
}

static void taskTwo(void)
{
	char message[] = "Received Message from taskTwo.";
	char msgBuf[MAX_MESSAGE_LENGTH];
	int i;

	for (i = 0; i < MAX_MESSAGES; i++)
	{
		if (msgQReceive(msgQueueId, msgBuf, MAX_MESSAGE_LENGTH,
			WAIT_FOREVER) == ERROR)
		{
			printf("msgQReceive in taskTwo failed.\n");
		}
		else
		{
			printf("taskTwo %s\n",msgBuf);
		}

		if (msgQSend(msgQueueIdNew, message, MAX_MESSAGE_LENGTH,
			WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
		{
			printf("msgQsend in taskTwo failed.\n");
		}
		else
		{
			printf("taskTwo sent a message.\n");
		}
	}

	msgQDelete(msgQueueId);
}

void s4_msgq2(void)
{
	int taskIdOne;
	int taskIdTwo;

	if ((msgQueueId = msgQCreate(MAX_MESSAGES, MAX_MESSAGE_LENGTH,
		MSG_Q_FIFO)) == NULL)
	{
		printf("msgQCreate in failed.\n");
	}

	if ((msgQueueIdNew = msgQCreate(MAX_MESSAGES, MAX_MESSAGE_LENGTH,
		MSG_Q_FIFO)) == NULL)
	{
		printf("msgQCreate in failed.\n");
	}

	if ((taskIdOne = taskSpawn("task1", PRI, 0, 2000,
		(FUNCTION)taskOne, 1, 2, 3, 4, 5)) == ERROR)
	{
		printf("taskSpawn taskOne failed.\n");
	}

	if ((taskIdTwo = taskSpawn("task2", PRI, 0, 2000,
		(FUNCTION)taskTwo, 1, 2, 3, 4, 5)) == ERROR)
	{
		printf("taskSpawn taskTwo failed.\n");
	}
}
