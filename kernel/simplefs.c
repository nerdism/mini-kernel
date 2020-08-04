
#include "kernel/disk.h"
#include "types.h"
#include "kernel/simplefs.h"

/**
 * this simple file system idea/strcture was from noterdam university 
 * which we found in one of that univerity os course project
 * we will use this simpleos temporarily
 */



#define MAGIC_NUMBER 0xf0f03410
#define DISK_BLOCK_SIZE 4096
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK (DISK_BLOCK_SIZE/sizeof(uint32_t))
#define INODES_PER_BLOCK (DISK_BLOCK_SIZE/sizeof(struct Inode))
#define INODE_BLOCKS_OFFSET 1 /* Inodes block begin after super block*/

struct SuperBlock {
    uint32_t magic_number;
    uint32_t blocks; /* total number of blocks in disk */
    uint32_t inode_blocks; /* number of the inode blocks */
    uint32_t inodes; /* number of the inodes */
};

/* inode is a representaion of the file or directory */
struct Inode {
    uint32_t valid; /* is this inode used*/
    uint32_t size;  /* how many bytes has been written in to this inode */
    uint32_t directs[POINTERS_PER_INODE]; /* pointers to blocks fo data */
    uint32_t indirect; /* pointer to a block of pointers */
};

/* it's pointer to a inode which determines the type of the inode */
struct Dirent {
    uint32_t inode; /* inode number  */
    uint32_t type;  /* type of inode */
    uint32_t size;  /* size of inode */
    char name[32];  /* name of the item */
};

/**
 * a disk block can have multiple representations 
 * it can be a super block
 * it can be a block of inodes struct
 * it can be a block of pointer to data blocks
 * it can be a data block (raw bytes)
 */
union DiskBlock {
    struct SuperBlock super_block;
    struct Inode inodes[INODES_PER_BLOCK];
    uint32_t pointers[POINTERS_PER_BLOCK];
    char   data[DISK_BLOCK_SIZE];
};

static inline bool read_nth_block(uint32_t blockno, union DiskBlock *block) {

    if (disk->read_block(blockno, block->data)) {
	return true; 
    }
    return false;
}


static bool load_inode(uint32_t ino, struct Inode *inode) {

    /* calculate the block number that inode is reside in */
    uint32_t block_no = ino / INODES_PER_BLOCK;

    /* becuase inode blocks are not begin from zero they have offset */
    block_no = block_no + INODE_BLOCKS_OFFSET;
    
    /* read the block */
    union DiskBlock block;
    if (!read_nth_block(block_no, &block)) return false;
    

    ino = ino % INODES_PER_BLOCK; /* inode number inside one block */
    *inode = block.inodes[ino];

    /* is this inode used */
    if (!inode->valid) return false;
    
    return true;
    
}

#if defined(TEST) /* @TODO: change entire function */
int32_t read_inode (uint32_t inumber, 
		    char *data, 
		    uint32_t length, 
		    uint32_t offset) {
		     
    /* if (itable[inumber] == 0) return -1; */

    struct Inode node;
    // Load inode information
    load_inode(inumber, &node);

    if (offset > node.Size) return 0;

    // the beginning block
    uint32_t begin_block = (offset / Disk::BLOCK_SIZE) + 1;

    // actual length that can be read;
    uint32_t alength = node.size - offset;

    if (alength <= 0) return 0;

    /*
     * meaning: if length was greater than file size
     *          just make the actual length the 
     *          the remaining length of the file
     */ 
    if (alength >= length) alength = length;
        

    // number of the blocks should be read
    uint32_t num_block_read = ((offset - ((begin_block-1)*Disk::BLOCK_SIZE) +
                               alength) / Disk::BLOCK_SIZE) + 1;

    // printf("%d",num_block_read);
    
    uint32_t bytes_read = 0;

    for (uint32_t i = 0; i < num_block_read; i++) {
        Block block;
        if (!read_nth_block(inumber, begin_block+i, &block)) {

            // printf("here..........\n");
            return bytes_read;
        }

        // inside 
        /* uint32_t block_offset = i * Disk::BLOCK_SIZE; */
        uint32_t read_begin = offset % Disk::BLOCK_SIZE;
        uint32_t read_len;

        if (read_begin + alength < Disk::BLOCK_SIZE) {
            read_len = read_begin + alength; 
        }
        else {
            read_len = Disk::BLOCK_SIZE - read_begin; 
        }
        
        strncpy(data, &block.Data[read_begin], read_len);

        data += read_len;
        bytes_read += read_len;
        alength -= read_len;
    }
    
    return bytes_read;		     
		     
}
#endif


bool simplefs_mount() {
    /* is hard disk connected to the bus */
    if (!disk->identify()) {
	return false;
    }
    

    /* check if disk has simplefs */

    union DiskBlock block;
    uint32_t super_blockno = 0;
    read_nth_block(super_blockno, &block); /* read super block */

    if (block.super_block.magic_number != MAGIC_NUMBER) {
	return false; 
    }
    
    if (block.super_block.blocks != (*(disk->blocks) / 8)) {
	return false; 
    }

    /* @TODO: check if 10% of disk blocks are inodes blocks */

    
    return true;
}



