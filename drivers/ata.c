
/**
 * this file contains ata driver implemention
 *
 * @TODO change below comment position
 * In short, volumes exist at the logical OS level,
 * and partitions exist at the physical,
 * media specific level.
 * Sometimes there is a one-to-one correspondence, 
 * but this is not guaranteed. 
 *
 * @TODO must of the code below is assumed
 *  that we are only using ata0 bus and master
 *  hard driver. so please write more general
 *  code which works for all buses and slaves too.
 */

#include "drivers/ata.h"
#include "drivers/pit.h"
#include "types.h"
#include "printf.h"
#include "kernel/low_level.h"
#include "kernel/kmalloc.h"
#include "kernel/string.h"



/* primary bus io registers base address	*/
#define PRIM_IO_BASE	0x1f0

/* primary bus control registers base address	*/
#define PRIM_CTRL_BASE	0x3f6

/* secondary bus io registers base address	*/
#define SCND_IO_BASE	0x170

/* secondary bus control registers base address */
#define SCND_CTRL_BASE	0x376


/* offset from io base */
#define DATA_REG    0	/* Data register	  R/W */
#define ERROR_REG   1	/* Error register	  R   */
#define FEATURE_REG 1	/* Features register        W */
#define SECTCNT_REG 2   /* Sector count register  R/W */
#define SECTNUM_REG 3	/* Sector number register R/W */
#define CYLLOW_REG  4	/* Cylindar low register  R/W */
#define CYLHIGH_REG 5	/* Cylindar high register R/W */
#define DRHE_REG    6	/* Drive / head register  R/W */
#define STATUS_REG  7	/* Status register	  R   */
#define COMMAND_REG 7	/* Command register	    W */


/* offset from control base */
#define ASTATUS_REG 0 /* Alternate status register R   */
#define DEVCTRL_REG 0 /* Device control register     W */
#define DRADDR_REG  1 /* Drive Adress register     R   */


#define STATUS_ERR 0x01	    /* indicates an error occured		      */
#define STATUS_IDX 0x02	    /* index. always set to zero		      */
#define STATUS_COR 0x04	    /* corrected data. always set to zero	      */
#define STATUS_DRQ 0x08	    /* set when data has pio data to transfer	      */
#define STATUS_SRV 0x10	    /* Overlapped Mode Service Request. 	      */
#define STATUS_DF  0x20	    /* Drive fault error (does not set ERR)	      */
#define STATUS_RDY 0x40	    /* driver ready				      */
#define STATUS_BSY 0x80	    /* indicates the drive is preparing to send/recv  */

/* useful commands */
#define IDENTIFY_COMMAND 0xEC 
#define RESET_COMMAND	 0x04
#define READ_SECTORS	 0x20
#define WRITE_SECTORS	 0x30


/* Base Address register type */
#define IO	0
#define CTRL	1

static uint16_t prim_bus[2] = {PRIM_IO_BASE, PRIM_CTRL_BASE};



/* read a byte from a register 
 * @Param type type of the base address
 * @Param reg  register name (macro name)
 *  
 *  ex.   readb_reg(IO,	STATUS_REG);
 **/
static inline uint8_t readb_reg(uint8_t type, uint8_t reg) {
    return inb(prim_bus[type] + reg);
}

/* read a word from a register 
 * paramaters are as same as readb_reg function */
static inline uint16_t readw_reg(uint8_t type, uint8_t reg) {
    return inw(prim_bus[type] + reg);
}

/* write a byte to a register 
 * @Param type type of the base address
 * @Param reg  register name (macro name)
 * @Param data  one byte data to be send
 *
 *  ex.   writeb_reg(IO, DRHE_REG, RESET_COMMAND);
 *  ex.   writeb_reg(IO, DATA_REG, 0xff);
 */
static inline void writeb_reg(uint8_t type, uint8_t reg, uint8_t data) {
    outb(prim_bus[type] + reg, data);
}

static inline void writew_reg(uint8_t type, uint8_t reg, uint16_t data) {
    outw(prim_bus[type] + reg, data);
}

static inline bool pio_ready() {
    
    while (1) {
	clock_wait(1);
	uint8_t stat = readb_reg(IO, STATUS_REG);
	/* if error bits are set */
	if ((stat & STATUS_ERR) || (stat & STATUS_DF)) {
	    return false;
	}
	/* check if busy bit cleard and drq bit set */
	else if (!(stat & STATUS_BSY) && (stat & STATUS_DRQ)) {
	    return true;
	}
    }
    
}

static inline void ata_soft_reset() {
    writeb_reg(CTRL, DEVCTRL_REG, RESET_COMMAND);
    clock_wait(1);
    writeb_reg(CTRL, DEVCTRL_REG, 0);
    clock_wait(1);
}

/* @TODO: after having a architecture remove this */
uint32_t ata_blocks;

/* 
 * detect if a ata device such as hard disk
 * or cdrom is connected to the ata bus
 */
bool ata_detect() {
    uint8_t status;

    status = readb_reg(IO, STATUS_REG);

    /* this means nothing attached to ata bus */
    if (status == 0xff) {
	return false;
    }


    /* ata_soft_reset(); */

    writeb_reg(IO, DRHE_REG, 0xA0); /* why 0xa0 ? */
    clock_wait(1);

    writeb_reg(IO, SECTCNT_REG, 0);
    writeb_reg(IO, SECTNUM_REG, 0);
    writeb_reg(IO, CYLLOW_REG,  0);
    writeb_reg(IO, CYLHIGH_REG, 0);

    writeb_reg(IO, COMMAND_REG, IDENTIFY_COMMAND);
    clock_wait(1);
    status = readb_reg(IO, STATUS_REG);
    if (!pio_ready()) {
	return false;
    }
    uint16_t buf[256];
    
    for (int i = 0; i < 256; i++) {
	buf[i] =  readw_reg(IO, DATA_REG);
    }
    printf("cylinders:           %u\n", buf[1]);
    printf("heads:               %u\n", buf[3]);
    printf("sector per track:    %u\n", buf[6]);

    ata_blocks = *((uint32_t*)&buf[57]);
    printf("number of the blocks:    %u\n", ata_blocks);
    
    return true;
}

#if defined(DEBUG)
static void print_blocks(void *buf, uint32_t blocks_cnt) {

    for (int i = 0; i < ATA_BLOCK_SIZE * blocks_cnt; i++) {
	printf("%c,", ((char*)buf)[i]); 
    }
}
#endif

void ata_init() {
    
    /* see if we have any connected device */
    if (ata_detect()) {
	printf("[+] ata0 initialized\n"); 
    } 
    else {
	printf("[-] ata0 does not exist\n"); 
    }
    
    #if defined(DEBUG)
    /* uint32_t cnt = 2; */
    /* char wbuf[cnt * ATA_BLOCK_SIZE]; */
    /* memset(wbuf, 'a', ATA_BLOCK_SIZE); */
    /* memset(&wbuf[ATA_BLOCK_SIZE], 'b', ATA_BLOCK_SIZE); */
    
    /* ata_write_blocks(17, cnt, wbuf); */

    /* char buf[cnt * ATA_BLOCK_SIZE]; */
    /* ata_read_blocks(17, cnt, buf); */ 

    /* print_blocks(buf, cnt); */
    #endif
    
}

/* sends some io command for reading/writing from/to data register*/
inline static void ata_handle_io(uint32_t lba, uint8_t block_cnt, uint8_t command) {

    writeb_reg(IO, DRHE_REG, (0xE0 | ((lba >> 24) & 0x0f)));
    writeb_reg(IO, FEATURE_REG, 0); /* can be ignored */
    writeb_reg(IO, SECTCNT_REG, block_cnt);

    writeb_reg(IO, SECTNUM_REG, lba);
    writeb_reg(IO, CYLLOW_REG,  lba >> 8);
    writeb_reg(IO, CYLHIGH_REG, lba >> 16);

    writeb_reg(IO, COMMAND_REG, READ_SECTORS);

}


bool ata_read_blocks(uint32_t lba, uint8_t block_cnt, void *buf) {
    
    
    ata_handle_io(lba, block_cnt, READ_SECTORS);  

    uint16_t cnt = (block_cnt == 0 ? 256 : block_cnt);
    uint16_t *buffer = (uint16_t*)buf;  	
    while (cnt > 0) {
	/* clock_wait(1); /1* should wait 400 ns for every successive read or writes *1/ */	
	if (pio_ready()) {
	    for (int i = 0; i < 256; i++) {
		buffer[i] = readw_reg(IO, DATA_REG);	
	    }
	    cnt--;
	    buffer += 256;

	}
	else {
	    return false; 
	}

    }
    printf("read %d blocks", block_cnt);
    return true;
}


bool ata_write_blocks(uint32_t lba, uint8_t block_cnt, void *buf) {

    ata_handle_io(lba, block_cnt, WRITE_SECTORS);  
    
    uint16_t cnt = (block_cnt == 0 ? 256 : block_cnt);
    uint16_t *buffer = (uint16_t*)buf;  	
    while (cnt > 0) {
	if (pio_ready()) {
	    for (int i = 0; i < 256; i++) {
		writew_reg(IO, DATA_REG, buffer[i]);	
	    }
	    cnt--;
	    buffer += 256;
	}
	else {
	    return false; 
	}
    }
    printf("write %d blocks", block_cnt);
    return true; 
}


