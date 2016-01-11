
/**
 * 
 */

#include "mem.h"

static void *
mem_alloc(struct tagMEM *mem, size_t sz)
{
	if(mem == NULL)
	{
		return NULL;
	}
	
	if(sz <= 0)
	{
		return NULL;
	}
	
	pstmemspace p = mem->first;
	
	if(p == NULL)
	{
		printf("failed\n");
		return NULL;
	}
	
	if((p->used == 0) && ((p->block_size) - sz - sizeof(stmemspace) > 0))
	{
		pstmemspace q = (pstmemspace)((char *)p->buf + sz);
		q->buf = (char *)q + sizeof(stmemspace);
		q->block_size = p->block_size - sz - sizeof(stmemspace);
		q->next = p->next;
		
		if(p->next != NULL)
		{
			p->next->prev = q;
		}
		
		p->next = q;
		q->prev = p;
		q->used = 0;
		p->block_size = sz;
		
		q->used = 1;
		return (void *)(p->buf);
	}
	
	else
	{
		p = p->next;
	}
}

static int
mem_free(struct tagMEM *mem, void *ptr)
{
	if(mem == NULL)
	{
		return -1;
	}
	
	if(ptr == NULL)
	{
		return -1;
	}
	
	pstmemspace p = (pstmemspace)((unsigned char *)ptr - sizeof(stmemspace));
	
	int num = (unsigned char *)p - (unsigned char *)(mem->first);
	
	if(num < 0 || num > mem->size)
	{
		return -1;
	}
	
	if(p->used != 1)
	{
		printf("memory not used\n");
		return -1;
	}
	
	p->used = 0;
	
	if((p->next != NULL) && (p->next->used == 0))
	{
		p->block_size += p->next->block_size + sizeof(stmemspace);
		pstmemspace q = p->next->next;
		p->next = q;
		
		if(q != NULL)
		{
			q->prev = p;
		}
	}
	
	if((p->prev != NULL) && (p->prev->used == 0))
	{
		p->prev->block_size += p->block_size + sizeof(stmemspace);
		p->prev->next = p->prev;
		
		if(p->next != NULL)
		{
			p->next->prev = p->prev;
		}
		
		return 0;
	}
	
	return -1;
}

static int
mem_strdup(struct tagMEM *mem, char *str)
{
	int length;
	char *buf = NULL;
	
	for(length = 0; (length < 255) && (str[length] != '\0'); length++);
	
	buf = (char *)mem_alloc(mem,length+sizeof(stmemspace)+1);
		
	if(buf != NULL)
	{
		for(buf[length--] = 0; length >= 0; length--)
		{
			buf[length] = str[length];
		}
		printf("%s\n",buf);
	}
	
	return 0;
}


pstMEM
MEM_create(void *buf, size_t sz)
{
	pstmemspace p = (pstmemspace)((unsigned char *)buf + sizeof(stMEM));
	
	p->block_size = sz - sizeof(stMEM);
	p->buf = (char *)p + sizeof(stmemspace);
	p->prev = p->next = NULL;
	p->used = 0;
	
	((pstMEM)buf)->alloc = mem_alloc;
	((pstMEM)buf)->free = mem_free;
	((pstMEM)buf)->strdup = mem_strdup;
	
	((pstMEM)buf)->size = sz;
	
	((pstMEM)buf)->first = p;
	
	return ((pstMEM)buf);
	
}

void
MEM_release(pstMEM *mem)
{
	int i = 0;
	
	unsigned char *p = (unsigned char *)(*mem);
	
	for(i=0;i < (*mem)->size; i++)
	{
		p[i] = 0;
	}
	
	*mem = NULL;
}



