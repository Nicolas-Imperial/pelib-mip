#include <stdio.h>
#include <stdlib.h>
#include <pelib/malloc.h>

#if 0
#define debug(var) printf("[%s:%s:%d] %s = \"%s\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_addr(var) printf("[%s:%s:%d] %s = \"%p\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_int(var) printf("[%s:%s:%d] %s = \"%d\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#define debug_size_t(var) printf("[%s:%s:%d] %s = \"%zu\"\n", __FILE__, __FUNCTION__, __LINE__, #var, var); fflush(NULL)
#else
#define debug(var)
#define debug_addr(var)
#define debug_int(var)
#define debug_size_t(var)
#endif

void
pelib_mem_malloc_init(pelib_malloc_queue_t* spacep, void* mem, size_t size)
{
	spacep->tail = (pelib_malloc_block_t*) malloc(sizeof(pelib_malloc_block_t));
	debug_size_t(size);
	spacep->tail->free_size = size;
	spacep->tail->space = mem;
	/* make a circular list by connecting tail to itself */
	spacep->tail->next = spacep->tail;
	spacep->mem = mem;
}

void*
pelib_mem_malloc(pelib_malloc_queue_t *spacep, size_t size, int alignment)
{
	// simple memory allocator, loosely based on public domain code developed by
	// Michael B. Allen and published on "The Scripts--IT /Developers Network".
	// Approach: 
	// - maintain linked list of pointers to memory. A block is either completely
	//	malloced (free_size = 0), or completely free (free_size > 0).
	//	The space field always points to the beginning of the block
	// - malloc: traverse linked list for first block that has enough space
	// - free: Check if pointer exists. If yes, check if the new block should be
	//	merged with neighbors. Could be one or two neighbors.

	pelib_malloc_block_t *b1, *b2, *b3;	 // running pointers for blocks

	if (size == 0) 
	{
		debug_addr(NULL);
		return NULL;
	}

	// always first check if the tail block has enough space, because that
	// is the most likely. If it does and it is exactly enough, we still
	// create a new block that will be the new tail, whose free space is
	// zero. This acts as a marker of where free space of predecessor ends
	b1 = spacep->tail;
	// If we request (alignment > 1) memory, then it's ok if the tail free memory can handle the size requested. Otherwise, we need to check if the tail can handle the size requested even after skipping as many bytes as necessary to get some (alignment > 1) address.
	/*
	debug_addr(b1);
	debug_size_t(b1->free_size);
	debug_size_t(size);
	debug_int(b1->free_size >= size);
	debug_int(alignment);
	debug_int(alignment > 1);
	debug_int((b1->free_size >= size && !(alignment > 1)));
	debug_addr(b1->space);
	debug_size_t(b1->free_size);
	*/
	if ((b1->free_size >= size && !(alignment > 1)) || ((alignment > 1) && ((((size_t)b1->space % alignment) == 0) || (b1->free_size - alignment + ((size_t)b1->space % alignment) >= size))))
	{
		// need to insert new block; new order is: b1->b2 (= new tail)
		b2 = (pelib_malloc_block_t*) malloc(sizeof(pelib_malloc_block_t));
		b2->next = b1->next;
		b1->next = b2;
		b2->free_size = b1->free_size - size;
		b2->space  = b1->space + size;
		
		// Display the block used  
		if(!(alignment > 1) || ((size_t)b1->space % alignment) == 0)
		{
			// Display the new block that holds the unused memory


			// need to update the tail
			spacep->tail = b2;

			b1->free_size = 0;
			debug_addr(spacep->mem);
			debug_size_t(b1->space - spacep->mem);
			debug_size_t(size);
			return b1->space;
		}
		else
		{
			// need to update the tail
			spacep->tail = b2;

			b3 = (pelib_malloc_block_t*) malloc(sizeof(pelib_malloc_block_t));
			b3->next = b1->next;
			b1->next = b3;
			b3->space = b1->space + alignment - ((size_t)b1->space % alignment);
			b1->free_size = alignment - ((size_t)b1->space % alignment);
			b2->space += alignment - ((size_t)b1->space % alignment);
			b2->free_size -= alignment - ((size_t)b1->space % alignment);

			// Display the new block that holds the unused memory

			// Display the new block holding memory skipped for alignment

			// Display the final block allocated 

			b3->free_size = 0;

			/*
			debug_addr(spacep->tail);
			debug_addr(spacep->tail->next);
			debug_addr(spacep->tail->space);
			debug_addr(spacep->tail->next->space);
			*/
			if(spacep->tail->next >= spacep->tail)
			{
				debug_size_t(spacep->tail->next->space - spacep->tail->space);
			}
			else
			{
				debug_size_t(spacep->tail->space - spacep->tail->next->space);
			}
			debug_addr(b3->space);
			return b3->space;
		}
	}

	// tail didn't have enough space; loop over whole list from beginning
	// As for above, stop at the first free space big enough to handle the request if no (alignment > 1) memory is requested
	// Otherwise, check each space after memory alignment
	while   (((b1->next->free_size < size) && !(alignment > 1)) || ((alignment > 1) && (((b1->free_size - alignment + ((size_t)b1->space % alignment) < size) && (((size_t)b1->space % alignment) != 0)) || ((((size_t)b1->space % alignment) == 0) && (b1->next->free_size < size)))))
	{
		if (b1->next == spacep->tail)
		{
			return NULL; // we came full circle
		}
		b1 = b1->next;
	}

	b2 = b1->next;

	// Display the block selected

	// If memory alignment is requested and the block is strictly bigger than requested, then we need to skip some bytes before allocating
	// If the block is not strictly bigger than requested, then we know that the free block is already (alignment > 1) because otherwise the loop above 
	// would have returned an allocation failure
	if (b2->free_size > size && (alignment > 1))
	{
		b3 = (pelib_malloc_block_t*) malloc(sizeof(pelib_malloc_block_t));
		b3->next = b2->next;
		b2->next = b3;
		b3->free_size = b2->free_size - alignment + ((size_t)b2->space % alignment);
		b2->free_size = b2->free_size - b3->free_size;
		b3->space = b2->space + alignment - ((size_t)b2->space % alignment);

		// Display the block that handles the memory skipped

		b2 = b3;
	}

	// If, perhaps after having split b2 into b2 and b3, b2 is still too big, then we need to create yet another block.
	if (b2->free_size > size)
	{
		// split block; new block order: b1->b2->b3
		b3 = (pelib_malloc_block_t*) malloc(sizeof(pelib_malloc_block_t));
		b3->next = b2->next; // reconnect pointers to add block b3
		b2->next = b3;
		b3->free_size = b2->free_size - size; // b3 gets remainder free space
		b3->space = b2->space + size; // need to shift space pointer
	}


	// Now b2 is the exact size and (alignment > 1) as requested
	b2->free_size = 0; // block b2 is completely used
	debug_addr(b2->space);
	return b2->space;
}

void
pelib_mem_free(pelib_malloc_queue_t *spacep, void *ptr)
{
	pelib_malloc_block_t *b1, *b2, *b3;	// running block pointers
	int j1, j2;			// booleans determining merging of blocks

	// loop over whole list from the beginning until we locate space ptr
	b1 = spacep->tail;
	while (b1->next->space != ptr && b1->next != spacep->tail)
	{
		b1 = b1->next;
	}

	// b2 is target block whose space must be freed
	b2 = b1->next;
	// tail either has zero free space, or hasn't been malloc'ed
	if (b2 == spacep->tail)
	{
		return;
	}

	// reset free space for target block (entire block)
	b3 = b2->next;
	b2->free_size = b3->space - b2->space;

	// determine with what non-empty blocks the target block can be merged
	j1 = (b1->free_size > 0 && b1 != spacep->tail); // predecessor block
	j2 = (b3->free_size > 0 || b3 == spacep->tail); // successor block

	if (j1)
	{
		if (j2)
		{
			// splice all three blocks together: (b1,b2,b3) into b1
			b1->next = b3->next;
			b1->free_size += b3->free_size + b2->free_size;
			if (b3 == spacep->tail)
			{
				spacep->tail = b1;
			}
			free(b3);
		}
		else
		{
			// only merge (b1,b2) into b1
			b1->free_size += b2->free_size;
			b1->next = b3;
		}
		free(b2);
	}
	else
	{
		if (j2)
		{
			// only merge (b2,b3) into b2
			b2->next = b3->next;
			b2->free_size += b3->free_size;
			if (b3 == spacep->tail)
			{
				spacep->tail = b2;
			}
			free(b3);
		}
	}
}

