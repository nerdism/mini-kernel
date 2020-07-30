
#include "drivers/ata.h"
#include "kernel/disk.h"

#define MULTIPLIER 8
#define BLOCK_SIZE (MULTIPLIER * ATA_BLOCK_SIZE)

/* read 4k blocks */
static bool read_block(uint32_t block_num, char *buf) {
    
     
    return 1;
}

/* write 4k blocks */
static bool write_block(uint32_t block_num, char *buf) {

    return 1;
}

struct Disk _disk = {
    .read_block  = read_block,
    .write_block = write_block
};

struct Disk *disk = &_disk;

