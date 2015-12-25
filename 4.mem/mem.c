
/**
 * 
 */

#include "mem.h"

static void *
mem_alloc(struct tagMEM *mem, size_t sz)
{
	if(mem == NULL)
	{
		return;
	}
	
	pstMEM p = mem;
	Memspace *q = NULL;
	unsigned int count;
	
	if(p->freeHead == NULL)
	{
		printf("no space\n");
	}
	
	else
	{	
		q = p->freeHead;
		p->freeHead = q->pNext;
		if((sz%(p->spaceSize)) != 0)
		{
			count = (sz/(p->spaceSize)) + 1;
			p->freeCount -= count;
		}
		
		else
		{
			count = sz/(p->spaceSize);
			p->freeCount -= count;
		}
		
		q->pNext = NULL;
		q->pPrev = p->usedTail;
		
		if(p->usedHead == NULL)
		{
			p->usedHead = p->usedTail = q;
		}
		
		else
		{
			p->usedTail->pNext = q;
			p->usedTail = q;
		}
		
	}
	
	return q;
		
}

static int
mem_free(struct tagMEM *mem, void *ptr)
{
	pstMEM p = mem;  
	Memspace *q = (Memspace *)ptr;  
	
	if(q == NULL)
	{
		return -1;
	}
	
	if(q->pPrev == NULL)
	{
		p->usedHead = q->pNext;
		if(p->usedHead != NULL)
		{
			p->usedHead->pPrev =NULL;
		}
	}
	
	if(q->pNext == NULL)
	{
		p->usedTail = q->pPrev;
		if(p->usedTail != NULL)
		{
			p->usedTail->pNext = NULL;
			
		}
	}
	
	else
	{
		q->pPrev->pNext = q->pNext;
		q->pNext->pPrev = q->pPrev;
	}
	
	q->pPrev = p->freeTail;
	q->pNext = NULL;
	
	p->freeTail->pNext = q;
	p->freeTail = q;
	p->freeCount++;
	
	return 0;
	
}

static int
mem_strdup(struct tagMEM *mem, char *str)
{
	pstMEM p = calloc(sizeof(stMEM),1);
	p = mem;
	if(p->freeHead == NULL)
	{
		printf("free no space\n");
		return -1;
	}
	else
	{
		Memspace *q = mem_alloc(p, sizeof(str));
		q = p->freeHead;
		p->freeHead = q->pNext;
		unsigned int count;
		count = ((sizeof(str))/(p->spaceSize)) + 1;
		p->freeCount -= count;
		q->pNext = p->freeHead;
		q->pPrev = p->usedTail;
		q->data = str;
		printf("%s\n",q->data);
		
		if(p->usedHead == NULL)
		{
			p->usedHead = p->usedTail = q;
		}
		
		else
		{
			p->usedTail->pNext = q;
			p->usedTail = q;
		}
		
	}
	
	return 0;
	
}


pstMEM
MEM_create(void *buf, size_t sz)
{
	pstMEM p = calloc(sizeof(stMEM),1);
	
	p = buf;
	p->spaceSize = 1024;
	if((sz%p->spaceSize) != 0)
	{
		p->spaceCount = (sz/p->spaceSize) + 1;
	}
	
	else
	{
		p->spaceCount = sz/(p->spaceSize);
	}
	
	p->freeCount = p->spaceCount;
	p->usedHead = p->usedTail = NULL;
	p->freeHead = p->freeTail = NULL;
	
	Memspace *q = NULL;
	p->first = (char *)malloc(sizeof(Memspace) * (p->spaceCount));
	int i;
	for(i = 0; i < (p->spaceCount); i++)
	{
		q = (Memspace *)(p->first + sizeof(Memspace) * i);
		q->pNext = NULL;
		q->pPrev = p->freeTail;
		
		if(p->freeHead == NULL)
		{
			p->freeHead = p->freeTail = q;
		}
		
		else
		{
			p->freeTail->pNext = q;
			p->freeTail = q;
		}
	}
	
	p->alloc = mem_alloc;
	p->free = mem_free;
	p->strdup = mem_strdup;
	
	return p;
}

tVOID
MEM_release(pstMEM *mem)
{
	
	pstMEM p = (pstMEM)calloc(sizeof(stMEM),1);
	p = mem;
	free(p->first);

	free(p);
}



