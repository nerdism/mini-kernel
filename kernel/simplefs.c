

#include "types.h"

#define MAGIC_NUMBER 0xf0f03410
#define DISK_BLOCK_SIZE 4096
#define POINTERS_PER_INODE 5
#define POINTERS_PER_BLOCK (DISK_BLOCK_SIZE/sizeof(uint32_t))
#define INODES_PER_BLOCK (DISK_BLOCK_SIZE/sizeof(struct Inode))



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

/* it's pointe to a inode which determine the type of the inode */
struct Disk_Item {
    uint32_t inode; /* inode number  */
    uint32_t type;  /* type of inode */
    uint32_t size;  /* size of inode */
    char name[32];  /* name of the item */
};

union DiskBlock {
    struct SuperBlock super_block;
    struct Inode inodes[INODES_PER_BLOCK];
    uint32_t pointers[POINTERS_PER_BLOCK];
    char   data[DISK_BLOCK_SIZE];
};


/* bool simplefs_mount(Disk *disk) { */
/*     return 1; */
/* } */

