
#include "drivers/screen.h"
#include "kernel/low_level.h"

#define VIDEO_TEXT_MODE 0xb8000
#define ROWS 25
#define COLS 80

#define CRT_ADDR_REG 0x03d4
#define CRT_DATA_REG 0x03d5
#define CRT_DATA_HIGH_BYTE 14
#define CRT_DATA_LOW_BYTE  15

#define RED_ON_BLACK   0b00001100
#define WHITE_ON_BLACK 0b00001111
#define VIDEO_END VIDEO_TEXT_MODE+(ROWS*COLS*2)

static char *cursor;
static int  video_end;


void print_char(char c) {
    int offset = ((int)cursor - VIDEO_TEXT_MODE)/2;
    
    if (c == '\n') {
	int t = (COLS - (offset % COLS))*2;
	if ((int)cursor + t >= video_end) {
	    scroll();	
	    cursor += (t-COLS*2);
	}
	else {
	    cursor += t;	
	}
    }
    else {
	if ((int)cursor + 2 > video_end) {
	    scroll();	
	    cursor -= (COLS*2);
	}
	cursor[0] = c; 
	cursor[1] = RED_ON_BLACK;
	cursor += 2;
    }
    set_cursor(((int)cursor-VIDEO_TEXT_MODE)/2);
}

void scroll() {
    char *cursor = (char*)VIDEO_TEXT_MODE;
    char *copy   = (char*)VIDEO_TEXT_MODE;

    /* int j = 0; */
    for (int i = COLS*2; i < ((ROWS)*COLS*2); i++) {
	cursor[0] = copy[i];
	cursor += 1;
    }
    for (int i = 0; i < COLS*2; i+=2) {
	cursor[i] = '\0';
	cursor[i+1] = WHITE_ON_BLACK;
    }
}

/* initialize screen curso */
void init_screen() {
    cursor = (char*)VIDEO_TEXT_MODE;
    video_end = VIDEO_END;    
    clear_screen();
}

void clear_screen() {
    char *screen = (char*)VIDEO_TEXT_MODE;
    for (int i = 0; i < (ROWS*COLS*2); i+=2) {
	screen[i] = '\0'; 
	screen[i+1] = WHITE_ON_BLACK;
    } 
}

/** @Param offset is the amount of distance from the 
 *   begining of the video memory without counting the 
 *   attribute bytes
 */
void set_cursor(int offset) {
    
    int high = offset / 256;
    int low  = offset % 256;
    /* select cursor location hight byte */
    outb(CRT_ADDR_REG, CRT_DATA_HIGH_BYTE);
    outb(CRT_DATA_REG, high);
    outb(CRT_ADDR_REG, CRT_DATA_LOW_BYTE);
    outb(CRT_DATA_REG, low);
}


