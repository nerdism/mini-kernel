
#include "kernel/kshell.h"
#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "kernel/simplefs.h"
#include "kernel/kmalloc.h"
#include "kernel/string.h"
#include "printf.h"

#define ENTER_KEY 13
#define BACKSPACE_KEY 8

static void read_line(char *buf) {
    
    char c;
    int i = 0;

    do {
	c =  keyboard_read(); 
	if (c < 0) continue;    
	if (i-1 < 0 && c == 8) continue;

	if (c == BACKSPACE_KEY) {
	    print_char(c);
	    buf[--i] = '\0';	
	    continue;
	}
	
	
	print_char(c); 
	buf[i++] = c;

	/* loop until enter key pressed */
    } while (c != ENTER_KEY);

    buf[i-1] = '\0';
}

static inline void mount() {
    if (simplefs_mount()) {
	printf("[+] simplefs mounted\n"); 
    }     
    else {
	printf("[-] mount failed\n"); 
    }
}

void kshell_run() {
    printf("[+] running kshell ...\n\n"); 
    char buf[256];
    while (true) {
	printf(">> ");
	read_line(buf);
	
	if (!strcmp(buf, "mount")) {
	    mount();
	}
    }
}

static inline bool is_alpha(char c) {
    if ((c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z')) {
	
	return true;	
    }
    return false;
}

static inline bool is_num(char c) {
    if ((c >= '0' && c <= '9')) {
	return true;	
    }
    return false;
}






