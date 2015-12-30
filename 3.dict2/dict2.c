
/**
 * 使用SQLITE辅助实现。
 * http://www.sqlite.com/
 */

#include <stdio.h>
#include <string.h>
#include "dict2.h"
#include "sqlite3.h"


static int 
dict_insert(struct tagDICT2 *dict, const char *id, const char *content)
{
	sqlite3 *p = dict->db;
	char *zErrMsg = 0;
	int rc;
	
	rc = sqlite3_open(dict->path,&(dict->db));
	
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
		return -1;
	}
	
	char *sql = sqlite3_mprintf("INSERT INTO SensorData VALUES('%s', '%s')",id,content);
	rc = sqlite3_exec(p, sql, 0, 0, &zErrMsg);
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(p));
		sqlite3_close(p);
	}
	
	sqlite3_close(p);
	
	return 0;
}

static int
dict_remove(struct tagDICT2 *dict, const char *id)
{
	sqlite3 *p = dict->db;
	char *zErrMsg = 0;
	int rc;
	
	rc = sqlite3_open(dict->path,&(p));
	
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(p));
		sqlite3_close(p);
		return -1;
	}
	
	char *sql = sqlite3_mprintf("DELETE FROM SensorData WHERE ID = '%s'",id);
	sqlite3_exec(p, sql, 0, 0, &zErrMsg);
	
	return 0;
}

static ssize_t
dict_length(struct tagDICT2 *dict)
{
	dict->db = NULL;
	char *zErrMsg = 0;
	char **Result;
	int nrow = 0;
	int ncolumn = 0;
	int rc;
	
	rc = sqlite3_open(dict->path,&(dict->db));
	
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
		return -1;
	}
	
	char *sql = "SELECT COUNT(*) FROM SensorData;";
	sqlite3_get_table(dict->db , sql , &Result , &nrow , &ncolumn , &zErrMsg );
	int i;
	
	for( i=1 ; i<( nrow + 1 ) * ncolumn ; i++ )
	{
		printf( "条目数：%s\t", Result[i] );
	}
	
	sqlite3_free_table( Result );
	
	return 0;
	
}

static ssize_t
dict_lookup(struct tagDICT2 *dict, const char *id, char *stack, size_t stacklen)
{
	dict->db = NULL;
	char *zErrMsg = 0;
	char **Result;
	int nrow = 0;
	int ncolumn = 0;
	int rc;
	rc = sqlite3_open(dict->path,&(dict->db));
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
		return -1;
	}
	
	char *sql = sqlite3_mprintf("SELECT Content FROM SensorData WHERE ID = '%s'",id);
	sqlite3_get_table( dict->db , sql , &Result , &nrow , &ncolumn , &zErrMsg );
	int i;
	printf( "The result of querying is :" );
	for( i=1 ; i <= nrow * ncolumn ; i++ )
	{
		printf( "%s\t", Result[i]);
		printf("\n");
	}
	
	if(Result[1] == NULL)
	{
		printf("No content\n");
		return 0;
	}
	sqlite3_free_table( Result );
	strncpy(stack,Result[1],stacklen);
	printf("stack:%s\n",stack);
	return 0;
	
}

int my_callback(void * data, int col_count, char ** col_values, char ** col_Name)
{
	int i;
	for( i=0; i < col_count; i++)
	{
		printf( "%s = %s\t", col_Name[i], col_values[i] == 0 ? "NULL" : col_values[i] );
	}
	
	printf("\n");
	
	return 0;
}

static void
dict_dump(struct tagDICT2 *dict)
{
	dict->db = NULL;
	char *zErrMsg = 0;
	int rc;
	
	rc = sqlite3_open(dict->path,&(dict->db));
	
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
	}
	
	char *sql = "select * from SensorData;";
	rc = sqlite3_exec(dict->db, sql, my_callback, 0, &zErrMsg);	
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
	}
	
	
}

static void
dict_empty(struct tagDICT2 *dict)
{
	dict->db = NULL;
	char *zErrMsg = 0;
	int rc;
	rc = sqlite3_open(dict->path,&(dict->db));
	
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
	}
	
	char *sql = "DELETE FROM SensorData;";
	sqlite3_exec(dict->db, sql, 0, 0, &zErrMsg);
	
}



pstDICT2
DICT2_create(const char *dbPath)
{
	pstDICT2 dict = calloc(sizeof(stDICT2), 1);
	
	if(dict == 	NULL)
	{
		printf("failed");
		return NULL;
	}
	// TODO
	
	dict->db = NULL;
	dict->path = dbPath;
	char *zErrMsg = 0;
	
	int  rc = sqlite3_open(dbPath,&(dict->db));
	
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
		return NULL;
	}
	
	char *sql = "CREATE TABLE SensorData(ID verchar(255), Content verchar(255));";
	rc = sqlite3_exec(dict->db, sql, 0, 0, &zErrMsg);
	if(rc != 0)
	{
		fprintf(stderr,"can not open:%s\n",sqlite3_errmsg(dict->db));
		sqlite3_close(dict->db);
		return NULL;
	}
	
	sqlite3_close(dict->db);
	
	dict->insert = dict_insert;
	dict->remove = dict_remove;
	dict->length = dict_length;
	dict->lookup = dict_lookup;
	dict->dump = dict_dump;
	dict->empty = dict_empty;

	return dict;
}

void
DICT2_release(pstDICT2 dict)
{
	if(!dict || !(dict))
	{
		return;
	}

	// TODO
	sqlite3 *p;
	p = dict->db;
	char *zErrMsg = 0;
	
	char *sql = "DROP TABLE SensorData;";
	sqlite3_exec(p, sql, 0, 0, &zErrMsg);
	
	sql = "DROP DATABASE dict;";
	sqlite3_exec(p, sql, 0, 0, &zErrMsg);
	
	free(dict);
	dict = NULL;
}
