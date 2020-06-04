
/**
 * this file contains ata driver implemention
 */

#include "ata.h"
#include "printf.h"
#include "low_level.h"

/* primary bus io registers base address	*/
#define PRIM_IO_BASE 0x1f0

/* primary bus control registers base address	*/
#define PRIM_CTRL_BASE 0x3f6

/* secondary bus io registers base address	*/
#define SCND_IO_BASE 0x170

/* secondary bus control registers base address */
#define SCND_CTRL_BASE 0x376


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


#define STATUS_ERR 0x01	    /* indicates an error occureed		      */
#define STATUS_DRQ 0x08	    /* set when data has pio data to transfer	      */
#define STATUS_DF  0x40	    /* Drive fault error (does not set ERR)	      */
#define STATUS_BSY 0x80	    /* indicates the drive is preparing to send/recv  */

#define IDENTIFY_COMMAND 0xEC 

/* Primary address. register address	*/
#define PA(REG) PRIM_IO_BASE + REG

/* Secondary address. register address	*/
#define SA(REG) SCND_IO_BASE + REG

static inline uint8_t read_status_reg() {
    return inb(PA(ERROR_REG));
}

static inline bool pio_ready() {
    uint8_t stat = read_status_reg();

    /* if error bits are set */
    if ((stat & STATUS_ERR) || (stat & STATUS_DF)) {
	return 0;
    }
    /* check if busy bit cleard and drq bit set */
    else if (!(stat & STATUS_BSY) && (stat & STATUS_DRQ)) {
	return 1;
    }
    else 
	return 0;
}


void ata_init() {
    outb(PA(SECTCNT_REG), 0xA0);    
    outb(PA(SECTNUM_REG), 0);    
    outb(PA(CYLLOW_REG ), 0);    
    outb(PA(CYLHIGH_REG), 0);    

    outb(PA(COMMAND_REG), IDENTIFY_COMMAND);

    uint8_t status;
    /* do { */
	/* status = read_status_reg(); */
	/* printf("status: %u\n", status); */
    /* } while(status == 1); */
}



