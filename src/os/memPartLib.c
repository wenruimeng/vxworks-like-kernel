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

/* optional check for bad blocks */

#define MEM_BLOCK_CHECK			0x10

/* response to errors when allocating memory */

#define MEM_ALLOC_ERROR_LOG_FLAG	0x20
#define MEM_ALLOC_ERROR_SUSPEND_FLAG	0x40

/* response to errors when freeing memory */

#define MEM_BLOCK_ERROR_LOG_FLAG	0x80
#define MEM_BLOCK_ERROR_SUSPEND_FLAG	0x100

#define NEXT_HDR(pHdr)  ((BLOCK_HDR *) ((char *) (pHdr) + (2 * (pHdr)->nWords)))
#define PREV_HDR(pHdr)	((pHdr)->prevHdr)

#define HDR_TO_BLOCK(pHdr)	((char *) ((int) pHdr + sizeof (BLOCK_HDR)))
#define BLOCK_TO_HDR(pBlock)	((BLOCK_HDR *) ((int) pBlock - \
						sizeof(BLOCK_HDR)))

#define HDR_TO_NODE(pHdr)	(& ((FREE_BLOCK *) pHdr)->node)
#define NODE_TO_HDR(pNode)	((BLOCK_HDR *) ((int) pNode - \
						OFFSET (FREE_BLOCK, node)))

static BOOL memPartLibInstalled = FALSE;

static OBJ_CLASS memPartClass;
CLASS_ID memPartClassId = &memPartClass;

static PARTITION memSysPartition;
PART_ID memSysPartId = &memSysPartition;
U32 memDefaultAlign = _ALLOC_ALIGN_SIZE;

static SEMAPHORE semMemSysPartition;

static void memPartSemInit(PART_ID partId);

FUNCTION  memPartSemInitRtn	= (FUNCTION) memPartSemInit;

unsigned memPartDefaultOption = MEM_BLOCK_ERROR_SUSPEND_FLAG | MEM_BLOCK_CHECK;

static BOOL memPartBlockIsValid(PART_ID partId, FAST BLOCK_HDR* pHdr, BOOL isFree);
static BLOCK_HDR* memAlignedBlockSplit(PART_ID partId
	, FAST BLOCK_HDR* pHdr
	, FAST unsigned nWords
	, unsigned minWords
	, unsigned align);

static STATUS memPartAddToPool(PART_ID partId, char* pPool, unsigned poolSize);

STATUS memPartLibInit(char* pPool, unsigned poolSize)
{
	if((!memPartLibInstalled) && 
		(OK == classInit(memPartClassId, sizeof(PARTITION), OFFSET(PARTITION, objCore)
		, (FUNCTION)memPartCreate, (FUNCTION)memPartInit, (FUNCTION)memPartDestroy)))
	{
		memPartInit(&memSysPartition, pPool, poolSize);
		memPartLibInstalled = TRUE;
	}

	return ((memPartLibInstalled)? OK : ERROR);
}

PART_ID memPartCreate(char* pPool, unsigned poolSize)
{
	PART_ID pPart = (PART_ID)objAlloc(memPartClassId);

	if(NULL != pPart)
	{
		memPartInit(pPart, pPool, poolSize);
	}

	return pPart;
}

void memPartInit(PART_ID partId, char* pPool, unsigned poolSize)
{
	memset((void*)partId, 0, sizeof(*partId));

	partId->options = memPartDefaultOption;
	partId->minBlockWords = sizeof (FREE_BLOCK) >> 1;	/* word not byte */

	(* memPartSemInitRtn) (partId);
	
	dllInit(&partId->freeList);
	
	objCoreInit(&partId->objCore, memPartClassId);
	
	memPartAddToPool(partId, pPool, poolSize);
}

STATUS memPartDestroy(PART_ID partId)
{
	return (ERROR);
}

void memAddToPool(FAST char *pPool, FAST unsigned poolSize)
{
    (void)memPartAddToPool(&memSysPartition, pPool, poolSize);
}


static STATUS memPartAddToPool(PART_ID partId, char* pPool, unsigned poolSize)
{
	BLOCK_HDR* pHdrStart;
	BLOCK_HDR* pHdrMid;
	BLOCK_HDR* pHdrEnd;
	char* tmp;
	int reducePool;

	if(!(IS_CLASS(partId, memPartClassId)))	/* only memPartClass can call this function */
	{
		return (ERROR);
	}

	tmp = (char*) MEM_ROUND_UP(pPool);
	reducePool = tmp - pPool;

	/* adjust the lenght */
	if(poolSize >= reducePool)
	{
		poolSize -= reducePool;
	}
	else
	{
		poolSize = 0;
	}
	pPool = tmp;
	
	poolSize = MEM_ROUND_DOWN(poolSize);
	
	/* at least one valid free block and three header blocks */
	if((sizeof(BLOCK_HDR)*3 + (partId->minBlockWords*2)) > poolSize)
	{
		return (ERROR);
	}

	/* initialize three blocks */
	pHdrStart = (BLOCK_HDR*)pPool;
	pHdrStart->prevHdr = NULL;
	pHdrStart->free = FALSE;		/* never in use */
	pHdrStart->nWords = sizeof(BLOCK_HDR) >> 1;

	pHdrMid = NEXT_HDR(pHdrStart);
	pHdrMid->prevHdr = pHdrStart;
	pHdrMid->free = TRUE;			/* the main block */
	pHdrMid->nWords = (poolSize - 2*sizeof(BLOCK_HDR)) >> 1;

	pHdrEnd = NEXT_HDR(pHdrMid);
	pHdrEnd->prevHdr = pHdrMid;
	pHdrEnd->free = FALSE;
	pHdrEnd->nWords = sizeof (BLOCK_HDR) >> 1;

	/* TODO take sem hear */
	semTake(partId->semPartId, WAIT_FOREVER);
	
	dllInsert(&partId->freeList, (DL_NODE*)NULL, HDR_TO_NODE(pHdrMid));
	partId->totalWords += (poolSize >> 1);

	/* TODO give sem hear */
	semGive(partId->semPartId);


	return (OK);
}

void* memPartAllignedAlloc(FAST PART_ID partId, unsigned nBytes, unsigned align)
{
	FAST unsigned nWords;
	FAST unsigned nWordsExtra;
	FAST DL_NODE* pNode;
	FAST BLOCK_HDR* pHdr;
	BLOCK_HDR* pNewHdr;
	BLOCK_HDR* origpHdr;

	if(!(IS_CLASS(partId, memPartClassId)))	/* only memPartClass can call this function */
	{
		return (NULL);
	}

	nWords = (MEM_ROUND_UP(nBytes)+sizeof(BLOCK_HDR)) >>1;

	if((nWords<<1) < nBytes)
	{
		/* TODO suspend the task */
		return (NULL);
	}

	if(nWords < partId->minBlockWords)
	{
		nWords = partId->minBlockWords;
	}

	/* TODO task the semaphore hear */
	semTake(partId->semPartId, WAIT_FOREVER);
	pNode = DLL_FIRST(&partId->freeList);
	nWordsExtra = nWords + align/2; /* why? */

	for(;;)
	{
		while(NULL != pNode)
		{
			if((NODE_TO_HDR(pNode)->nWords > nWordsExtra) ||
				((NODE_TO_HDR(pNode)->nWords == nWords) && 
				(ALIGNED(HDR_TO_BLOCK(NODE_TO_HDR(pNode)), align))))
			{
				break;
			}

			pNode = DLL_NEXT(pNode);
		}

		if(NULL == pNode)
		{
			/*TODO give the semaphore */
			semGive(partId->semPartId);
			return NULL;
		}

		pHdr = NODE_TO_HDR(pNode);
		origpHdr = pHdr;

		pNewHdr = memAlignedBlockSplit(partId, pHdr, nWords, partId->minBlockWords, align);
		if(NULL != pNewHdr)
		{
			pHdr = pNewHdr;
			break;
		}

		pNode = DLL_NEXT(pNode);
	}

	pHdr->free = FALSE;
	partId->allBlocksAlloc++;
	partId->allWordsAlloc += pHdr->nWords;
	partId->curBlocksAlloc++;
	partId->curWordsAlloc += pHdr->nWords;

	/*TODO give the  semaphore hear */
	semGive(partId->semPartId);
	return (HDR_TO_BLOCK(pHdr));
	
}

void* memPartAlloc(FAST PART_ID partId, unsigned bytes)
{
	return memPartAllignedAlloc(partId, bytes, memDefaultAlign);
}

STATUS memPartFree(PART_ID partId, char* pBlock)
{
	FAST BLOCK_HDR *pHdr;
    FAST unsigned   nWords;
    FAST BLOCK_HDR *pNextHdr;

	if(!IS_CLASS(partId, memPartClassId))
	{
		return (ERROR);
	}

	if(NULL == pBlock)
	{
		return (OK);
	}

	pHdr =  BLOCK_TO_HDR(pBlock);

	semTake(partId->semPartId, WAIT_FOREVER);

	if((partId->options & MEM_BLOCK_CHECK)
		&& !memPartBlockIsValid(partId, pHdr, FALSE))
	{
		semGive(partId->semPartId);
		return (ERROR);
	}

	nWords = pHdr->nWords;
	if(PREV_HDR(pHdr)->free)
	{/* the prev hdr is free and than coalesce with it */
		pHdr->free = FALSE;
		pHdr = PREV_HDR(pHdr);
		pHdr->nWords += nWords;
	}
	else
	{
		pHdr->free = TRUE;
		dllInsert(&partId->freeList, (DL_NODE*)NULL, HDR_TO_NODE(pHdr));
	}

	/* check to coalesce with the next */
	pNextHdr = NEXT_HDR(pHdr);
	if(pNextHdr->free)
	{
		pHdr->nWords += pNextHdr->nWords;
		dllRemove(&partId->freeList, HDR_TO_NODE(pNextHdr));
	}

	/* cannot use pNextHdr->prevHdr=pHdr hear */
	NEXT_HDR(pHdr)->prevHdr = pHdr;

	partId->curBlocksAlloc--;
	partId->curWordsAlloc -= nWords;

	/* TODO give sem hear */
	semGive(partId->semPartId);
	
	return (OK);
}

static BOOL memPartBlockIsValid(PART_ID partId, FAST BLOCK_HDR* pHdr, BOOL isFree)
{
	BOOL valid;

	TASK_LOCK();
	semGive(partId->semPartId);
	
	valid = MEM_ALIGNED(pHdr)
	&& MEM_ALIGNED(pHdr->nWords*2)
	&& (pHdr->nWords < partId->totalWords) 
	&& (pHdr->free == isFree)
	&& (pHdr == PREV_HDR(NEXT_HDR(pHdr)))
	&& (pHdr == NEXT_HDR(PREV_HDR(pHdr)));
	
	semTake(partId->semPartId, WAIT_FOREVER);
	TASK_UNLOCK();

	return valid;
}

static BLOCK_HDR* memAlignedBlockSplit(PART_ID partId
	, FAST BLOCK_HDR* pHdr
	, FAST unsigned nWords
	, unsigned minWords
	, unsigned align)
{
	FAST BLOCK_HDR *pNewHdr;
    FAST BLOCK_HDR *pNextHdr;
    FAST char *endOfBlock;
    FAST char *pNewBlock;
    int blockSize;

	endOfBlock = (char*)pHdr + (pHdr->nWords*2);

	pNewBlock = (char*)((unsigned)endOfBlock - ((nWords - sizeof(BLOCK_HDR)/2)*2));

	pNewBlock = (char*)((unsigned)pNewBlock & (~(align-1)));

	pNewHdr = BLOCK_TO_HDR(pNewBlock);

	blockSize = ((char*)pNewHdr - (char*)pHdr)/2;

	if(blockSize < minWords)
	{
		if(pNewHdr == pHdr)
		{
			dllRemove(&partId->freeList, HDR_TO_NODE(pHdr));
		}
		else
		{
			return NULL;
		}
	}
	else
	{	/* recaculate pHdr */
		pNewHdr->prevHdr = pHdr;
		pHdr->nWords = blockSize;
	}

	if(((U32)endOfBlock - (U32)pNewHdr - (nWords*2)) < (minWords*2))
	{
		pNewHdr->nWords = (endOfBlock - pNewBlock + sizeof(BLOCK_HDR))/2;
		pNewHdr->free = TRUE;

		NEXT_HDR(pNewHdr)->prevHdr = pNewHdr;
	}
	else
	{/* space left is enough to be a fragment on the free list then */
		pNewHdr->nWords = nWords;
		pNewHdr->free = TRUE;

		pNextHdr = NEXT_HDR(pNewHdr);
		/* words °üÀ¨BlockHdr */
		pNextHdr->nWords = ((U32)endOfBlock - (U32)pNextHdr) / 2;
		pNextHdr->prevHdr = pNewHdr;
		pNextHdr->free = TRUE;

		dllAdd(&partId->freeList, HDR_TO_NODE(pNextHdr));

		NEXT_HDR(pNextHdr)->prevHdr = pNewHdr;
	}

	return (pNewHdr);
}

static void memPartSemInit(PART_ID partId)
{
	semBInit(&semMemSysPartition, SEM_Q_FIFO, SEM_FULL);

	partId->semPartId = &semMemSysPartition;
}

void* malloc(unsigned bytes)
{
	return memPartAlloc(memSysPartId, bytes);
}

void free(void* p)
{
	memPartFree(memSysPartId, (char*)p);
}

