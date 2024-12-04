/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>


#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "yik",
    /* First member's full name */
    "Hongjing Xiang",
    /* First member's email address */
    "molepi40@mail.ustc.edu.cn",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

#if 0
#define DEBUG 1
#endif

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8
#define DWORD 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
// min
#define max(x, y) (x < y ? y: x)

/* size*/
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))
#define INIT_MEM_SIZE 64
#define MIN_BLOCK_SIZE 32
#define EXTRA_SIZE 16
#define HEADER_SIZE 8
#define DHEADER_SIZE 16
// 5 bucket, 5 * 3 * 8
#define BUCKET_SIZE 24
#define TOTAL_BUCKET_SIZE 120

/**
 *  operations of block
 */
#define GET_BLOCK_FROM_PAYLOAD(bp) ((void*)((uint64_t)bp - HEADER_SIZE))
#define GET_PAYLOAD(bp) (bp + HEADER_SIZE)
#define GET_NEXT_BLOCK(bp, size) ((void*)((uint64_t)bp + size))
#define GET_HEADER(bp) ((uint64_t*)bp)
#define GET_HEADER_VAL(bp) (*(uint64_t*)bp)
#define GET_FOOTER(bp, size) ((uint64_t*)((uint64_t)bp + size - HEADER_SIZE))
#define GET_FOOTER_VAL(bp, size) (*(uint64_t*)((uint64_t)bp + size - HEADER_SIZE))
#define GET_ALLOC(header) (*header & 0x1)
#define GET_SIZE(header) (*header & (~0x7L))
// set last bit as alloc bit. with alignment 8 the last 3 bit of size is 0
#define MAKE_HEADER(bp, size, valid) (*(uint64_t*)(bp) = (valid ? size | valid : size))
#define MAKE_FOOTER(bp, size, valid) (*(uint64_t*)((uint64_t)bp + size - HEADER_SIZE) = (valid ? size | valid : size))
// operation on allcation bit
#define SET_ALLOC_BIT(header) (*header |= 0x1)
#define UNSET_ALLOC_BIT(header) (*header &= ~0x1L)

#define MAKE_FOOTER_FROM_HEADER(bp) (*((uint64_t*)GET_NEXT_BLOCK(bp) - 1) = *(uint64_t*)bp)

/**
 * operations of free list
 */
// get next free block
#define GET_NEXT(bp) (*(void**)(bp + HEADER_SIZE))
// get prev free block
#define GET_PREV(bp) (*(void**)(bp + DHEADER_SIZE))
// get block from free list
#define TAKE_OUT_BLOCK_FROM_FREE_LIST(block) GET_NEXT(GET_PREV(block)) = GET_NEXT(block); \
                                             GET_PREV(GET_NEXT(block)) = GET_PREV(block)
// insert block to free list
#define INSERT_BLOCK_TO_FREE_LIST(block, header) GET_NEXT(block) = GET_NEXT(header); \
                                                 GET_PREV(block) = header; \
                                                 GET_PREV(GET_NEXT(header)) = block; \
                                                 GET_NEXT(header) = block
                                                 
/**
 * bucket 
 * 2^6, 2^10, 2^11, 2^12. total 5 free list
 */


void* heap_start;
void* heap_end;
void* mem_start;
void* mem_end;
void* dummy_header;
void* dummy_footer;
uint64_t heap_size;
uint64_t mem_size;

// print block info
void print_block(void* block) {
    uint64_t *header, *footer;
    uint64_t bsize;

    header = GET_HEADER(block);
    bsize = GET_SIZE(header);
    footer = GET_FOOTER(block, bsize);
    printf("block at %p, size %lu, header %lx, footer %lx\n", 
        block, bsize, *header, *footer);
}

// first fit block
void* find_fit(size_t asize) {
    void *block, *bucket;
    uint64_t *header, *footer;
    size_t bsize;
    
    #ifdef DEBUG 
    printf("traverse blocks\n");
    #endif

    // get free list
    bucket = heap_start;
    while (bucket + BUCKET_SIZE < dummy_header) {
        if (asize <= GET_SIZE((uint64_t*)bucket)) {
            break;
        }
        bucket += BUCKET_SIZE;
    }
    block = GET_NEXT(bucket);
    
    while (block != bucket) {
        #ifdef DEBUG
        printf("    ");
        print_block(block);
        #endif
        header = GET_HEADER(block);
        bsize = GET_SIZE(header);
        assert(bsize > 0);
        footer = GET_FOOTER(block, bsize);
        if (!GET_ALLOC(header) &&  bsize >= asize) {
            // get block from free list
            TAKE_OUT_BLOCK_FROM_FREE_LIST(block);
            // set alloc bit
            SET_ALLOC_BIT(header);
            SET_ALLOC_BIT(footer);
            return block;
        }
        block = GET_NEXT(block);
    }
    return NULL;
}

static inline void insert_block_to_free_list(void* block, size_t bsize) {
    void *bucket = heap_start;
    if (bsize <= (1 << 6)) {
        
    } else if (bsize <= (1 << 10)) {
        bucket += BUCKET_SIZE;
    } else if (bsize <= (1 << 11)) {
        bucket += 2 * BUCKET_SIZE;
    } else if (bsize <= (1 << 12)) {
        bucket += 3 * BUCKET_SIZE;
    } else {
        bucket += 4 * BUCKET_SIZE;
    }
    INSERT_BLOCK_TO_FREE_LIST(block, bucket);
}

// coalesce free block
void coalesce_block(void* block) {
    void *prev_block, *next_block;
    uint64_t *header, *footer, *next_header, *prev_footer;

    header = GET_HEADER(block);
    size_t bsize = GET_SIZE(header);
    footer = GET_FOOTER(block, bsize);
    next_header = footer + 1;
    prev_footer = header - 1;
    
    if (!GET_ALLOC(prev_footer)) {
        // get prev block
        prev_block = block - GET_SIZE(prev_footer);
        #ifdef DEBUG
        printf("coalesce prev block\n   ");
        print_block(prev_block);
        #endif        
        // take prev block from free list
        TAKE_OUT_BLOCK_FROM_FREE_LIST(prev_block);
        bsize += GET_SIZE(prev_footer);
        if (!GET_ALLOC(next_header)) {
            next_block = next_header;
            #ifdef DEBUG
            printf("coalesce next block\n   ");
            print_block(next_block);
            #endif              
            // take next block from free list
            TAKE_OUT_BLOCK_FROM_FREE_LIST(next_block);
            bsize += GET_SIZE(next_header);
        }
        MAKE_HEADER(prev_block, bsize, 0);
        MAKE_FOOTER(prev_block, bsize, 0);
        // insert coalesced block
        insert_block_to_free_list(prev_block, bsize);
    }
    else if (!GET_ALLOC(next_header)) {
        // get next block
        next_block = next_header;
        #ifdef DEBUG
        printf("coalesce next block\n   ");
        print_block(next_block);
        #endif  
        // take next block from free list
        TAKE_OUT_BLOCK_FROM_FREE_LIST(next_block);
        bsize += GET_SIZE(next_header);
        MAKE_HEADER(block, bsize, 0);
        MAKE_FOOTER(block, bsize, 0);
        // insert coalesced block
        insert_block_to_free_list(block, bsize);
    } else {
        UNSET_ALLOC_BIT(header);
        UNSET_ALLOC_BIT(footer);
        // insert coalesced block
        insert_block_to_free_list(block, bsize);
    }    
}

// split allocated block
void split_block(void* block, size_t asize) {
    void *next_block;
    uint64_t *header = GET_HEADER(block);
    uint64_t bsize = GET_SIZE(header);

    if ((bsize - asize) >= MIN_BLOCK_SIZE) {
        MAKE_HEADER(block, asize, 1);
        MAKE_FOOTER(block, asize, 1);
        next_block = GET_NEXT_BLOCK(block, asize); // now the block size is asize
        MAKE_HEADER(next_block, (bsize - asize), 0);
        MAKE_FOOTER(next_block, (bsize - asize), 0);
        // coalesce next block
        coalesce_block(next_block);
        #ifdef DEBUG
        printf("split 2 block\n    ");
        print_block(block);
        printf("    ");
        print_block(next_block);
        #endif
    }
}

/* 
 * mm_init - initialize the malloc package.
 * heap: | dummy header | next | prev | mem | dummy footer | NO!!!!
 * heap: | dummy header | next | prev | dummy header | mem | dummy footer | YES!!!!
 */
int mm_init(void) {
    // alloacate heap
    mem_size = INIT_MEM_SIZE;
    heap_size = ALIGN(INIT_MEM_SIZE + TOTAL_BUCKET_SIZE + 2 * HEADER_SIZE + SIZE_T_SIZE); 
    heap_start = mem_sbrk(heap_size);
    if (heap_start == NULL) {
        return 0;
    }    
    heap_end = heap_start + heap_size;

    // make dummy header
    dummy_header = heap_start + TOTAL_BUCKET_SIZE;
    MAKE_HEADER(dummy_header, 0, 1);

    // make mem header and footer
    mem_start = dummy_header + HEADER_SIZE;
    mem_end = mem_start + mem_size;
    MAKE_HEADER(mem_start, mem_size, 0);
    MAKE_FOOTER(mem_start, mem_size, 0);

    // make dummy footer
    dummy_footer = mem_end;
    MAKE_HEADER(dummy_footer, 0, 1);

    // initialize bucket free list
    void* bucket = heap_start;
    MAKE_HEADER(bucket, 1 << 6, 1);
    GET_NEXT(bucket) = GET_PREV(bucket) = bucket;
    bucket += BUCKET_SIZE;
    MAKE_HEADER(bucket, 1 << 10, 1);
    GET_NEXT(bucket) = GET_PREV(bucket) = bucket;
    bucket += BUCKET_SIZE;
    MAKE_HEADER(bucket, 1 << 11, 1);
    GET_NEXT(bucket) = GET_PREV(bucket) = bucket;
    bucket += BUCKET_SIZE;
    MAKE_HEADER(bucket, 1 << 12, 1);
    GET_NEXT(bucket) = GET_PREV(bucket) = bucket;
    bucket += BUCKET_SIZE;
    MAKE_HEADER(bucket, 1 << 31, 1);
    GET_NEXT(bucket) = GET_PREV(bucket) = bucket;

    insert_block_to_free_list(mem_start, mem_size);

    return 1;
}

void* mm_extend(size_t asize) {
    void *ext_heap_start = mem_sbrk(asize);
    #ifdef DEBUG
    printf("original heap end is %p, extend heap addr is %p\n", heap_end, ext_heap_start);
    #endif
    assert(ext_heap_start == heap_end);
    heap_end += asize;
    mem_end += asize;

    void *block = dummy_footer;
    MAKE_HEADER(block, asize, 1);
    MAKE_FOOTER(block, asize, 1);

    dummy_footer = GET_NEXT_BLOCK(block, asize);
    assert(dummy_footer == mem_end);
    MAKE_HEADER(dummy_footer, 0, 1);

    return block;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    #ifdef DEBUG
    printf("malloc size of %lu\n", size);
    #endif

    // total size of block
    int asize = ALIGN(max(size + EXTRA_SIZE, MIN_BLOCK_SIZE) + SIZE_T_SIZE);
    void* block = find_fit(asize);

    if (block == NULL) {
        block = mm_extend(asize);
    }

    #ifdef DEBUG
    printf("malloc find fit block\n    ");
    print_block(block);
    #endif

    split_block(block, asize);
    return GET_PAYLOAD(block);
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr) {
    #ifdef DEBUG
    printf("free %p\n", ptr);
    #endif

    void *block = GET_BLOCK_FROM_PAYLOAD(ptr);
    coalesce_block(block);
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    void *oldptr = ptr;
    void *newptr;
    size_t copySize;
    
    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - SIZE_T_SIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;
}
