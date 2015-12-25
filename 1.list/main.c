
#include "list.h"


int
main(int argc, char *argv[])
{
	pstLIST list = NULL;

	list = LIST_create();

	if(NULL != list)
	{
		char text[512];

		list->insert(list, "abc");
		list->insert(list, "aac");
		list->insert(list, "bac");
		list->insert(list, "cad");
		
		
		
		list->dump(list);

		list->lookup(list, 1, text, sizeof(text));
		printf("text = %s\n", text);

		list->dump(list);

		LIST_release(list);
	}


	return 0;
}
