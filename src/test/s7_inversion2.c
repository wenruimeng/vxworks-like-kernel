#include "includes.h"

#define ITER 3
#define LONG_TIME (1 << 23)
#define HIGH 102
#define MID 103
#define LOW 104

static SEM_ID semMutex;

static int vxworks_training2;

static void prioLow(void)
{
	int i;
	int j;
	
	for (i = 0; i < ITER; i++)
	{
		printf("Low priority task tries to lock semaphore.\n");
		semTake(semMutex, WAIT_FOREVER);
		printf("Low priority task locks semaphore.\n");
		for (j = 0; j < LONG_TIME; j++);
		printf("Low priority task unlocks semaphore.\n");
		semGive(semMutex);

	}

	printf(".........................Low priority task exited.\n");
}

static void prioMedium(void)
{
	int i;
	taskDelay(2);

	for (i = 0; i < LONG_TIME * 10; i ++)
	{
		if ((i % LONG_TIME) == 0)
		{
			printf("Medium task running.\n");
		}
	}
	
	printf("-------------------------Medium priority task exited.\n");
}

static void prioHigh(void)
{
	int i;
	int j;
	taskDelay(3);

	for (i = 0; i < ITER; i++)
	{
		printf("High priority task tries to lock semaphore.\n");
		semTake(semMutex, WAIT_FOREVER);
		printf("High priority task locks semaphore.\n");
		for (j = 0; j < LONG_TIME; j++);
		printf("High priority task unlocks semaphore.\n");
		semGive(semMutex);
	}

	printf(".........................High priority task exited.\n");
}

void s7_inversion2(void)
{
	int low;
	int medium;
	int high;
	printf("\n\n...............##RUNNING##...............\n\n\n");
	semMutex = semMCreate(SEM_Q_PRI | SEM_INVERSION_SAFE);

	if ((low = taskSpawn("task1", LOW, 0x100, 20000,
		(FUNCTION)prioLow, 0, 0, 0, 0, 0)) == ERROR)
	{
		printf("taskSpawn prioLow failed.\n");
	}

	if ((medium = taskSpawn("task2", MID, 0x100, 20000,
		(FUNCTION)prioMedium, 0, 0, 0, 0, 0)) == ERROR)
	{
		printf("taskSpawn prioMedium failed.\n");
	}

	if ((high = taskSpawn("task3", HIGH, 0x100, 20000,
		(FUNCTION)prioHigh, 0, 0, 0, 0, 0)) == ERROR)
	{
		printf("taskSpawn prioHigh failed.\n");
	}
}
