#include "includes.h"

#define ITER 10

static SEM_ID semBinary;

static int global = 0;

static int vxworks_training3;

static void taskOne(void)
{
	int i;
	for (i = 0; i < ITER; i++)
	{
		semTake(semBinary, WAIT_FOREVER);
		printf("I am taskOne and global = %d...................\n", ++global);
		semGive(semBinary);
	}
}

static void taskTwo(void)
{
	int i;
	semGive(semBinary);	/* NOTE 1 */
	for ( i = 0; i < ITER; i++)
	{
		semTake(semBinary, WAIT_FOREVER);
		printf("I am taskTow and global = %d...................\n", --global);
		semGive(semBinary);
	}
}

void s3_sem1(void)
{
	int taskIdOne;
	int taskIdTwo;
	semBinary = semBCreate(SEM_Q_FIFO, SEM_FULL);
	semTake(semBinary, WAIT_FOREVER);	/* NOTE 2 */
	taskIdOne = taskSpawn("task1", 90, 0, 2000,
		(FUNCTION)taskOne, 1, 2, 3, 4, 5);
	taskIdTwo = taskSpawn("task2", 90, 0, 2000,
		(FUNCTION)taskTwo, 1, 2, 3, 4, 5);
}
