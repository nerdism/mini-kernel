
#include "drivers/ata.h"
#include "kernel/disk.h"

#define MULTIPLIER 8
#define BLOCK_SIZE (MULTIPLIER * ATA_BLOCK_SIZE)

/* read 4k blocks */
static bool read_block(uint32_t block_num, char *buf) {
    
    /* TODO
     * should we check the hard disk and fs sector boundry ?
     */
    if (ata_read_blocks(block_num * MULTIPLIER, MULTIPLIER, buf)) {
	return true; 
    }

    return false;
}

/* write 4k blocks */
static bool write_block(uint32_t block_num, char *buf) {

    if (ata_write_blocks(block_num * MULTIPLIER, MULTIPLIER, buf)) {
	return true; 
    }

    return false;
}

extern uint32_t ata_blocks;

struct Disk _disk = {
    .blocks	 = &ata_blocks,
    .identify    = ata_detect,
    .read_block  = read_block,
    .write_block = write_block
};

struct Disk *disk = &_disk;

