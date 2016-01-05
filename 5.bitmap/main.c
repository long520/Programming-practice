
#include "bitmap.h"

int
main(int argc, char *argv[])
{
	/*
	color_t color = 0xFF000000;
	pstBITMAP bm = BITMAP_create(20,20,color);
	if(bm == NULL)
	{
		printf("create failed\n");
		return -1;
	}
	*/
	void *stack = NULL;
	char *bmpFilePath = "rc/2.bmp";
	
	pstBITMAP bmp = BITMAP_load (bmpFilePath);
	if(bmp == NULL)
	{
		printf("load failed\n");
		return 0;
	}
	
	bmp->putPixel(bmp,5,5,0xFFFFFFFF);
	
	color_t color1 = 0xFF00FF00;
	
	bmp->fillRectangle(bmp,5,10,5,5,&color1);
	
	color_t color2;
	bmp->getPixel(bmp,15,15,&color2);
	
	int size = bmp->toBMP(bmp,BFMT_WIN32_V30_ARGB8888,stack,sizeof(stack));
	printf("size:%d\n",size);
	
	BITMAP_release(&bmp);
	
	return 0;
}
