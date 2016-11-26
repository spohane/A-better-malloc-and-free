//mymalloc.c
//sp1106,sed96
#include "mymalloc.h"
static char myblock[5000];
static char blocks[sizeof(myblock)]; //size of the block entries 
static void *blockentry[sizeof(myblock)/sizeof(struct Info)+1] = {0}; //pointers to all of the block enteries
static const int blocksize = 1+ sizeof(myblock)/sizeof(struct Info);  
static int indexfree() {
	int x=0;
	while ( x < blocksize){
		if (blockentry[x] == 0) 
			return x;
		x++;
	}
	return 1; 
}

void *mymalloc(unsigned int size, char *file, int line)
{
	static struct Info *first;
	static int init = 0;
	struct Info *in, *nextblock;
	//error to handle malloc(0)
	if (size == 0) {
		fprintf(stderr, "ERROR- Can not allocate 0 bytes: __file__ '%s' on __line__ '%d'\n", file, line);
		return NULL;
	}
	//initialize the previous, next, and size of the first block
	if(!init)	
	{
		first = (struct Info*) blocks;
		first->previous = 0;
		first->next = 0;
		first->size = sizeof(myblock) - sizeof(struct Info);
		first->allocated = 1;
		//set the initial value as 1 to move onto allocating the next block
		init = 1;
		blockentry[indexfree()] = blocks;
	}
	in = first;
	//make sure that the first block that been allocated in the previous if statement
	while(in!=0)
	{
		//make sure that the next block has not been allocation and that it has enough memory space
		if(!in->allocated || in->size < size) {
			in = in->next;
		}
		//make sure if this block is large enough to allocate memory, but checks to see if it has no space after it
		else if(in->size < (size + sizeof(struct Info))) {
			in->allocated = 0;
			//stops creating more blocks in memory and returns this block
			return (char*)in + sizeof(struct Info);
		}
		else {
			//else if there is enough memory for more allocations, make enough memory for the next metadata and block
			nextblock = (struct Info*)((char*)in + sizeof(struct Info) + size);
			blockentry[indexfree()] = nextblock;
			//set the next, previous, and size of the next block 
			nextblock->next = in->next;
			nextblock->previous = in;
			nextblock->size = in->size - sizeof(struct Info) - size;
			nextblock->allocated = 1;
			in->next = nextblock;
			in->size = size;
			in->allocated = 0;
			return (char*)in + sizeof(struct Info);
		}
	}
	//return NULL if no more memory allocations can be made
	return NULL;
}


void myfree(void *ptr, char *file, int line)
{
	struct Info *next, *point, *previous;
	//if a pointer has not been malloced, it is a null pointer, and a null pointer can not be freed
	if (ptr == NULL) {
		printf("ERROR- Pointer is null:  __file__  '%s' on __line__ '%d'\n", file, line);
		return;
	}
	point = (struct Info*)((char*)ptr - sizeof(struct Info));
	int x;
	int malloced = 0;
	//check to see if the pointer has already been malloced 
	for ( x = 0; x < blocksize; x++) {
		if (!point->allocated && point == blockentry[x] ) {
			malloced = 1; 
			break;
		}
	}
	//if pointer has not been malloced, return error message for attempting to free memory not malloced
	if (!malloced) {
		printf("ERROR- Freeing memory not malloced: __file__ '%s' on __line__ '%d'\n", file, line);
		return;
	}
	//check to see if the previous block is free and combine the current free block to the previous one
	if((previous = point->previous) != 0 && previous->allocated )
	{
		previous->size += sizeof(struct Info) + point->size;
		blockentry[x] = 0; 
	}
	else
	{ 
		point->allocated = 1;
		previous = point;
	}
	//combine with the next block if it is free
	if((next = point->next) != 0 && next->allocated)
	{
		previous->size += sizeof(struct Info) + next->size;
		previous->next = next->next;
		//once every block is combined, make the blockentries empty
		for (x = 0; x < blocksize; x++) {
			if (next == blockentry[x]) {
				blockentry[x] = 0; 
				break;
			}
		}
	}
}
