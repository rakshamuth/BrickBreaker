#include <stdlib.h>
#include "mylib.h"

u16* videoBuffer = (u16*) 0x6000000;

void setPixel(int x, int y, u16 color)
{
    videoBuffer[y * 240 + x] = color;
}

void waitForVblank()
{
	while(SCANLINECOUNTER > 160);
	while(SCANLINECOUNTER < 160);
}

void drawRect(int row, int col, int height, int width, u16 color)
{
	for (int r=row; r<row+height; r++) {
		for (int c=col; c<col+width; c++) {
			setPixel(c,r,color);
		}
	}
}

void delay(int n)
{
	volatile int x;
	for (int i=0; i<n*8000; i++)
	{
		x = x + 1;
	}
}

void drawImage3(int r, int c, int width, int height, const u16* image)
{
	for (int i=r; i<height+r; i++) {
		DMA[3].src = image + (i*width);
		DMA[3].dst = videoBuffer + OFFSET(i,c);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_INCREMENT | width;
		//DMA_SOURCE_INCREMENT is default
	}
}

void drawReverseImage3(int r, int c, int width, int height, const u16* image)
{
	for (int i=r; i<height+r; i++) {
		DMA[3].src = image + (i*width);
		DMA[3].dst = videoBuffer + OFFSET(i,c);
		DMA[3].cnt = DMA_ON | DMA_SOURCE_DECREMENT | width;
	}
}

void boundsCheck(int *var, int lowerBound, int upperBound, int *delta, int size)
{
	if(*var <= lowerBound)
	{
		*var = lowerBound;
		*delta = -*delta;
	}
	if(*var >= upperBound-size+1)
	{
		*var = upperBound-size+1;
		*delta = -*delta;
	}
}


void paddleBoundsCheck(int *r, int *c, int paddleLocation, int paddleWidth, int *delta) {
	if(*r == 148 && (*c<=paddleLocation+paddleWidth-2 && *c>=paddleLocation)) {
		*delta = -*delta;
	}
}

void drawLives(int lives)
{
	//void drawRect(int row, int col, int height, int width, u16 color)
	//L
	drawRect(153,185,5,2,BLACK);
	drawRect(158,185,2,5,BLACK);
	//I
	drawRect(153,192,7,2,BLACK);
	//V
	drawRect(153,195,6,2,BLACK);
	drawRect(159,196,1,2,BLACK);
	drawRect(153,198,6,2,BLACK);
	//E
	drawRect(153,201,7,2,BLACK);
	drawRect(159,201,1,5,BLACK);
	drawRect(156,201,1,5,BLACK);
	drawRect(153,201,1,5,BLACK);
	//S
	drawRect(153,207,1,5,BLACK);
	drawRect(159,207,1,5,BLACK);
	drawRect(156,207,1,3,BLACK);
	drawRect(153,207,4,2,BLACK);
	drawRect(156,210,3,2,BLACK);

	if(lives == 3) {
		drawRect(153,220,5,2,BLACK);
		drawRect(158,217,2,5,BLACK);
		drawRect(155,217,2,5,BLACK);
		drawRect(152,217,2,5,BLACK);
	}
	if(lives==2) {
		drawRect(153,217,1,5,BLACK);
		drawRect(159,217,1,7,BLACK);
		drawRect(156,217,1,5,BLACK);
		drawRect(153,222,4,2,BLACK);
		drawRect(156,217,3,2,BLACK);
	}
	if(lives==1) {
		drawRect(153,217,6,2,BLACK);
	}
}
