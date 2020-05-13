#include "printf.h"
#include "memory_layout.h"
#include "string.h"
#include "types.h"
#include "memory.h"

#define KILO 1024
#define MEGA (KILO * KILO)

#define BLOCK 4096

/**
 * for more explanation about this struct go to 
 * memory_test.asm in boot folder 
 */
struct Mem_Info {
    uint32_t basel;  /* 64 bit base address */
    uint32_t baseh; 

    uint32_t limitl; /* 64 bit length */
    uint32_t limith;

    uint32_t type;
    uint32_t acpi;
};

/* beginning of the biggest free memory area */
void *memory_start;

/* where does bitmap structure in memory places*/
uint8_t *bitmap;    

/* how many byte is the bitmap structure */
uint32_t bitmap_bytes;

/* size of the bitmap */
uint32_t bitmap_bits;

/* set a specific bit in a bitmap byte to one 
 * which indicates that block has been occupied
 * @Param block_no is number of the nth block
 *  which is zero base
 * */
static inline void set_bitmap(uint32_t block_no) {
    bitmap[block_no/8] |= (128 >> (block_no)%8); 
}

/* set a specific bit in a bitmap byte to zero 
 * which indicates that block has been freed
 * @Param block_no is number of the nth block
 *  which is zero base
 */
static inline void unset_bitmap(uint32_t block_no) {
    bitmap[block_no/8] &= ~(128 >> (block_no)%8); 
}

/* check if a memory block is free return 1 if 
 * it is and zero if not
 * @Param block_no is number of the nth 4k block
 *  which is zero base
 */
static inline bool isblock_free(uint32_t block_no) {
    return !(bitmap[block_no/8] & (128 >> (block_no)%8));
}

void memory_init() {

    /* memory test entries count */
    uint32_t c = *((uint32_t*)MEM_INFO_CNT);  

    /* address of beginning of a memory region with biggest free area*/
    void    *mem_begin ;

    /* size of the biggest memory area*/
    uint32_t max_len = 0;

    /* pointer to memory information obtained by bios int 0x15 (look at the memory_test.asm */
    struct Mem_Info *mem_ptr = (struct Mem_Info*)MEM_INFO;


    /* determine biggest free memory area
     * @Todo change this so that it controles
     *	other memory regions
     **/
    for (int i = 0; i < c; i++) {
	if (mem_ptr[i].limitl > max_len) {
    	    mem_begin = (void*)mem_ptr[i].basel;	    	
	    max_len = mem_ptr[i].limitl;
	}
    }

    uint8_t ismb = 0;
    if (max_len > MEGA) ismb = 1;
    printf("free memory begin: 0x%h, len: %u %s\n",
		mem_begin,
		((ismb)? max_len/MEGA : max_len/KILO),
		((ismb)? "MB": "KB"));
    
    
    uint32_t bitmap_size = (max_len/BLOCK); /* [+] we discard the remaining bytes of the last block */

    /* we place the bitmap struct in the beginning of the free memory 
     * and later we will set the occupied Blocks to 1 in the bitmap
     */
    bitmap	 = mem_begin;
    bitmap_bits  = bitmap_size;
    bitmap_bytes = bitmap_size/8;

    memory_start = mem_begin;

    /* how many BLOCKs does bitmap struct has occupied */
    uint32_t bitmap_blocks = (bitmap_bytes/BLOCK)+1;
    printf("bitmap blocks: %u\n", bitmap_blocks);
    printf("bitmap size: %u bits or %u bytes\n", bitmap_bits, bitmap_bytes);


    memset(bitmap, 0, bitmap_bytes);

    /* set bitmap occupied memory blocks to 1 in the corresponding bits*/
    for (uint32_t i = 0; i < bitmap_blocks; i++) set_bitmap(i);
        
    /* for (uint32_t i = 0; i < bitmap_bytes; i++) printf("%h ", bitmap[i]); */
}


/* allocate blocks of memory with given size */
void *mem_alloc(uint32_t size) {
    uint32_t block_count = (size/BLOCK) + 1;
    
    /* beginning of free memory block index */
    uint32_t free_ind;

    uint32_t cnt = 0, i = 0;

    /* search for block_count number of free blocks of memory */
    while (cnt != block_count && i < bitmap_bits) {
	if (isblock_free(i)) {
	    if (!cnt) free_ind = i; 	
	    cnt++;
	}
	else {
	    cnt = 0;	
	}
	i++;
    }
    if (cnt == block_count) {
	int x = free_ind;
	while (cnt > 0) {
	    set_bitmap(x++);	
	    cnt--;
	} 
	return (void*)(memory_start+(free_ind*BLOCK));
    }
	

    return 0;
}


