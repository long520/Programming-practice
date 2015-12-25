/**
 * 基础内存分配器练习。
 * 设计一个内存分配模块，有与系统malloc，free，strdup相类似的接口。\n
 * 内存分配器调度内存固定在传入的一块内存空间内，\n
 * 这样一个内存分配器可以广泛地应用到项目中，避免频繁使用系统的malloc，free接口，\n
 * 使系统进行频繁的页切换，从而有效提高运行效率。
 *
 */

#include <stdio.h>
#include <stdlib.h>

#ifndef PRACTICE_MEM_H_
#define PRACTICE_MEM_H_
#if defined(__cplusplus) && __cplusplus
extern "C" {
#endif

typedef void tVOID;

typedef struct memspace  
{  
struct memspace *pNext;   
struct memspace *pPrev;  
char* data;  
} Memspace;


typedef struct tagMEM
{
	/**
	 * man malloc
	 * 
	 */
	void *
	(*alloc)(struct tagMEM *mem, size_t sz);

	/**
	 * man free
	 * 
	 */
	int
	(*free)(struct tagMEM *mem, void *ptr);
	
	/**
	 * man strdup
	 * 
	 */
	int
	(*strdup)(struct tagMEM *mem, char *str);

	 
	unsigned int spaceCount;
	unsigned int spaceSize;
	unsigned int freeCount;  
	Memspace *freeHead;  
	Memspace *freeTail; 
	Memspace *usedHead;   
	Memspace *usedTail;
	char *first;

}stMEM, *pstMEM;


/**
 * 创建内存分配器。
 *
 * \param[in] sz 预分配调度内存大小。
 *
 */
extern pstMEM
MEM_create(void *buf, size_t sz);

/**
 * 释放内存分配器。
 *
 */
extern tVOID
MEM_release(pstMEM *mem);


#if defined(__cplusplus) && __cplusplus
};
#endif
#endif // PRACTICE_MEM_H_
