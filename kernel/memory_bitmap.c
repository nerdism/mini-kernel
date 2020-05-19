
#include "printf.h"
#include "types.h"
#include "memory_bitmap.h"
#include "string.h"

/* Memory Block Size */
#define BLOCK 4096

/* beginning of the memory which bitmap structure handles it */
void    *bitmap_memory_start;

/* pointer to the bitmap structure */
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


/* static void print_bitmap() { */
/*     for (uint32_t i = 0; i < bitmap_bytes; i++) printf("%h ", bitmap[i]); */
/* } */


static bool bitmap_mem_test() {
     /* test memory allocate */
    mem_alloc(3*4096);
    void *m2 = mem_alloc(4*4096);
    mem_alloc(4*4096);
    mem_free(m2, 4*4096);
    if (!isblock_free(4) ||
        !isblock_free(5) ||
	!isblock_free(6) ||
	!isblock_free(7)) {

	return 0;	
    }
    return 1;
}


void bitmap_memory_init(void *memory_start, uint32_t limit) {

    uint32_t bitmap_size = (limit/BLOCK); /* [+] we discard the remaining bytes of the last block */

    /* we place the bitmap struct in the beginning of the free memory 
     * and later we will set the occupied Blocks to 1 in the bitmap
     */
    bitmap_memory_start = memory_start;
    bitmap	 = memory_start;
    bitmap_bits  = bitmap_size;
    bitmap_bytes = bitmap_size/8;

    /* how many BLOCKs does bitmap struct has occupied */
    uint32_t bitmap_blocks = (bitmap_bytes/BLOCK)+1;

    /* printf("bitmap blocks: %u\n", bitmap_blocks); */
    /* printf("bitmap size: %u bits or %u bytes\n", bitmap_bits, bitmap_bytes); */

    memset(bitmap, 0, bitmap_bytes);

    /* set bitmap occupied memory blocks to 1 in the corresponding bits*/
    for (uint32_t i = 0; i < bitmap_blocks; i++) set_bitmap(i);


    if (!bitmap_mem_test()) printf("bitmap memory failed\n");
    else printf("[+] bitmap memory initialized\n");
}

/* allocate blocks of memory with given size */
void *mem_alloc(uint32_t size) {
    if (size == 0) return (void*)0;
    uint32_t block_cnt = (size/BLOCK) + (size%BLOCK!=0);
    
    /* beginning of free memory block index */
    uint32_t free_ind;

    uint32_t cnt = 0, i = 0;

    /* search for block_cnt number of free blocks of memory */
    while (cnt != block_cnt && i < bitmap_bits) {
	if (isblock_free(i)) {
	    if (!cnt) free_ind = i; 	
	    cnt++;
	}
	else {
	    cnt = 0;	
	}
	i++;
    }
    if (cnt == block_cnt) {
	int x = free_ind;
	while (cnt > 0) {
	    set_bitmap(x++);	
	    cnt--;
	} 
	return (void*)(bitmap_memory_start+(free_ind*BLOCK));
    }
	

    return 0;
}


void mem_free(void *mem_ptr, uint32_t size) {
    if (size == 0) return;
    uint32_t block_cnt = (size/BLOCK) + (size%BLOCK!=0);
    
    uint32_t block_begin = ((mem_ptr-bitmap_memory_start)/BLOCK);

    uint32_t i = 0;
    while (i < block_cnt) {
	unset_bitmap(block_begin++);
	i++;
    }
}




