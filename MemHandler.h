#include <stdint.h>



void print_global_head();

//Struct to keep meta information of blocks of memory
struct block_meta {
	size_t size;
	struct block_meta *next;
	uint8_t free;
	int magic;    // For debugging only. TODO: remove this in non-debug mode.
	char data[1];
};

void *global_base;

//Changing name of the struct to just block_meta
typedef struct block_meta block_meta;

//macro for getting the size of the meta info struct
#define META_SIZE sizeof(block_meta)

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

//SPLIT_IN_MALLOC()
//Does same as split() but only for malloc calls in which
//memory is reused
void split_in_malloc(block_meta * block1, size_t size);

//FIND_FREE_BLOCK()
//(goes through the linked list starting at global base and
// find a free block. Returns its address or NULL if not found
block_meta *find_free_block(block_meta **last, size_t size);

//REQUEST_SPACE()
//Requests space
block_meta *request_space(block_meta* last, size_t size);

//MALLOC()
//Allocates memory from the heap. First, it requests already
//available space, if none, then it increases the heap size.
//It returns a pointer to the allocated memory in the heap.
void *malloc(size_t size);

//GET_BLOCK_PTR
//Gets the pointer of the meta block
block_meta *get_block_ptr(void *ptr);

//FREE()
//Frees memory from the heap starting at the given address
void free(void *ptr);

//REALLOC()
//Realocates memory. If theres big enough free memory available
//it will use that first.
void *realloc(void *ptr, size_t size);
