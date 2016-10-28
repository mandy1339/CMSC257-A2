#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "MemHandler.h"
#define align_to_4_bytes(x) (((x-1)>>2)<<2)+4
#define align_to_8_bytes(x) (((x-1)>>3)<<3)+8
#define align_to_2_bytes(x) (((x-1)>>1)<<1)+2

//Initialize Global_Base (head for the linked list)

//Declaring the head for the linked list
void *global_base = NULL;

void print_global_head()
{
    printf("\npointer o global base: %p\n", global_base);
}




//MERGE_ADJACENT()
//Loops once through the linked list and merges adjacent blocks
uint8_t merge_adjacent()
{
    block_meta * traverser = global_base;   //start at the base
    uint8_t more_to_merge = 0; //initially we don't know if merging will happen
    while (traverser)       //loope once through the linked list and merge adjacents
    {
        // block has next  AND      it's free      AND     next block is free
        if(traverser->next && traverser->free == 1 && traverser->next->free == 1)
       {
            traverser->size = traverser->size + sizeof(block_meta) + traverser->next->size; //1st block takes full size
            traverser->next = traverser->next->next;  //1st block points to next's next
            more_to_merge = 1;                  //more to merge = true, keep merging
        }
        traverser = traverser->next;        //move through the loop
    }
    return more_to_merge;   //return boolean
}

//MERGE_ALL_ADJACENT()
//Merges adjacent all adjacent blocks repeatedly until there is
//no more to merge
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void merge_all_adjacent()
{
    uint8_t more_to_merge = 1;             //boolean initially true so we can access the loop

    while(more_to_merge)
    {
        more_to_merge = merge_adjacent();   //returns true if it merged or false if it didn't
    }
}



//SHOW_STATE()
//Shows state of the linked list at the time it's called
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void show_state()
{
    print_metas();
    printf("Top of the heap is at %p\n", sbrk(0));
    printf("Memory leaks: %d bytes\n\n", memory_leaks());
}



//MEMORY_LEAKS()
//Shows memory leaks. Memory links include the size of the linked
//list and the free bytes of memory underneath the program break
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int memory_leaks()
{
    int memory_leaks = 0;
    block_meta * traverser = global_base;
    while (traverser)
    {
        memory_leaks += sizeof(block_meta);     //add every struct
        if(traverser->free)                     //if memory is free
        {
            memory_leaks += traverser->size;    //add memory
        }
        traverser = traverser->next;
    }
    return memory_leaks;
}



//PRINT_METAS
//Traverses the linked list of meta_block's and prints
//their information
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void print_metas()
{
    printf("\nBlocks: ");
    block_meta * traverser = global_base;
    while (traverser)
    {
        printf("(%zu %d), ", traverser->size, traverser->free);
        traverser = traverser->next;
    }
    printf("\n");
}


//FIND_FREE_BLOCK()
//(goes through the linked list starting at global base and
// find a free block. Returns its address or NULL if not found
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
block_meta *find_free_block(block_meta **last, size_t size)    //<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
{                                                               //implement a best fit algorithm instead of a first fit algorithm
	block_meta * traverser = global_base;                       //DONE
	block_meta * best_fit;
	while (traverser && !(traverser->free && traverser->size >= size))   //find first fit (best fit will follow)
	{
		*last = traverser;
		traverser = traverser->next;
	}

    best_fit = traverser;
    while(traverser)
    {
        //if free and smaller than prev best fit, and bigger than the required size
        if(traverser->free && traverser->size < best_fit->size && traverser->size >= size)
        {
            best_fit = traverser;
        }
        traverser = traverser->next;
    }
	return best_fit;
}



//REQUEST_SPACE()
//Requests space
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
block_meta *request_space(block_meta* last, size_t size) {
	block_meta *block;
	block = sbrk(0);
	void *request = sbrk(size + META_SIZE);
	assert((void*)block == request); // Not thread safe.
	if (request == (void*)-1) {
		return NULL; // sbrk failed.
	}

	if (last) { // NULL on first request.
		last->next = block;
	}
	block->size = size;
	block->next = NULL;
	block->free = 0;
	block->magic = 0x12345678;
	return block;
}

//Our malloc is really wasteful if we try to re-use an existing block and we don’t need all of the space.
//Implement a function that will split up blocks so that they use the minimum amount of space necessary

//SPLIT()
//Splits a block if you need less memory than it offers
//Creates a new meta_block for the new block. Keeps contiguous
//order in the linked list.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void split_in_realloc(void *ptr, size_t size)
{
    block_meta * block1 = get_block_ptr(ptr);
    if(block1->size - sizeof(block_meta) <= sizeof(block_meta) + 1);   // Split ONLY if there will be space for at least a meta + 1 byte
        return;
    block_meta * block2 = ptr + size;    //dont need to allocate space because we are already inside the heap
    block2->size = block1->size - size - sizeof(block_meta);
    block1->size = size;
    block2->next = block1->next;
    block1->next = block2;
    block2->free = 1;
    block1->free = 0;

}

//SPLIT_IN_MALLOC()
//Splits block into 2 blocks if we are reusing a block but not filling it up
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void split_in_malloc(block_meta * block1, size_t size)
{
    if(block1->size - size <= sizeof(block_meta) + 1)   // Split ONLY if there will be space for at least a meta + 1 byte
    {
        printf("\nblock size: %d, block free: %d\n", block1->size, block1->free);
        printf("\nblock1->size - size: %d <= sizeof(block_meta) + 1 %d\n", block1->size - size, sizeof(block_meta) + 1);
        return;
    }
    else
    {
        block_meta * block2 = (void *)block1 + sizeof(block_meta) + size; // Needed to cast block1 to void * to be
        block2->size = block1->size - size - sizeof(block_meta);          //able to do pointer arithmetic in byte
        block1->size = size;                                              //size instead of in sizeof(block_meta) size.
        block2->next = block1->next;
        block1->next = block2;
        block2->free = 1;
        block1->free = 0;
    }
}

//MALLOC()
//Allocates memory from the heap. First, it requests already
//available space, if none, then it increases the heap size.
//It returns a pointer to the allocated memory in the heap.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void *malloc(size_t size) {
	 block_meta *block;
	// TODO: align size?                    <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    size_t aligned_size = align_to_4_bytes(size);
	if (aligned_size <= 0) {
		return NULL;
	}

	if (!global_base) { // First call.
		block = request_space(NULL, aligned_size);
		if (!block) {
			return NULL;
		}
		global_base = block;
	}
	else {
		block_meta *last = global_base;
		block = find_free_block(&last, aligned_size);
		if (!block) { // Failed to find free block.
			block = request_space(last, aligned_size);
			if (!block) {
				return NULL;
			}
		}
		else {      // Found free block
			// TODO: consider splitting block here.   <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
			split_in_malloc(block, aligned_size);           //DONE

			block->free = 0;
			block->magic = 0x77777777;
			merge_all_adjacent();                       //-----------------------------------------------
		}
	}

	return(block + 1);
}



//GET_BLOCK_PTR()
//Gets the pointer of the meta block
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 block_meta *get_block_ptr(void *ptr) {
	return (block_meta*)ptr - 1;
}



 //FREE()
 //Frees memory from the heap starting at the given address
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void free(void *ptr)
{
	if (!ptr) {
		return;
	}

	// TODO: consider merging blocks once splitting blocks is implemented.  <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	block_meta* block_ptr = get_block_ptr(ptr);
	assert(block_ptr->free == 0);
	assert(block_ptr->magic == 0x77777777 || block_ptr->magic == 0x12345678);
	block_ptr->free = 1;
	block_ptr->magic = 0x55555555;
	merge_all_adjacent();       //-----------------------------------------------
}



//CALLOC()
//Does the same as malloc() but zeroes the memory first
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void *calloc(size_t nelem, size_t elsize) {
    size_t aligned_size = align_to_4_bytes(elsize * nelem);// TODO: check for overflow.      <<<<<<<<<<<<<<<<<<<<<<<<<<

	void *ptr = malloc(aligned_size);
	memset(ptr, 0, aligned_size);
	return ptr;
}




//REALLOC()
//Realocates memory. If theres big enough free memory available
//it will use that first.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void *realloc(void *ptr, size_t size)
{
    size_t aligned_size = align_to_4_bytes(size);
	if (!ptr) {
		// NULL ptr. realloc should act like malloc.
		return malloc(aligned_size);
	}

	block_meta* block_ptr = get_block_ptr(ptr);
	if (block_ptr->size >= aligned_size) {
		// We have enough space. Could free some once we implement split.   >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		split_in_realloc(ptr, aligned_size);//-------------------------------------------
		merge_all_adjacent();//-------------------------------------------
		return ptr;
	}

	// Need to really realloc. Malloc new space and free old space.
	// Then copy old data to new space.
	void *new_ptr;
        new_ptr = malloc(aligned_size);
	if (!new_ptr) {
		return NULL; // TODO: set errno on failure.
	}
	memcpy(new_ptr, ptr, block_ptr->size);
	free(ptr);
	return new_ptr;
}
