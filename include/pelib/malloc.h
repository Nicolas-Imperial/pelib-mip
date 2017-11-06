#ifndef PELIB_MALLOC_H
#define PELIB_MALLOC_H

typedef struct mem_block
{
	void* space;			// pointer to space for data in block             
	size_t free_size;		// actual free space in block (0 or whole block)  
	struct mem_block *next;		// pointer to next block in circular linked list 
} pelib_malloc_block_t;

typedef struct
{
	pelib_malloc_block_t *tail;		// "last" block in linked list of blocks
	void* mem;
} pelib_malloc_queue_t;

void pelib_mem_malloc_init(pelib_malloc_queue_t* spacep, void* mem, size_t size);
void* pelib_mem_malloc(pelib_malloc_queue_t *spacep, size_t size, int alignment);
void pelib_mem_free(pelib_malloc_queue_t *spacep, void *ptr);

#endif
