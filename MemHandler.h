//////////////////////////////////////////////////////////////////////////////////
//
//  File           : cmsc257-assign2-support.h
//  Description    : Header file that contains the function prototypes used in both
//                   cmsc257-assign2.c and cmsc257-assign2-support.c
//                   it also declares the head of the linked list and the struct
//                   used as a node.
//
//   Author        : Armando L. Toledo
//   Last Modified : 10/28/2016
//
//

#include <stdint.h>

//Struct to keep meta information of blocks of memory
struct block_meta {
	size_t size;
	struct block_meta *next;
	int free;
	int magic;    // For debugging only. TODO: remove this in non-debug mode.
};

//Declaring head of the linked list of block_meta's
void *global_base;

//Changing name of the struct to just block_meta
typedef struct block_meta block_meta;

//macro for getting the size of the meta info struct
#define META_SIZE sizeof(block_meta)


//~~~~~~~~ IMPORTANT FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//MALLOC()
//Allocates memory from the heap. First, it requests already
//available space, if none, then it increases the heap size.
//It returns a pointer to the allocated memory in the heap.
void *malloc(size_t size);

//REALLOC()
//Realocates memory. If theres big enough free memory available
//it will use that first.
void *realloc(void *ptr, size_t size);

//FREE()
//Frees memory from the heap starting at the given address
void free(void *ptr);

//CALLOC()
//Does the same as malloc() but zeroes the memory first
void *calloc(size_t nelem, size_t elsize);


//~~~~~~~~ HELPING FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

//MERGE_ADJACENT()
//Loops once through the linked list and merges adjacent blocks
uint8_t merge_adjacent();

//MERGE_ALL_ADJACENT()
//Merges adjacent all adjacent blocks repeatedly until there is
//no more to merge
void merge_all_adjacent();

//PRINT_METAS
//Traverses the linked list of meta_block's and prints
//their information
void print_metas();

//SPLIT()
//Splits a block if you need less memory than it offers
//Creates a new meta_block for the new block. Keeps contiguous
//order in the linked list.
void split(void *ptr, size_t size);


//FIND_FREE_BLOCK()
//(goes through the linked list starting at global base and
// find a free block. Returns its address or NULL if not found
block_meta *find_free_block(block_meta **last, size_t size);

//REQUEST_SPACE()
//Requests space
block_meta *request_space(block_meta* last, size_t size);

//GET_BLOCK_PTR
//Gets the pointer of the meta block
block_meta *get_block_ptr(void *ptr);

//SHOW_STATE()
//Shows state of the linked list at the time it's called
void show_state();

//MEMORY_LEAKS()
//Shows memory leaks. Memory links include the size of the linked
//list and the free bytes of memory underneath the program break
int memory_leaks();

//MEMORY_LEAKS_v2()
//Shows memory leaks. Memory links include the size of the linked
//list and the free bytes of memory underneath the program break
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int memory_leaks_v2(void * starting_prg_brk, void * current_prg_brk, int allocated_bytes);

