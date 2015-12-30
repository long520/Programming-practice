
/**
 * 基础数据库连接。
 * 通过数据库实现一个容器，实现向其每插入字符串的索引，以及字符串的内容，
 * 当重复插入的时候覆盖上一次插入相同索引的数据，
 * 模块操作（插入、移除、检索）必须线程安全（可以同时多线程进行读写）。
 * 通过字符串索引可以获取到对应的内容。
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include "sqlite3.h"

#ifndef PRACTICE_DICT2_H_
#define PRACTICE_DICT2_H_
#if defined(__cplusplus) & __cplusplus
extern "C" {
#endif



/**
 * 容器数据结构
 */
typedef struct tagDICT2
{
  /**
   * 向容器插入一条数据。
   *
   * @return 成功插入返回0，重复插入返回0，失败返回-1。
   */
  int
  (*insert)(struct tagDICT2 *dict, const char *id, const char *content);

  /**
   * 从容器中移除一条指定的数据。
   *
   * @return 移除成功返回0，否则返回-1。
   */
  int
  (*remove)(struct tagDICT2 *dict, const char *id);

  /**
   * 返回容器索引的条目数。
   */
  ssize_t
  (*length)(struct tagDICT2 *dict);

  /**
   * 按字符串查找到该索引上的数据。
   *
   * @return 若对应索引上有字符串则把字符串拷贝到缓冲长度为stacklen的stack的缓冲中并返回字符串的长度，\n
   *          否则返回-1。
   */
  ssize_t
  (*lookup)(struct tagDICT2 *dict, const char *id, char *stack, size_t stacklen);

  /**
   * 按顺序打印容器中的索引以及数据。
   *
   * @return 无。
   */
  void
  (*dump)(struct tagDICT2 *dict);
  
  /**
   * 清空容器内的所有索引数据。
   *
   * @return 无。
   */
   void
   (*empty)(struct tagDICT2 *dict);

   const char *path;
   sqlite3 *db;

}stDICT2,*pstDICT2;


/**
 * 创建容器。
 *
 * @param[in] dbPath 数据库文件所在路径。
 * 
 * @retval 容器句柄 创建成功。
 * @retval NULL 创建失败。
 */
extern pstDICT2
DICT2_create(const char *dbPath);

/**
 * 释放容器。
 * 
 * @param[in] dict 容器句柄。
 *
 * @return 无。
 */
extern void
DICT2_release(pstDICT2 dict);



#if defined(__cplusplus) & __cplusplus
}
#endif
#endif


