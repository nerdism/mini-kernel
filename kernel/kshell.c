
#include "kernel/kshell.h"
#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "printf.h"

static void read_line(char *buf) {
    
    char c;
    int i = 0;
    do {
	c =  keyboard_read(); 
	if (c < 0) continue;    
	buf[i++] = c;
    } while (c != 13);

}

void kshell_run() {
    printf("running kshell\n\n"); 
    char buf[256];
    while (true) {
	printf(">> ");
	read_line(buf);
    }
}


