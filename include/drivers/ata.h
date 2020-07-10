#ifndef ATA_H
#define ATA_H

#include "types.h"
void ata_init();

void ata_read_blocks(uint32_t lba, uint8_t block_cnt, void *buf);

void ata_write_blocks(uint32_t lba, uint8_t block_cnt, void *buf);

/**
 * usualy in a system there are 2 ata bus
 * and also can have 2 additional ata buses which makes it 4 ata bus
 * and each ata bus can have master and slave drives
 *
 * there are 10 registers for doing io per ata bus
 *
 * how to detect whether we have a hard drive or not in a ata bus ?
 * best method is IDENTIFY command
 * first we select a drive with sending 0xA0 to master drive
 * or 0xB0 to slave drive to port (ATA_BASE_ADDRESS + 6)
 * which is drive/head register.
 * then set the sector count, sector num, cylinder low, cylinder high,
 * registers to zero.
 * the send the identify command (0xEC) to command regsiter (offset 7).
 * then read the status port if the value is zero the drive
 * does not exist
 * but if DRQ bit is set and BSY bit is cleared 
 * then derive is ready for data transfer
 * read from the data register 256 times a 16 bit value
 *
 */

/*		Ata status register
 * -------------------------------------------------
 * | BSY | RDY | DF | SRV | DRQ | CORR | IDX | ERR |
 * -------------------------------------------------
 *
 *	ERR 	Indicates an error occurred. 
 *		Send a new command to clear it 
 *		(or nuke it with a Software Reset).
 *
 *	IDX 	Index. Always set to zero.
 *
 *	CORR 	Corrected data. Always set to zero.
 *
 *	DRQ 	Set when the drive has PIO data to transfer, 
 *		or is ready to accept PIO data.
 *
 *	SRV 	Overlapped Mode Service Request.
 *
 *	DF 	Drive Fault Error (does not set ERR).
 *
 *	RDY 	Bit is clear when drive is spun down, 
 *		or after an error. Set otherwise.
 *
 *	BSY 	Indicates the drive is preparing to send/receive data 
 *		(wait for it to clear). 
 *		In case of 'hang' (it never clears), do a software reset
 */

/**
 * for a more comprehensive explination have look at
 * https://wiki.osdev.org/ATA_PIO_Mode
 * or if link is down search "ata pio mode"
 */

#endif
