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

#ifndef __MEMPART_LIB_H_
#define __MEMPART_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifndef	_ALLOC_ALIGN_SIZE
#define	_ALLOC_ALIGN_SIZE	4	/* 4 byte boundary */
#endif	/* _ALLOC_ALIGN_SIZE */

#define MEM_ROUND_UP(x)		ROUND_UP(x, _ALLOC_ALIGN_SIZE)
#define MEM_ROUND_DOWN(x)	ROUND_DOWN(x, _ALLOC_ALIGN_SIZE)
/*#define STACK_ROUND_UP(x)	ROUND_UP(x, _STACK_ALIGN_SIZE)
#define STACK_ROUND_DOWN(x)	ROUND_DOWN(x, _STACK_ALIGN_SIZE)*/
#define MEM_ALIGNED(x)		ALIGNED(x, _ALLOC_ALIGN_SIZE)

struct _semaphore;

typedef struct _mem_part
{
	OBJ_CORE objCore;
	DL_LIST freeList;
	
	unsigned totalWords;
	unsigned minBlockWords;
	unsigned options;

	unsigned curBlocksAlloc;
	unsigned curWordsAlloc;
	unsigned allBlocksAlloc;
	unsigned allWordsAlloc;

	struct _semaphore* semPartId;
}PARTITION;

typedef struct _mem_part* PART_ID;

typedef struct _blockHdr
{
	struct _blockHdr* prevHdr;
	unsigned nWords:31;
	unsigned free:1;

#if (_ALLOC_ALIGN_SIZE == 16)
	U32 pad[2];
#endif

}BLOCK_HDR;

typedef struct _freeBlock
{
	struct
	{
		struct _blockHdr* prevHdr;
		unsigned nWords:31;
		unsigned free:1;
	}hdr;

	DL_NODE node;
}FREE_BLOCK;

extern PART_ID memSysPartId;

STATUS memPartLibInit(char* pPool, unsigned poolSize);
void memPartInit(PART_ID partId, char* pPool, unsigned poolSize);
void* memPartAlloc(FAST PART_ID partId, unsigned bytes);
STATUS memPartFree(PART_ID partId, char* pBlock);
PART_ID memPartCreate(char* pPool, unsigned poolSize);
STATUS memPartDestroy(PART_ID partId);
void memAddToPool(FAST char *pPool, FAST unsigned poolSize);

#ifdef __cplusplus
}
#endif

#endif
