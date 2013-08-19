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

#define PRINT printf

ULONG kernelTicks = 0;

typedef struct 
{
	Q_HEAD qHead;
	int otherArg;
}SomeStruct;

typedef struct 
{
	Q_NODE node;
	int key1;
	int key2;
}SomeNode;

static BOOL eachFifo(Q_FIFO_NODE* pQNode, int arg)
{
	int i = *((int*)arg);

	PRINT("FIFO: i:%d key:%d i:%d\n", i, ((SomeNode*)pQNode)->key1, ((SomeNode*)pQNode)->key2);

	i++;
	*((int*)arg) = i;
}

static BOOL eachPri(Q_PRI_NODE* pQNode, int arg)
{
	int i = *((int*)arg);

	PRINT("PRI: i:%d key:%d i:%d\n", i, ((SomeNode*)pQNode)->key1, ((SomeNode*)pQNode)->key2);

	i++;
	*((int*)arg) = i;
}

void testQueue()
{
	int keys[] = {1,4,6,8,2,4,3,6,10};
	SomeStruct s;
	SomeNode n[sizeof(keys)/sizeof(int)];
	SomeNode *pNode;
	int i, len = sizeof(keys)/sizeof(int);

	memset(&s, 0, sizeof(s));

	memset(&n, 0, sizeof(n));

	PRINT("test Q_FIFO\n");
	qInit(&s.qHead, Q_FIFO, 1,2,3,4,5);

	for(i = 0; i < len; i++)
	{
		n[i].key1 = keys[i];
		n[i].key2 = i;
		Q_PUT(&s.qHead, &n[i], n[i].key1);
	}

	i = 0;
	Q_EACH(&s.qHead, eachFifo, &i);

	pNode = (SomeNode*)Q_GET(&s.qHead);
	while(NULL != pNode)
	{
		PRINT("PNode:key1:%d key2:%d\n", pNode->key1, pNode->key2);
		pNode = (SomeNode*)Q_GET(&s.qHead);
	}

	memset(&s, 0, sizeof(s));
	memset(&n, 0, sizeof(n));

	PRINT("\ntest Q_PRI\n");
	qInit(&s.qHead, Q_PRI_LIST, 1,2,3,4,5);

	for(i = 0; i < len; i++)
	{
		n[i].key1 = keys[i];
		n[i].key2 = i;
		Q_PUT(&s.qHead, &n[i], n[i].key1);
	}

	i = 0;
	Q_EACH(&s.qHead, eachPri, &i);

	/* Q_REMOVE(&s.qHead, &n[4]); */
	Q_CALIBRATE(&s.qHead, 8);

	n[4].key1 = 13;
	Q_RESORT(&s.qHead, &n[4], 13);


	pNode = (SomeNode*)Q_GET(&s.qHead);
	while(NULL != pNode)
	{
		PRINT("PNode:key1:%d key2:%d\n", pNode->key1, pNode->key2);
		pNode = (SomeNode*)Q_GET(&s.qHead);
	}
}

