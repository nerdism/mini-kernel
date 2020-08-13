
#include "kernel/kshell.h"
#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "kernel/kmalloc.h"
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

void kshell_run() {
    printf("[+] running kshell ...\n\n"); 
    char buf[256];
    while (true) {
	printf(">> ");
	read_line(buf);
    }
}


char **split(char *line, char d, uint32_t *token_cnt) {
    
    char **tokens;
    bool is_delim = true;
    uint32_t i = 0, word_begin = 0;
    char c;
    while ((c = line[i])) {

	/* find delimeter */
	if (c == d) {
	    /* if previous char was a delim or nothing (in case the first char was delimeter)*/
	    if (!is_delim){
		

		/* make memory with word size */
		uint32_t word_size = i - word_begin; 
		char *str = (char*)kmalloc(word_size);
		/* copy line from word begin till the delim char */	
		is_delim = true;
	    } 
	}
	else {
	    	
	    if (is_delim) {
		word_begin = i-1; 
	    }

	    is_delim = false;
	}

	i++;
    }

    return tokens;
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


