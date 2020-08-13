
#include "types.h"


void *memset(void *s, uint8_t c, uint32_t n) {
    uint32_t x = 0;
    uint8_t *a = (uint8_t *)s; 
    while (x < n) {
	a[x++] = c; 
    }
    return s;
}

int32_t strcmp(const char *s1, const char *s2) {
    
    bool is_equal = true;
    while (*s1 && *s2) {
	if (*s1 != *s2) {
	    is_equal = false;
	    break;
	}	
	++s1;++s2;
    }
    if (is_equal && *s1 == *s2) return 0;
    if (*s1 > *s2)  return 1;
    else	    return -1; 
}

#if defined(FUTURE)
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
#endif



