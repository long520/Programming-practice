
/**
 * 对于toBMP方法实现，\n
 * 建议优先实现BFMT_WIN32_V30_ARGB8888，BFMT_WIN32_V30_RGB888,	BFMT_WIN32_V30_RGB565格式，\n
 * 上述格式实现后，可尝试实现BFMT_OS2_V11_ARGB8888格式，\n
 * 再者，可以思考索引方式如何实现。
 */

#include <stdio.h>
#include <string.h>
#include "bitmap.h"

static int
bmp_putPixel(struct tagBITMAP *bm, ssize_t x, ssize_t y, color_t color)
{
	
	color_t *rgb = NULL;
	U32 coordinate = 0;
	
	rgb = (color_t *)(&(bm->pixel[0]));
	
	coordinate = x + (y * (bm->Bmp.bmiHeader.biWidth));
	
	rgb[coordinate] = color;
	
	return 0;
}

static int 
bmp_getPixel(struct tagBITMAP *bm, ssize_t x, ssize_t y, color_t *color)
{
	color_t *rgb = NULL;
	int coordinate = 0;
	
	rgb = (color_t *)(&(bm->pixel[0]));
	
	coordinate = x + (y * (bm->Bmp.bmiHeader.biWidth));
	
	printf("Width:%d Height:%d x:%d y:%d",bm->Bmp.bmiHeader.biWidth,bm->Bmp.bmiHeader.biHeight,x,y);
	
	*color = rgb[coordinate];
	
	return 0;
}

static int
bmp_fillRectangle(struct tagBITMAP *bm, ssize_t x, ssize_t y, size_t width, size_t height, color_t *color)
{
	
	color_t *rgb = NULL;
	rgb = (color_t *)(&(bm->pixel[0]));
	
	int i, j;
	int coordinate;
	for(j = y; j < y + height; j++)
	{
		for(i = x; i < x +width; i++)
		{
			coordinate = i + (j * (bm->Bmp.bmiHeader.biWidth));
			rgb[coordinate] = *color;
		}
	}
	
	return 0;
}

static ssize_t
bmp_toBmp(struct tagBITMAP *bm, enBITMAP_FORMAT fmt, void *stack, size_t stackLen)
{
	FILE *fp = fopen("tobmp.bmp","wb");
	if(fp == NULL)
	{
		printf("打开失败\n");
		return -1;
	}
	
	U16 newbfType = 0x4D42;
	U32 bmppicth = ((bm->Bmp.bmiHeader.biWidth * bm->Bmp.bmiHeader.biBitCount + 31)/32 * 4);
	printf("width:%d bmppicth:%d\n",bm->Bmp.bmiHeader.biWidth,bmppicth);
	
	fseek(fp, 0L, SEEK_SET);
	fwrite(&newbfType, sizeof(U16),1,fp);
	
	fwrite(&(bm->Bmp.bfHeader), sizeof(stBITMAPFILEHEADER),1,fp);
	fwrite(&(bm->Bmp.bmiHeader),sizeof(stBITMAPINFOHEADER),1,fp);

	int nheight;
	nheight = bm->Bmp.bmiHeader.biHeight;
	printf("nheight:%d\n",nheight);
	color_t (*rgb)[bm->Bmp.bmiHeader.biWidth] = NULL;
	rgb = (color_t (*)[bm->Bmp.bmiHeader.biWidth])(&(bm->pixel[0]));
	
	for(nheight = bm->Bmp.bmiHeader.biHeight-1;nheight >= 0; nheight--)
	{
		printf("%d\t",nheight);
		if(nheight%10 == 0)
		{
			printf("\n");
		}
		fwrite(rgb[nheight],bmppicth, 1, fp);
	}
	printf("\n");
	U32 size = bm->Bmp.bfHeader.bfSize;
	/*
	if(stackLen < size)
	{
		printf("Out of memory\n");
		return 0;
	}
	
	int read = fread(stack, 1, size, fp);
	if(read == 0)
	{
		printf("stack read failed\n");
		return -1;
	}
	*/
	fclose(fp);
	
	return 0;
	
}

pstBITMAP
BITMAP_create(size_t width, size_t height, color_t background)
{
	// TODO
	pstBITMAP bmp = (pstBITMAP)calloc(sizeof(stBITMAP),1);
	if(bmp == NULL)
	{
		printf("bmp create failed\n");
		return NULL;
	}
	
	U8 bitCounperpix = 8;
	
	U32 bmppicth = (width * bitCounperpix +31)/32*4;
	printf("bmppicth:%d\n",bmppicth);
	U32 filesize = bmppicth * height;
	printf("filesize:%d\n",filesize);
	
	bmp->Bmp.bfHeader.bfType = 0x4D42;
	bmp->Bmp.bfHeader.bfSize = filesize + sizeof(stBITMAPFILEHEADER) + sizeof(stBITMAPINFOHEADER);
	bmp->Bmp.bfHeader.bfReserved1 = 0;
	bmp->Bmp.bfHeader.bfReserved2 = 0;
	bmp->Bmp.bfHeader.bfOffBits = sizeof(stBITMAPFILEHEADER) + sizeof(stBITMAPINFOHEADER);
	
	bmp->Bmp.bmiHeader.biSize = sizeof(stBITMAPINFOHEADER);
	printf("biSize:%d\n",bmp->Bmp.bmiHeader.biSize);
	bmp->Bmp.bmiHeader.biWidth = width;
	printf("width:%d\n",bmp->Bmp.bmiHeader.biWidth);
	bmp->Bmp.bmiHeader.biHeight = height;
	bmp->Bmp.bmiHeader.biPlanes = 1;
	bmp->Bmp.bmiHeader.biBitCount = bitCounperpix;
	printf("biBitCount:%d\n",bmp->Bmp.bmiHeader.biBitCount);
	bmp->Bmp.bmiHeader.biCompression = 0;
	bmp->Bmp.bmiHeader.biSizeImage = filesize;
	bmp->Bmp.bmiHeader.biXPelsPerMeter = 0;
	bmp->Bmp.bmiHeader.biYPelsPerMeter = 0;
	bmp->Bmp.bmiHeader.biClrUsed = 0;
	bmp->Bmp.bmiHeader.biClrImportant = 0;
	
	bmp_fillRectangle(bmp, 0, 0, width, height,&background);
	printf("aaaaa\n");
	return bmp;
}

static void 
showBmpFILEHead(stBITMAPFILEHEADER* pBmpHead)
{
	printf("FILE\n");
	printf("bfType:%#X\n",pBmpHead->bfType);
	printf("bfSize:%d\n",pBmpHead->bfSize);
	printf("bfReserved1:%d\n",pBmpHead->bfReserved1);
	printf("bfReserved2:%d\n",pBmpHead->bfReserved2);
	printf("bfOffBits:%d\n",pBmpHead->bfOffBits);
}

static void 
showBmpInforHead(stBITMAPINFOHEADER* pBmpInforHead)
{
	printf("INFO:\n");
	printf("biSize:%d\n",pBmpInforHead->biSize);
	printf("biwidth:%d\n",pBmpInforHead->biWidth);
	printf("biheight:%d\n",pBmpInforHead->biHeight);
	printf("biPlanes:%d\n",pBmpInforHead->biPlanes);
	printf("biBitCount:%d\n",pBmpInforHead->biBitCount);
	printf("biCompression:%d\n",pBmpInforHead->biCompression);
	printf("biSizeImage:%d\n",pBmpInforHead->biSizeImage);
	printf("biXPelsPerMeter:%d\n",pBmpInforHead->biXPelsPerMeter);
	printf("biYPelsPerMeter:%d\n",pBmpInforHead->biYPelsPerMeter);
	printf("biClrUsed:%d\n",pBmpInforHead->biClrUsed);
	printf("biClrImportant:%d\n",pBmpInforHead->biClrImportant);
}

pstBITMAP
BITMAP_load(const char *bmpFilePath)
{
	// TODO
	printf("load\n");
	FILE *fp = fopen(bmpFilePath, "rb");
	if(fp == NULL)
	{
		printf("load open failed\n");
		return NULL;
	}
	
	pstBITMAP bmp = calloc(sizeof(stBITMAP),1);
	if(bmp == NULL)
	{
		printf("load bmp failed\n");
		return NULL;
	}
	/*
	ssize_t read;
	U16 bfType;

	read = fread(&bfType, 1, sizeof(U16), fp);
	if(read == 0)
	{
		printf("read bfType failed\n");
		return NULL;
	}
	
	if(bfType != 0x4D42)
	{
		printf("file is not .bmp file!");
		fclose(fp);
		free(bmp);
		return NULL;
	}
	printf("bfType:%#X\n",bfType);
	*/
	char *BmpFileHeader;
	BmpFileHeader=(char *)calloc(14,sizeof(char));
	U16 *temp_WORD;
	U32 *temp_DWORD;
	fread(BmpFileHeader,1,14,fp);
	temp_WORD=(U16* )(BmpFileHeader);
	bmp->Bmp.bfHeader.bfType=*temp_WORD;
	printf("aaaaa\n");
	
	if(bmp->Bmp.bfHeader.bfType != 0x4D42)
	{
		printf("file is not .bmp file!");
		return NULL;
	}
	
	temp_DWORD=(U32 *)(BmpFileHeader+sizeof(bmp->Bmp.bfHeader.bfType));
	bmp->Bmp.bfHeader.bfSize=*temp_DWORD;
	temp_WORD=(U16*)(BmpFileHeader+sizeof(bmp->Bmp.bfHeader.bfType)+sizeof(bmp->Bmp.bfHeader.bfSize));
	bmp->Bmp.bfHeader.bfReserved1=*temp_WORD;
	temp_WORD=(U16*)(BmpFileHeader+sizeof(bmp->Bmp.bfHeader.bfType)+sizeof(bmp->Bmp.bfHeader.bfSize)+sizeof(bmp->Bmp.bfHeader.bfReserved1));
	bmp->Bmp.bfHeader.bfReserved2=*temp_WORD;
	temp_DWORD=(U32*)(BmpFileHeader+sizeof(bmp->Bmp.bfHeader.bfType)+sizeof(bmp->Bmp.bfHeader.bfSize)+sizeof(bmp->Bmp.bfHeader.bfReserved1)+sizeof(bmp->Bmp.bfHeader.bfReserved2));
	bmp->Bmp.bfHeader.bfOffBits=*temp_DWORD;
	//fread(&(bmp->Bmp.bfHeader),sizeof(stBITMAPFILEHEADER), 1,fp);
	showBmpFILEHead(&(bmp->Bmp.bfHeader));
	
	fread(&(bmp->Bmp.bmiHeader),sizeof(stBITMAPINFOHEADER), 1,fp);
	showBmpInforHead(&(bmp->Bmp.bmiHeader));
	
	fseek(fp, 0L, SEEK_END);
	U32 size = ftell(fp);
	
	U8 *buf = (U8 *)calloc(1, sizeof(size));
	
	fseek(fp, 0L, SEEK_SET);
	fread(buf, size, 1, fp);
	
	color_t (*rgb)[bmp->Bmp.bmiHeader.biWidth] = NULL;
	
	U32 width = bmp->Bmp.bmiHeader.biWidth;
	U32 height = bmp->Bmp.bmiHeader.biHeight;
	
	rgb = calloc(1,bmp->Bmp.bmiHeader.biSizeImage);
	
	U32 bmppicth = ((width * bmp->Bmp.bmiHeader.biBitCount)+31)/32*4;
	
	printf("load bmppicth:%d\n",bmppicth);
	
	int i,j;
	for(i=height,j=0;i>0;i--,j++)
	{
		memcpy(rgb[i],&(buf[bmp->Bmp.bfHeader.bfOffBits + (bmppicth * j)]),bmppicth);
	}
	
	printf("i:%d\t j:%d\t bfOffBits:%d\n",i,j,bmp->Bmp.bfHeader.bfOffBits);
	
	bmp->pixel = (color_t *)rgb;
	
	bmp->putPixel = bmp_putPixel;
	bmp->getPixel = bmp_getPixel;
	bmp->fillRectangle = bmp_fillRectangle;
	bmp->toBMP = bmp_toBmp;
	
	
	fclose(fp);
	
	return bmp;
	
}

void
BITMAP_release(pstBITMAP *bm)
{
	// TODO
	if(*bm != NULL)
	{
		free(*bm);
		*bm = NULL;
	}
}



