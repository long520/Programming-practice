/**
 * 基础文件解析练习。
 * 对位图数据进行操作，可以通过读入BMP文件以及创建句柄的方式创建位图模块，\n
 * 通过模块方法对位图进行编辑，\n
 * 然后保存成新的文件，\n
 * 各种操作方法必须线程安全。
 */



#ifndef PRACTICE_BITMAP_H_
#define PRACTICE_BITMAP_H_
#if defined(__cplusplus) && __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "bitmap.h"

/**
 * 颜色类型。
 * 按照ARGB8888格式标准，共32位，\n
 * 其中[31:24]表示Alpha即透明分量，\n
 * [23:16]表示Red即红色分量，\n
 * [15:8]表示Green即绿色分量，\n
 * [7:0]表示Blue即蓝色分量，\n
 */
typedef unsigned long  color_t;
typedef unsigned char  U8;
typedef unsigned short U16;
typedef unsigned int   U32;

typedef struct tagBITMAPFILEHEADER 
{
	
	U16 bfType;    //位图类型 bm 
	U32 bfSize;    //位图文件大小
	U16 bfReserved1;  // 0
	U16 bfReserved2;  // 0
	U32 bfOffBits;   //位图数据起始位置
}stBITMAPFILEHEADER, *pstBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER 
{
	U32 biSize;  //结构所占字节
	U32 biWidth;  //位图宽度    每行像素数
	U32 biHeight;  //位图高度   行数
	U16 biPlanes;  //1
	U16 biBitCount;   //每个像素的位数
	U32 biCompression;  // 0
	U32 biSizeImage;    //位图大小
	U32 biXPelsPerMeter;  //水平分辨率
	U32 biYPelsPerMeter;  // 垂直分辨率
	U32 biClrUsed;  //位图使用颜色数
	U32 biClrImportant;  //显示过程重要颜色
}stBITMAPINFOHEADER, *pstBITMAPINFOHEADER;


typedef struct color
{
	U8 Blue;
	U8 Green;
	U8 Red;
	U8 Alpha;
}stcolor, *pstcolor;

typedef struct tagBITMAPINFO 
{ 
    stBITMAPFILEHEADER bfHeader;
	stBITMAPINFOHEADER bmiHeader;
}stBITMAPINFO, *pstBITMAPINFO;

typedef enum tagBITMAP_FORMAT
{
	BFMT_WIN32_V30_ARGB8888,
	BFMT_WIN32_V30_RGB888,
	BFMT_WIN32_V30_RGB565,
	
	BFMT_OS2_V11_ARGB8888,

}enBITMAP_FORMAT;

typedef struct tagBITMAP
{
	/**
	 * 向位图上某像素着色。
	 * 
	 * \param[in] bm 模块句柄。
	 * \param[in] x 像素所在水平坐标。
	 * \param[in] y 像素所在垂直坐标。
	 * \param[in] color 颜色值。
	 * 
	 * \retval 0 成功。
	 * \retval -1 失败，通常是参数错误。
	 * 
	 */
	int
	(*putPixel)(struct tagBITMAP *bm, ssize_t x, ssize_t y, color_t color);

	/**
	 * 从位图上某像素获取颜色。
	 * 
	 * \param[in] bm 模块句柄。
	 * \param[in] x 。
	 * \param[in] y 像素所在垂直坐标。
	 * \param[out] color 颜色值。
	 * 
	 * \retval 0 成功。
	 * \retval -1 失败，通常是参数错误。
	 * 
	 */
	int
	(*getPixel)(struct tagBITMAP *bm, ssize_t x, ssize_t y, color_t *color);
	
	/**
	 * 从位图上某区域填充颜色。
	 * 
	 * \param[in] bm 模块句柄。
	 * \param[in] x 区域左上角位置所在水平坐标。
	 * \param[in] y 区域左上角位置所在垂直坐标。
	 * \param[in] width 区域的宽度。
	 * \param[in] height 区域的高度。
	 * \param[out] color 颜色值。
	 * 
	 * \retval 0 成功。
	 * \retval -1 失败，通常是参数错误。
	 * 
	 */
	int
	(*fillRectangle)(struct tagBITMAP *bm, ssize_t x, ssize_t y, size_t width, size_t height, color_t *color);
	
	/**
	 * 生成BMP文件。
	 * 生成的文件数据存到stack的内存缓冲空间中，\n
	 * 在传入stack时候需要考虑到文件可能的大小，\n
	 * 并且分配stack足够大的空间。
	 * 
	 * \param[in] bm 模块句柄。
	 * \param[in] fmt 文件格式，详见enBITMAP_FORMAT。
	 * \param[out] stack 文件数据缓冲。
	 * \param[in] stackLen 文件数据缓冲的大小。
	 * \param[in] height 区域的高度。
	 * \param[out] color 颜色值。
	 * 
	 * \retval 文件数据的大小 成功。
	 * \retval 0 失败，缓冲空间不足。
	 * \retval -1 失败，通常是参数错误。
	 * 
	 */
	ssize_t
	(*toBMP)(struct tagBITMAP *bm, enBITMAP_FORMAT fmt, void *stack, size_t stackLen);
	
	stBITMAPINFO Bmp;
	color_t *pixel;

}stBITMAP, *pstBITMAP;


/**
 * 创建位图模块句柄。
 */
extern pstBITMAP
BITMAP_create(size_t width, size_t height, color_t background);

/**
 * 从BMP文件中读入数据并创建位图模块句柄。
 */
extern pstBITMAP
BITMAP_load(const char *bmpFilePath);

/**
 * 释放位图模块句柄。
 */
extern void
BITMAP_release(pstBITMAP *bm);


#if defined(__cplusplus) && __cplusplus
};
#endif
#endif // PRACTICE_BITMAP_H_
