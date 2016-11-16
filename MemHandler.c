//////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmsc257-assign2-support.c
//  Description    : Support file that implements my improved versions of calloc()
//                   malloc() realloc() and free(). It also contains helping
//                   functions for both printing analysis information to the screen
//                   and to assists with utilities of the more important functions.
//
//
//   Author        : Armando L. Toledo
//   Last Modified : 10/28/2016
//

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include "MemHandler.h"
#define align_to_4_bytes(x) (((x-1)>>2)<<2)+4   //alignment implemented in demo is size 4 bytes
#define align_to_8_bytes(x) (((x-1)>>3)<<3)+8
#define align_to_2_bytes(x) (((x-1)>>1)<<1)+2

//Initialize Global_Base (head for the linked list)

//Declaring the head for the linked list
void *global_base = NULL;


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
            traverser->size = traverser->size + META_SIZE + traverser->next->size; //1st block takes full size
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
    printf("Memory leaks: %d bytes\n", memory_leaks());
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
        memory_leaks += META_SIZE;     //add every struct and
        if(traverser->free)                     //if memory is free
        {
            memory_leaks += traverser->size;    //add memory
        }
        traverser = traverser->next;
    }
    return memory_leaks;
}




//MEMORY_LEAKS_v2()
//Shows memory leaks. Memory links include the size of the linked
//list and the free bytes of memory underneath the program break
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int memory_leaks_v2(void * starting_prg_brk, void * current_prg_brk, int allocated_bytes)
{
    int memory_leaks_v2 = 0;
    int allocated_heap_size = 0;

    allocated_heap_size = (uint64_t)(current_prg_brk) - (uint64_t)(starting_prg_brk);
    memory_leaks_v2 = allocated_heap_size - allocated_bytes - 1024; //1024 is the size of the string buffer c puts on the heap

    return memory_leaks_v2;
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
    if(traverser)
    {
        printf("(%zu %d)", traverser->size, traverser->free);
        traverser = traverser->next;
    }
    while (traverser)
    {
        printf("  (%zu %d) ", traverser->size, traverser->free);
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
        //if free and smaller than prev best fit, and bigger than the required size (Best Fit)
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



//SPLIT()
//Splits a block if you need less memory than it offers
//Creates a new meta_block for the new block. Keeps contiguous
//order in the linked list.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void split(void *ptr, size_t size)
{
    block_meta * block1 = get_block_ptr(ptr);
    if(block1->size - size < META_SIZE + 4)   // if no space for at least a meta + 4 bytes
    {                                                  // then don't split
        return;
    }

    block_meta * block2 = ptr + size;    //dont need to allocate space because we are already inside the heap
    block2->size = block1->size - size - META_SIZE;
    block1->size = size;
    block2->next = block1->next;
    block1->next = block2;
    block2->free = 1;
    block1->free = 0;
}



//MALLOC()
//Allocates memory from the heap. First, it requests already
//available space, if none, then it increases the heap size.
//It returns a pointer to the allocated memory in the heap.
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void *malloc(size_t size)
{
	 block_meta *block;
    size_t aligned_size = align_to_4_bytes(size);       //Align size-----------------------------------
	if (aligned_size <= 0) {
		return NULL;
	}

	if (!global_base) {                                 // First call.
		block = request_space(NULL, aligned_size);
		if (!block) {
			return NULL;
		}
		global_base = block;
	}
	else {                                              //Else: not first call
		block_meta *last = global_base;
		block = find_free_block(&last, aligned_size);   //Find free block (best fit algorithm)---------------------
		if (!block) {                                   //If failed to find free block.
			block = request_space(last, aligned_size);  //  Request space and increase program break
			if (!block) {
				return NULL;
			}
		}
		else {                                          //Found free block
			// TODO: consider splitting block here.
			split(block + 1, aligned_size);  //passing block + 1 because split will call get_block_ptr() DONE-----------------------------------

			block->free = 0;
			block->magic = 0x77777777;
			merge_all_adjacent();                       //Merge if needed-------------------------------
		}
	}

	return(block + 1);
}



//GET_BLOCK_PTR()
//Gets the pointer of the meta block
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 block_meta *get_block_ptr(void *ptr)
{
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
	merge_all_adjacent();       //Merge if needed--------------------------------------------
}



//CALLOC()
//Does the same as malloc() but zeroes the memory first
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void *calloc(size_t nelem, size_t elsize)
{
    size_t aligned_size = align_to_4_bytes(elsize * nelem);

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
		// We have enough space. Could free some once we implement split.
		split(ptr, aligned_size);    //Split if possible-------------------------------------------
		merge_all_adjacent();                   //Merge if needed-------------------------------------------
		return ptr;
	}

	// Need to really realloc. Malloc new space and free old space.
	// Then copy old data to new space.
	void *new_ptr;
        new_ptr = malloc(aligned_size);
	if (!new_ptr) {
		return NULL;
	}
	memcpy(new_ptr, ptr, block_ptr->size);
	free(ptr);
	return new_ptr;
}
