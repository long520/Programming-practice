
#include "mem.h"
#include <malloc.h>
#include <stdio.h>
#include <string.h>

/**
 * 静态内存用于内存分配器调度。
 */
static unsigned
_cache[1024 * 1024];

int
main(int argc, char *argv[])
{
	printf("create\n");
	pstMEM mem = MEM_create(_cache, sizeof(_cache));
	if(mem == NULL)
	{
		printf("failed\n");
		return -1;
	}
	
	char *str = "abc";
	if(NULL != mem)
	{
		void *ptr = (void *)mem->alloc(mem, 1024);
		
		printf("strdup\n");
		mem->strdup(mem, (char *)str);
		
		printf("free\n");
		mem->free(mem, ptr);
		
	}
	
	MEM_release(&mem);
	
	
	return 0;
}
