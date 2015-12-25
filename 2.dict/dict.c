
#include "dict.h"
#include <stdio.h>
#include <string.h>

pstDICT_NODE new_create()
{
	pstDICT_NODE new = (pstDICT_NODE)calloc(sizeof(stDICT_NODE),1);
	new->str = NULL;
	memset(new->reference, 0, sizeof(new->reference));

	return new;
}

static int
dict_insert(struct tagDICT *dict, char *id, char *content)
{
	char *p = id;
	char code;
	
 	pstDICT_NODE new = dict->root;

	while (NULL != new && *p)
	{
		if(*p >='a' && *p <= 'z')
		{
			code = *p - 'a';
		}

		else if(*p >= 'A' && *p <= 'Z')
		{
			code = *p - 'A';
		}

		if(new->reference[code] == NULL)
		{
			new->reference[code] = new_create();
		}

		new = new->reference[code];
		p++;
	}

	if(new == NULL)
	{
		return 0;
	}

	else 
	{
		new->str = strdup(content);
	}
	
	return 0;
}

static int
dict_remove(struct tagDICT *dict, char *id)
{
	char *p =id;
	char code;

	pstDICT_NODE new = dict->root;

	while(NULL != new && *p)
	{
		
		if(*p >='a' && *p <= 'z')
		{
			code = *p - 'a';
		}

		else if(*p >= 'A' && *p <= 'Z')
		{
			code = *p - 'A';
		}
			
		new = new->reference[code];
		p++;
	
	}

	if(NULL != new && new->str)
	{
		new->str = "\t";
		return 0;
	}
	
	else
	{
		return -1;
	}
}

static ssize_t
dict_lookup(struct tagDICT *dict, char *id, char *stack, size_t stacklen)
{
	char *p = id;
	char code;
	int n = 0;

	pstDICT_NODE new = dict->root;

	while (NULL != new && *p)
	{
			
		if(*p >='a' && *p <= 'z')
		{
			code = *p - 'a';
		}

		else if(*p >= 'A' && *p <= 'Z')
		{
			code = *p - 'A';
		}
		
		new = new->reference[code];
		n++;
		p++;
	}

	if(NULL != new && new->str)
	{
		strcpy(stack, new->str);
		return (n);
	}

	else
	{
		return -1;
	}
}

int 
trie_dump(pstDICT_NODE node)
{
	int i;

	if (NULL == node)
	{
		return -1;
	}

	for(i = 0; i < 26; i++)
	{
		if(NULL != node->reference[i])
		{
			printf("%c\t", ('a' + i));
			trie_dump(node->reference[i]);
			
		}	
	}
	if (node->str)
	{
		printf("%s\n", node->str);
	}
	
	return 0;
}

void 
dict_dump(struct tagDICT *dict)
{
	pstDICT_NODE new = dict->root;

	if(dict == NULL)
	{
		return;
	}

	trie_dump(new);
	
}

pstDICT
DICT_create()
{
	pstDICT dict = (pstDICT)calloc(sizeof(stDICT), 1);

	// TODO
	if(dict == NULL)
	{
		return NULL;
	}
	else
	{
		dict->insert = dict_insert;
		dict->remove = dict_remove;
		dict->lookup = dict_lookup;
		dict->dump = dict_dump;
		
		dict->root = (pstDICT_NODE)calloc(sizeof(stDICT_NODE),1);
		
		return dict;
	}
}

int 
trie_delete(pstDICT_NODE node)
{
	int i;

	if (node == NULL)
	{
		return -1;
	}

	for(i = 0; i < 26; i++)
	{
		if(node->reference[i] != NULL)
		{
			trie_delete(node->reference[i]);
		}	
	}
	free (node);
	node = NULL;
}

void
DICT_release(pstDICT dict)
{
	if(!dict)
	{
		return;
	}

	// TODO
	pstDICT_NODE p = dict->root;
	
	if(trie_delete(p))
	{
		free(dict);
		dict = NULL;
	}
	
}
