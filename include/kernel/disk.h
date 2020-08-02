#ifndef DISK_H
#define DISK_H

#include "types.h"

struct Disk {
    
    uint32_t *blocks; /* number of the ATA_SIZE blocks in the disk*/

    bool (*identify) ();

    bool (*read_block) (uint32_t block_num, char *buf);

    bool (*write_block) (uint32_t block_num, char *buf);
};

extern struct Disk *disk;



#endif
