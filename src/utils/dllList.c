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

STATUS dllInit(FAST DL_LIST* pList)
{
	pList->head = NULL;
	pList->tail = NULL;

	return (OK);
}

STATUS dllDelete(FAST DL_LIST* pList)
{
	return (OK);
}

void dllInsert(FAST DL_LIST* pList, FAST DL_NODE* pPrev, FAST DL_NODE* pNode)
{
	register DL_NODE* pNext;
	
	if(NULL == pPrev)
	{
		pNext = pList->head;
		pList->head = pNode;
	}
	else
	{
		pNext = pPrev->next;
		pPrev->next = pNode;
	}

	if(NULL == pNext)
	{
		pList->tail = pNode;
	}
	else
	{
		pNext->prev = pNode;
	}

	pNode->next = pNext;
	pNode->prev = pPrev;
	
}

void dllAdd(DL_LIST* pList, DL_NODE* pNode)
{
	dllInsert(pList, pList->tail, pNode);
}

void dllRemove(DL_LIST* pList, DL_NODE* pNode)
{
	if(NULL == pNode->prev)
	{
		pList->head = pNode->next;
	}
	else
	{
		pNode->prev->next = pNode->next;
	}

	if(NULL == pNode->next)
	{
		pList->tail = pNode->prev;
	}
	else
	{
		pNode->next->prev = pNode->prev;
	}
}

DL_NODE* dllGet(FAST DL_LIST* pList)
{
	FAST DL_NODE* pNode = pList->head;

	if(NULL != pNode)
	{
		pList->head = pNode->next;

		if(NULL == pNode->next)
		{
			pList->tail = NULL;
		}
		else
		{
			pNode->next->prev = NULL;
		}
	}

	return pNode;
}

int dllCount(DL_LIST *pList)
{
    FAST DL_NODE *pNode = DLL_FIRST (pList);
    FAST int count = 0;

    while (pNode != NULL)
	{
	count++;
	pNode = DLL_NEXT (pNode);
	}

    return (count);
}

