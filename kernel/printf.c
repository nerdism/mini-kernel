
#include <stdarg.h>
#include "drivers/screen.h"
#include "printf.h"
#include "types.h"


void printf(const char *format, ...) {
    va_list list;

    /* arg place holder */
    int d;
    char c, *s;


    va_start(list, format);
    int flag = 0; 
    int i = 0;
    while (format[i]) {
	if (flag) {
	    flag = 0;	
	    i++;
	    continue;
	}
	if (format[i] == '%') {
	    flag = 1;
	    if (!format[i+1]) break;
	    switch (format[i+1]) {
		case 'c':
		    c = (char)va_arg(list, int);	     
		    print_char(c);
		    break;

		case 'd':
		    d = va_arg(list, int32_t);
		    print_int32(d);
		    break;
		case 'u':
		    d = va_arg(list, uint32_t);
		    print_uint32(d);
		    break;
		case 'h':
		    d = va_arg(list, uint32_t);
		    print_hex(d);
		    break;
		case 's':
		    s = va_arg(list, char*);	    		    
		    print_str(s);
		    break;
	    }	
	
	}
	else {
	    print_char(format[i]);	
	}
	i++;
    }
    va_end(list);
}

/* print a number in to the screen */
void print_int32(int32_t d) {
    char str[30];
    if (d == 0) {print_char('0'); return;}

    int n = d, i = 0, minus = 0;
    if (n < 0) {
	minus = 1;
	n *= -1;
    }
    while (n > 0) {
	str[i++] = (char)((n%10) + '0');
	n /= 10;
    }
    if (minus) str[i] = '-';
    else i--;
    
    for (int j = i; j >= 0; j--) {
	print_char(str[j]); 
    }
}

/* print a unsigend number in to the screen */
void print_uint32(uint32_t d) {
    char str[20];
    if (d == 0) {print_char('0'); return;}

    uint32_t n = d, i = 0;
    while (n > 0) {
	str[i++] = (char)((n%10) + '0');
	n /= 10;
    }
    i--; 

    for (int j = i; j >= 0; j--) {
	print_char(str[j]); 
    }
}

/* print a unsigend number in to the screen */
void print_hex(uint32_t d) {
    char hex[16] = {'0', '1', '2', '3',
		    '4', '5', '6', '7',
		    '8', '9', 'A', 'B',
		    'C', 'D', 'E', 'F'};
    char str[20];
    if (d == 0) {print_char('0'); return;}

    uint32_t n = d, i = 0;
    while (n > 0) {
	str[i++] = ((hex[n%16]));
	n /= 16;
    }
    i--; 

    for (int j = i; j >= 0; j--) {
	print_char(str[j]); 
    }
}

/* print a string into the screen */ 
void print_str(char *s) {
    int i = 0;
    while (s[i] != '\0') {  	
	print_char(s[i++]);
    }
}



