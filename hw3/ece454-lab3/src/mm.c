/*
 * ECE454 Lab 3 - Malloc
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "TTGH",
    /* First member's first name and last name */
    "Won-Jin:Kim",
    /* First member's student number */
    "1003611424",
    /* Second member's first name and last name (do not modify if working alone) */
    "",
    /* Second member's student number (do not modify if working alone) */
    ""
};

/*************************************************************************
 * Basic Constants and Macros
 * You are not required to use these macros but may find them helpful.
*************************************************************************/
#define WSIZE       sizeof(void *)            /* word size (bytes) */
#define DSIZE       (2 * WSIZE)            /* doubleword size (bytes) */
#define CHUNKSIZE   (1<<7)      /* initial heap size (bytes) */

#define MAX(x,y) ((x) > (y)?(x) :(y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)          (*(uintptr_t *)(p))
#define PUT(p,val)      (*(uintptr_t *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)     (GET(p) & ~(DSIZE - 1))
#define GET_ALLOC(p)    (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp)        ((char *)(bp) - WSIZE)
#define FTRP(bp)        ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

void* heap_listp = NULL;
/***********************************************************
* Utilities
***********************************************************/

#define MAX_FREE_LIST 14

typedef struct fblock
{
    struct fblock* prev;
    struct fblock* next;
} fblock;

fblock* fLists[MAX_FREE_LIST];

int find_fList(int size)
{
    size -= 1;
    int val = -2;
    while (size >>= 1) val += 1;

    int res = MAX(0,val);

    if (res >= MAX_FREE_LIST)
    {
       return MAX_FREE_LIST-1;
    }
    return res;
}

void add_fList(fblock* fb)
{
    if(fb == NULL) return;

    int idx = find_fList(GET_SIZE(HDRP(fb))-DSIZE);

    if(fLists[idx] != NULL)
    {
        fb->prev = NULL;
        fb->next = fLists[idx];
        fLists[idx]->prev = fb;
        fLists[idx] = fb;
    } else {
        fLists[idx] = fb;
        fLists[idx]->prev = fLists[idx]->next = NULL;
    }
}

void remove_fList(fblock* fb)
{
    if(fb == NULL) return;
    
    int idx = find_fList(GET_SIZE(HDRP(fb)) - DSIZE);

    if (fb->prev == NULL && fb->next == NULL)
    { 
        fLists[idx] = NULL;
    }
    else if (fb->prev==NULL)
    {       
        fLists[idx] = fb->next;
        fLists[idx]->prev = NULL; 
    }
    else if (fb->next == NULL)
    {      
        (fb->prev)->next = NULL;
    } else {                               
        ((fb)->next)->prev = (fb)->prev;
        ((fb)->prev)->next = (fb)->next;
    }
}

size_t determine_size(size_t size , int is_malloc)
{
    int val = is_malloc?11:12;
    if (size < DSIZE)
    {
        size = 2 * DSIZE;
    }
    else if (size<=(2<<val))
    {
        size --;
        for(int i=1; i<=5; ++i)
        {
            size |= size >> i;
        }
        size ++;
    } else {
        size--;
        size = DSIZE * (size/ DSIZE)  + DSIZE;
    }
    return size;
}

void place(void* bp, size_t asize);

/**********************************************************
 * mm_init
 * Initialize the heap, including "allocation" of the
 * prologue and epilogue
 **********************************************************/
 int mm_init(void)
 {
   if ((heap_listp = mem_sbrk(4*WSIZE)) == (void *)-1)
         return -1;
     PUT(heap_listp, 0);                         // alignment padding
     PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));   // prologue header
     PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));   // prologue footer
     PUT(heap_listp + (3 * WSIZE), PACK(0, 1));    // epilogue header
     heap_listp += DSIZE;

     memset(fLists, '\0', sizeof(fLists));
     return 0;
 }

/**********************************************************
 * coalesce
 * Covers the 4 cases discussed in the text:
 * - both neighbours are allocated
 * - the next block is available for coalescing
 * - the previous block is available for coalescing
 * - both neighbours are available for coalescing
 **********************************************************/
void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) 
    {       /* Case 1 */
        return bp;
    }

    else if (prev_alloc && !next_alloc) 
    { /* Case 2 */
        remove_fList((fblock *)NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));

        PUT(HDRP(bp),PACK(size, 0));
        PUT(FTRP(bp),PACK(size, 0));
        return (bp);
    }

    else if (!prev_alloc && next_alloc) 
    { /* Case 3 */
        remove_fList((fblock *)PREV_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));

        PUT(FTRP(bp),PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)),PACK(size, 0));
        return (PREV_BLKP(bp));
    }

    else 
    {            /* Case 4 */
        remove_fList((fblock *)PREV_BLKP(bp));
        remove_fList((fblock *)NEXT_BLKP(bp));
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(FTRP(NEXT_BLKP(bp))) ;

        PUT(HDRP(PREV_BLKP(bp)),PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)),PACK(size, 0));
        return (PREV_BLKP(bp));
    }
}

/**********************************************************
 * extend_heap
 * Extend the heap by "words" words, maintaining alignment
 * requirements of course. Free the former epilogue block
 * and reallocate its new header
 **********************************************************/
void *extend_heap(size_t words)
{
    char *bp;

    /* Allocate an even number of words to maintain alignments */
    if ( (bp = mem_sbrk(words + DSIZE)) == (void *)-1 )
        return NULL;
    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(words+DSIZE, 0));                // free block header
    PUT(FTRP(bp), PACK(words+DSIZE, 0));                // free block footer
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));        // new epilogue header
    place(bp, words);
    /* Coalesce if the previous block was free */
    // return coalesce(bp);
    return bp;
}


/**********************************************************
 * find_fit
 * Traverse the heap searching for a block to fit asize
 * Return NULL if no free blocks can handle that size
 * Assumed that asize is aligned
 **********************************************************/
void * find_fit(size_t asize)
{
    // void *bp;
    // for (bp = heap_listp; GET_SIZE(HDRP(bp)) > 0; bp = NEXT_BLKP(bp))
    // {
    //     if (!GET_ALLOC(HDRP(bp)) && (asize <= GET_SIZE(HDRP(bp))))
    //     {
    //         return bp;
    //     }
    // }
    for (int i = find_fList(asize); i < MAX_FREE_LIST; ++i)
    {
        fblock* cur = fLists[i];
        while (cur != NULL)
        {
            int ss = GET_SIZE(HDRP(cur)) - DSIZE;
            if (asize <= ss && ss < ((DSIZE*2)+asize))
            {
                place(cur, ss);
                remove_fList(cur);
                return cur;
            }
            else if (ss >= ((DSIZE*2)+asize))
            {
                size_t val = ss - asize;
                void* ptr = (void*)cur +  val;
                remove_fList(cur);
                PUT(HDRP(ptr), PACK(asize+DSIZE, 0));
                PUT(FTRP(ptr), PACK(asize+DSIZE, 0));

                PUT(HDRP(cur), PACK(val, 0));
                PUT(FTRP(cur), PACK(val, 0));
                // coalesce(cur);
                add_fList(cur);
            
                place(ptr, asize);
                return ptr;
            }
            cur = cur->next;      
        }

    }
    return NULL;
}

/**********************************************************
 * place
 * Mark the block as allocated
 **********************************************************/
void place(void* bp, size_t asize)
{
  /* Get the current block size */
//   size_t bsize = GET_SIZE(HDRP(bp));
  
//   PUT(HDRP(bp), PACK(bsize, 1));
//   PUT(FTRP(bp), PACK(bsize, 1));
    asize += DSIZE;
  /* Get the current block size */
    PUT(HDRP(bp), PACK(asize, 1));
    PUT(FTRP(bp), PACK(asize, 1));
}

/**********************************************************
 * mm_free
 * Free the block and coalesce with neighbouring blocks
 **********************************************************/
void mm_free(void *bp)
{
    if(bp == NULL)
    {
      return;
    }

    if(GET_ALLOC(HDRP(bp))==0)
    {
        return;
    }

    size_t size = GET_SIZE(HDRP(bp));
    PUT(HDRP(bp), PACK(size,0));
    PUT(FTRP(bp), PACK(size,0));
    add_fList(coalesce(bp));
}


/**********************************************************
 * mm_malloc
 * Allocate a block of size bytes.
 * The type of search is determined by find_fit
 * The decision of splitting the block, or not is determined
 *   in place(..)
 * If no block satisfies the request, the heap is extended
 **********************************************************/
void *mm_malloc(size_t size)
{
    size_t asize = size; /* adjusted block size */
    // size_t extendsize; /* amount to extend heap if no fit */
    char * bp;

    /* Ignore spurious requests */
    if (asize <= 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    asize = determine_size(asize, 1);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL)
    {
        return bp;
    }

    // /* No fit found. Get more memory and place the block */
    if ((bp = extend_heap(asize)) == NULL)
    {
        return NULL;
    }

    return bp;
}

/**********************************************************
 * mm_realloc
 * Implemented simply in terms of mm_malloc and mm_free
 *********************************************************/
void *mm_realloc(void *ptr, size_t size)
{
    /* If size == 0 then this is just free, and we return NULL. */
    if(size == 0)
    {
      mm_free(ptr);
      return NULL;
    }

    /* If oldptr is NULL, then this is just malloc. */
    if (ptr == NULL)
      return (mm_malloc(size));

    void *oldptr = ptr;
    void *newptr;
    size_t prev_size = GET_SIZE(HDRP(oldptr))-DSIZE;

    size = determine_size(size, 0);
    
    if (size <= prev_size) 
    {   
        size_t notused = prev_size - size;
        if (notused >= DSIZE*2)
        {
            PUT(HDRP(oldptr + size + DSIZE), PACK(notused, 0));
            PUT(FTRP(oldptr + size + DSIZE), PACK(notused, 0));

            place(oldptr, size);
            add_fList(oldptr + size + DSIZE);
        }
        return oldptr;
    } else { 
        PUT(HDRP(ptr), PACK(prev_size+DSIZE, 0));
        PUT(FTRP(ptr), PACK(prev_size+DSIZE, 0));
        ptr = coalesce(ptr);
       
        if(size <= GET_SIZE(HDRP(ptr)) - DSIZE)
        {
            place(ptr, GET_SIZE(HDRP(ptr)) - DSIZE);
            memmove(ptr, oldptr, prev_size);
            return ptr;
        }

        newptr = mm_malloc(size);
        if (newptr == NULL){
            return NULL;
        }
        memcpy(newptr, oldptr, prev_size);
        mm_free(oldptr);
        add_fList((fblock * )ptr);
        return newptr;
    }
}

/**********************************************************
 * mm_check
 * Check the consistency of the memory heap
 * Return nonzero if the heap is consistant.
 *********************************************************/
int mm_check(void){
  return 1;
}
