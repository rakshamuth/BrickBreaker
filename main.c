#include <stdlib.h>
#include "mylib.h"
#include "intro.h"
#include "sidebar.h"
#include "end.h"

extern const u16 intro_screen[38400];

int main(void)
{
	int i;
	REG_DISPCNT = MODE3 | BG2_ENABLE;

	int paddleWidth = 20;
	int paddleLocation = 110; //left side of paddle

	//setting array to determine whether brick is there or not
	BricksMatrix blocks;
	for (int i=0; i<5; i++) {
		for (int j=0; j<5; j++) {
			blocks.matrix[i][j] = 1;
		}
	}

	int ball_r = 148;
	int ball_c = paddleLocation+10;
	int dr = 1;
	int dc = 1;
	int state = 0;	//0-home screen, 1-game screen, 2-end screen
	int lives = 3;

	while(1) {
		if (state == 0) {
			waitForVblank();
			for (i = 0; i < 160; i++)
			{
				DMA[3].src=&intro_screen[240*i];		//where you're getting from
				DMA[3].dst=videoBuffer+OFFSET(i,0);	//where you're putting it
				DMA[3].cnt=DMA_ON | 240;
			}
			if(KEY_DOWN_NOW(BUTTON_START)) {
				state = 1;
				int color = 0;
				for (int r=0; r<160; r++) {
					DMA[3].src=&color;
					DMA[3].dst=videoBuffer+OFFSET(r,0);
					DMA[3].cnt=DMA_ON | DMA_SOURCE_FIXED | 240;
				}
				paddleWidth = 20;
				paddleLocation = 110; //left side of paddle

				//setting array to determine whether brick is there or not
				for (int i=0; i<5; i++) {
					for (int j=0; j<5; j++) {
						blocks.matrix[i][j] = 1;
					}
				}
				ball_r = 148;
				ball_c = paddleLocation+10;
				dr = 1;
				dc = 1;
				lives = 3;
			}
		}
		if (state == 1) {	
			//Draw side panels
			drawImage3(0, 0, 60, 160, sidebar);
			drawReverseImage3(0, 180, 60, 160, sidebar);
			drawLives(lives);
			//Draw bricks
			int brickWidth = 24;
			int brickHeight = 12;
			for (int i=0; i<5; i++) {
				for (int j=0; j<5; j++) {
					if (blocks.matrix[i][j] == 1) {
						drawRect((i*brickHeight), (j*brickWidth)+60, 1, brickWidth, GREY);
						drawRect((i*brickHeight)+brickHeight-1, (j*brickWidth)+60, 1, brickWidth, GREY);
						drawRect((i*brickHeight), (j*brickWidth)+60, brickHeight, 1, GREY);
						drawRect((i*brickHeight), (j*brickWidth)+brickWidth+59, brickHeight, 1,GREY);
						drawRect((i*brickHeight)+1, (j*brickWidth)+61, brickHeight-2, brickWidth-2, RED);
					}
				}
			}
			
			drawRect(150, paddleLocation, 3, paddleWidth, WHITE);
			//all of the user interfacing!
			if(KEY_DOWN_NOW(BUTTON_LEFT))
			{
				paddleLocation--;
				if(paddleLocation<60)
					paddleLocation=60;
				else
					drawRect(150, paddleLocation+paddleWidth, 3, 1, BLACK);
			
			}
			if(KEY_DOWN_NOW(BUTTON_RIGHT))
			{
				paddleLocation++;
				if(paddleLocation>180-paddleWidth)
					paddleLocation=180-paddleWidth;
				else
					drawRect(150, paddleLocation-1, 3, 1, BLACK);
			
			}
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = 0;
			}
			boundsCheck(&ball_r, 0, 159, &dr, 2);
			boundsCheck(&ball_c, 60, 179, &dc, 2);
			paddleBoundsCheck(&ball_r, &ball_c, paddleLocation, paddleWidth, &dr);
			
			if((*(videoBuffer + OFFSET(ball_r+dr,ball_c+dc)) == GREY))
			{
				int i=(ball_r+dr)/12;
				int j=((ball_c+dc)-60)/24;
				blocks.matrix[i][j] = 0;
				if((*(videoBuffer + OFFSET(ball_r+dr,ball_c)) == GREY))
					dr = -dr;
				if((*(videoBuffer + OFFSET(ball_r,ball_c+dc)) == GREY))
					dc = -dc;
				drawRect((i*brickHeight), (j*brickWidth)+60, brickHeight, brickWidth, BLACK);
			}

			drawRect(ball_r, ball_c, 2, 2, BLACK);
			ball_r = ball_r + dr;
			ball_c = ball_c + dc;
			drawRect(ball_r, ball_c, 2, 2, WHITE);
			if(ball_r > 150) {
				lives--;
				drawRect(ball_r, ball_c, 2, 2, BLACK);
				drawRect(150, paddleLocation, 3, paddleWidth, BLACK);
				paddleLocation = 110;
				ball_r = 148;
				ball_c = paddleLocation+10;
				dr = 1;
				dc = 1;
				delay(5);
				if (lives==0)
					state=2;
			}
			int win =1;
			for (int i=0; i<5; i++) {
				for (int j=0; j<5; j++) {
					if (blocks.matrix[i][j] == 1)
						win = 0;
				}
			}
			if(win)
				state=2;
		}
		if (state == 2) {
			waitForVblank();
			drawImage3(0, 0, 240, 160, end_screen);
			if(KEY_DOWN_NOW(BUTTON_SELECT)) {
				state = 0;
			}
		}
	}
}



