#ifndef DISK_H
#define DISK_H

struct Disk {
    
    bool (*read_block) (uint32_t block_num, char *buf);

    bool (*write_block) (uint32_t block_num, char *buf);
};

extern struct Disk *disk;


#endif
