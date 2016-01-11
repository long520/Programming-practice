
#include "list.h"
#include <stdio.h>
#include <string.h>

static struct tagLIST_NODE *
list_last(struct tagLIST *const list)
{

	pstLIST_NODE last = NULL;					

	if(NULL != list->next)
	{
		last = list->next;					

		while(last->next != NULL)
		{
			last = last->next;				
		}
		return last;						
	}
	else
	{
		return 0;						
	}
}

static int
list_insert(struct tagLIST *const list, const char *str)
{
	// TODO
	if(str == NULL)
	{
		return -1;
	}
	
	pstLIST_NODE newnode = calloc(sizeof(stLIST_NODE),1);
	pstLIST_NODE last = NULL;

	last = list_last(list);
	
	if(newnode == NULL)
	{
		return -1;
	}
	
	char *data = strdup(str);
	newnode->str = data;
	
	if(last == NULL)
	{
		list->next = newnode;
	}
	
	else
	{
		last->next = newnode;
	}
	
	return 0;

}

static int
list_remove(struct tagLIST *const list, const char *str)
{
	// TODO
	if(str == NULL)
	{
		return -1;
	}
	
	pstLIST_NODE p = NULL;
	pstLIST_NODE q = NULL;

	if(list->next != NULL)
	{
		p = list->next;
		
		if(strcmp(p->str,str) == 0)
		{
			if(p->next)				
			{
				list->next = p->next;		
			}
			else
			{
				list->next = NULL;			
			}

			free(p);				

			return 0;				
		}
		
		q = p;
		p = p->next;	

		while (p)
		{
			if (0 == strcmp(p->str, str))	
			{
                if (NULL == p->next)	
                {
                    q->next = NULL;
                }
                q->next = p->next;
				free(p);
				return 0;
			}
			else
			{
				q = p;
				p = p->next;
			}
		}
		
	
	}

	return -1;	
}


static ssize_t
list_length(struct tagLIST *const list)
{
	// TODO
	ssize_t num = 0;
	pstLIST_NODE p = list->next;

	if(list->next == NULL)

	{	
		return num;
	}
	
	else
	{
		num = 1;

		while(p->next != NULL)
		{
			p = p->next;
			num++;
		}
		
	}

	return num;
}

static ssize_t
list_lookup(struct tagLIST *const list, int id, char *stack, size_t stacklen)
{
	// TODO
	if(list->next == NULL)
	{
		printf("list is NULL");
		return -1;
	}

	ssize_t leg = list->length(list);
	ssize_t num = 1;
	int i = 0;
	pstLIST_NODE p = list->next;
	
	if(id <= leg)
	{
		while(num < id)
		{
			p = p->next;
			num++;
		}
				
		for(i = 0; i < stacklen; i++)
		{
			stack[i] = p->str[i];
		}

	}

	return -1;

}

static void
list_dump(struct tagLIST *const list)
{
	// TODO
	pstLIST_NODE tmp = list->next;
	
	if(list->next != NULL)
	{
		tmp = list->next;
	

		while(tmp !=NULL)
		{
			printf("%s\t",tmp->str);
			tmp = tmp->next;
		}
	}
	
	else
	{
		printf("list is NULL\n");
	}

	printf("\n");
}

pstLIST
LIST_create()
{
	pstLIST list = calloc(sizeof(stLIST), 1);

	// TODO
	if(list == NULL)
	{
		return NULL;
	}
	
	else
	{
	// 接口
	list->insert = list_insert;
	list->remove = list_remove;
	list->length = list_length;
	list->lookup = list_lookup;
	list->dump = list_dump;
	
	}

	return list;
}

void
LIST_release(pstLIST list)
{
	if(!list)
	{
		return;
	}

	// TODO
	pstLIST_NODE p = list->next;

	while(p != NULL)
	{
		list->next = p->next;
		free(p);
		p = list->next;
	}
	free(list);
	list = NULL;
}
