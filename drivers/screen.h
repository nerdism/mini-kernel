#ifndef SCREEN_H
#define SCREEN_H


/* set the cursor to begining of the video  memory*/
void init_screen();

/* print a string on the monitor */
void print(char *s);

void clear_screen();

/* set vga text mode cursor to specific area */
void set_cursor(int offset);

void scroll();
char *print_char(char c, char *cursor);

#endif

