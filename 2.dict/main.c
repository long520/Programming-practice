
#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int
main(int argc, char *argv[])
{
	pstDICT dict = DICT_create();
	char str[100];

	if(dict != NULL)
	{
		dict->insert(dict,"a","add");
		dict->insert(dict,"b","bad");
		dict->insert(dict,"x","xxx");
		dict->insert(dict,"r","red");
		dict->insert(dict,"y","yes");

		dict->dump(dict);

		while(1)
		{
			char txt[100] = "";
			printf("add string index:");
			scanf("%s",str);

			if((dict->lookup(dict,str,txt,sizeof(txt))) >= 0)
			{
				printf("%s\n",txt);
			}

			else
			{
				printf("did not find\n");
			}

			printf("delete string index:");
			scanf("%s",str);

			if(dict->remove(dict,str) != 0)
			{
				printf("delete false\n");
			}

			else
			{
				dict_dump(dict);
			}

			memset(txt, 0, sizeof(txt));

		}

		DICT_release(dict);
	}
	return 0;
}
