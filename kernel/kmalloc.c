
#include "kmalloc.h"
#include "types.h"
#include "printf.h"


struct Knode {
    uint32_t	    state;
    void	    *addr;
    uint32_t	    length;
    struct Knode    *next;
};


struct Knode *khead;


void kmalloc_init(void *memory_start, uint32_t limit) {
    
}
