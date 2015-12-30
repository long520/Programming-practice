

#include "dict2.h"



int 
main(int argc, char *argv[])
{
	char *dbPath = "dictsqlite.db";
	char str[100];
	pstDICT2 dict = DICT2_create(dbPath);
	printf("creat successful\n");
	
	dict->insert(dict,"a","abc");
	dict->insert(dict,"b","bar");
	dict->insert(dict,"c","cabs");
	dict->insert(dict,"d","delay");
	dict->insert(dict,"e","ecvt");
	
	
	dict->dump(dict);
	
	dict->length(dict);
	
	char buf[100] = "";
	printf("\n");
	
	printf("input ID:");
	scanf("%s",str);
	
	dict->lookup(dict, str, buf, sizeof(buf));
	
	printf("remove ID:");
	scanf("%s",str);
	
	dict->remove(dict, str);
	dict->dump(dict);
	
	dict->empty(dict);
	printf("清空数据库\n");
	dict->dump(dict);
	printf("数据库无内容\n");
	
	DICT2_release(dict);
	
	return 0;
}
