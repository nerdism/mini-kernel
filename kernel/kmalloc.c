
#include "kmalloc.h"
#include "types.h"
#include "printf.h"

#define FREE_STATE  0xbcbcbcbc
#define USED_STATE  0x5e5e5e5e

#define KNSIZE  sizeof(struct Knode)

/* Doubly linked list node */
/* we use doubly linked list because merging memory chunks are
 * easier this way */
struct Knode {
    uint32_t	    state;
    void	    *addr;  /* it is possible to delete this */
    uint32_t	    length;
    struct Knode    *next;
    struct Knode    *prev;
};


/* Head of the double linked list */
struct Knode *khead;

static bool kmalloc_test() {
    struct Knode *ptr = khead;
    uint32_t ptr_len = ptr->length;
    void *ptr_addr = ptr->addr;
    uint32_t ptr_state = ptr->state;

    void *arr[10];
    for (int i = 0; i < 10; i++) {
	arr[i] = kmalloc(1000 * (i+1)); 
    }

    for (int i = 0; i < 10; i++) {
	kfree(arr[i]); 
    }

    if (khead->length == ptr_len   &&
        khead->addr == ptr_addr &&
	khead->state == ptr_state) {
	return 1;	
    }

    return 0;
}


/* static void kmalloc_debug() { */
/*     printf("NO  STATE    ADDR    LENGTH    NEXT    PREV\n"); */
/*     struct Knode *ptr = khead; */

/*     int32_t i = 0; */
/*     while (ptr) { */
		
/* 	printf("%d  %d     %h    %d      %d      %d\n", */ 
/* 	i++, */
/* 	(ptr->state == USED_STATE ? 1 : 0 ), */
/* 	ptr->addr, */
/* 	ptr->length, */
/* 	ptr->next, */
/* 	ptr->prev); */

/* 	ptr = ptr->next; */ 
/*     } */
/* } */

static bool kmalloc_test_run() {
    /* kmalloc_debug(); */
    /* printf("-----------------------\n"); */
    if (!kmalloc_test())
	return 0;
    /* kmalloc_debug(); */
    return 1;
}

/* Initialize the doubly linked list head */
void kmalloc_init(void *memory_start, uint32_t limit) {
    /* KNSIZE	  = sizeof(struct Knode); */

    khead	  = (struct Knode*)memory_start;
    khead->state  = FREE_STATE;
    khead->addr	  = memory_start + KNSIZE;
    khead->length = limit;
    khead->next	  = 0;
    khead->prev   = 0;

    
    if (!kmalloc_test_run())
	printf("[-] kmalloc failed\n");
    else
	printf("[+] kmalloc memory initialized\n");
}


/* Split a free state chunk into a used state chunk 
 * and a free state chunk with reminder size */
static void split_chunk(struct Knode *ptr, uint32_t length) {
    struct Knode *n_ptr = (void*)ptr + length;     

    /* Set new chunk attributes */
    n_ptr->state  = FREE_STATE;
    n_ptr->addr   = n_ptr + 1;
    n_ptr->length = ptr->length - length;
    n_ptr->next   = ptr->next; 
    n_ptr->prev   = ptr;

    if (ptr->next)
	ptr->next->prev = n_ptr;	

    /* Change the old chunk */
    ptr->length	  = length;
    ptr->next     = n_ptr;
    
}


/* Allocate the about of size memory 
 * returns the usable memory address */
void *kmalloc(uint32_t size) {
    
    /* Make size to be a multiple of the Knode size 
     * in order to avoid fragmentation */
    uint32_t ex = size % KNSIZE;
    if (ex)
	size += (KNSIZE - ex);

    size += KNSIZE;
    
    struct Knode *ptr = khead;
    
    bool found = 0;
    /* Find the proper length */
    while (ptr) {
	if (ptr->state == FREE_STATE && ptr->length >= size) {
	    found = 1;	
	    break;
	}
	ptr = ptr->next;	
    }

    if (found) {
	// split the chunk 
	if (ptr->length - size > 2 * KNSIZE)
	    split_chunk(ptr, size);	

	ptr->state = USED_STATE;

	/* return the memory after the knode */
	return (ptr+1);
    }
    
    printf("[-] Out of Kmalloc Memory !!!!!!!!\n");

    return 0;
}


static void kmerge_chunk(struct Knode *ptr) {

    /* ptr_next and ptr_prev */
    struct Knode *ptr_n, *ptr_p;

    ptr_n = ptr->next;
    ptr_p = ptr->prev;

    if (ptr_n && ptr_n->state == FREE_STATE) {
	ptr->length += ptr_n->length;
	ptr->next = ptr_n->next;
	if (ptr_n->next)
	    ptr_n->next->prev = ptr;
    }

    if (ptr_p && ptr_p->state == FREE_STATE) {
	ptr_p->length += ptr->length;	
	ptr_p->next = ptr->next;
	ptr_n->prev = ptr_p;
    }
}


/* free a kernel memory region */
void kfree(void *mem) {
    struct Knode *ptr = ((struct Knode*)mem) - 1;

    /* validate the given address */
    if (ptr->addr != mem || ptr->state != USED_STATE) {
	printf("[-] kfree invalid memory !!\n"); 
	return;
    }

    ptr->state = FREE_STATE;
    kmerge_chunk(ptr);
}




