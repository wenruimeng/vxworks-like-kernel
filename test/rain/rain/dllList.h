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

#ifndef __DLL_LIST_H_
#define __DLL_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _dlnode
{
	struct _dlnode *next;
	struct _dlnode *prev;
}DL_NODE;

typedef struct _dllist
{
	DL_NODE *head;
	DL_NODE *tail;
}DL_LIST;


#define DLL_FIRST(pList)			\
    (								\
    (((DL_LIST *)(pList))->head)	\
    )

#define DLL_LAST(pList)				\
    (								\
    (((DL_LIST *)(pList))->tail)	\
    )

#define DLL_NEXT(pNode)				\
    (								\
    (((DL_NODE *)(pNode))->next)	\
    )


#define DLL_PREVIOUS(pNode)			\
    (								\
    (((DL_NODE *)(pNode))->prev)	\
    )

#define DLL_EMPTY(pList)			\
    (								\
    (((DL_LIST *)pList)->head == NULL)		\
    )

STATUS dllInit(FAST DL_LIST* pList);
STATUS dllDelete(FAST DL_LIST* pList);
void dllInsert(FAST DL_LIST* pList, FAST DL_NODE* pPrev, FAST DL_NODE* pNode);
void dllAdd(DL_LIST* pList, DL_NODE* pNode);
void dllRemove(DL_LIST* pList, DL_NODE* pNode);
DL_NODE* dllGet(FAST DL_LIST* pList);
int dllCount(DL_LIST *pList);

#ifdef __cplusplus
}
#endif

#endif

