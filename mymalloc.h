//mymalloc.h
//sp1106,sed96
#ifndef _mymalloc_h_
#define _mymalloc_h_
#include <stdlib.h>
#include <stdio.h>

#define malloc(x) mymalloc(x, __FILE__ , __LINE__ )
#define free(ptr) myfree(ptr, __FILE__ , __LINE__ )



void *mymalloc(unsigned int size, char *file, int line);
void myfree(void *ptr, char *file, int line);
struct Info
{
	short allocated;
	int size;
	struct Info *previous, *next;
};



#endif
