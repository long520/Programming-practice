
/**
 * 基础链表练习二
 * 实现一个字符串链表，每插入一个字符串所为索引，以及其对应内容，链表会对其索引进行按字典进行排序（不区分大小写，如a在aa前，aa在b前等），
 * 重复插入的字符串索引会更新上一次的对应索引内容，
 * 通过字符串索引可以获取到对应的内容。
 *
 * 思考：
 *  1、在庞大数据量的情况下，如何设计数据结构去使得插入和查找的速度提高。
 */


#include <stdio.h>
#include <stdlib.h>

#ifndef PRACTICE_DICT_H_
#define PRACTICE_DICT_H_
#if defined(__cplusplus) & __cplusplus
extern "C" {
#endif

#define ssize_t int

typedef struct tagDICT_NODE
{
	char *str;

	struct tagDICT_NODE *reference[26];
	
}stDICT_NODE,*pstDICT_NODE;

/**
 * 链表数据结构
 */
typedef struct tagDICT
{
  /**
   * 向链表插入一条数据。
   *
   * @return 成功插入返回0，重复插入返回0，失败返回-1。
   */
  int
  (*insert)(struct tagDICT *const dict, const char *id, const char *content);

  /**
   * 从链表中移除一条指定的数据。
   *
   * @return 移除成功返回0，否则返回-1。
   */
  int
  (*remove)(struct tagDICT *const dict, const char *id);

  /**
   * 返回链表的长度。
   */
  ssize_t
  (*length)(struct tagDICT *const dict);

  /**
   * 按字符串查找到该索引上的数据。
   *
   * @return 若对应索引上有字符串则把字符串拷贝到缓冲长度为stacklen的stack的缓冲中并返回字符串的长度，\n
   *          否则返回-1。void
   */
  ssize_t
  (*lookup)(struct tagDICT *const dict, const char *id, char *stack, size_t stacklen);

  /**
   * 按顺序打印链表中的索引以及数据。
   *
   * @return 无。
   */
  void
  (*dump)(struct tagDICT *const dict);
  
  struct tagDICT_NODE *root;

}stDICT,*pstDICT;

/**
 * 创建链表。
 * 
 * @retval 链表句柄 创建成功。
 * @retval NULL 创建失败。
 */
extern pstDICT
DICT_create();

/**
 * 释放链表。
 * 
 * @param[in] dict 链表句柄。
 *
 * @return 无。
 */
extern void
DICT_release(pstDICT dict);




#if defined(__cplusplus) & __cplusplus
}
#endif
#endif


