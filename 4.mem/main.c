
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
	pstMEM mem = MEM_create(_cache, sizeof(_cache));
	if(mem == NULL)
	{
		printf("failed\n");
		return -1;
	}
	
	char *str = "abc";
	char a[10] = "qwer";
	if(NULL != mem)
	{
		void *ptr = mem->alloc(mem, 1024);
		ptr = a;
		printf("a = [%s]\n",(char *)ptr);
		mem->strdup(mem, str);
		
		mem->free(mem, ptr);
		ptr = NULL;
		
	}
	
	MEM_release(&mem);
	
	
	return 0;
}
