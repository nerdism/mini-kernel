#ifndef ATA_H
#define ATA_H

void ata_init();


/**
 * there are 10 registers for doing io per ata bus
 *
 * how to detect whether we have a hard drive or not ?
 * best method is IDENTIFY command
 * first we select a drive with sending 0xA0 to master drive
 * and 0xB0 to slave drive to port (PRIM_IO_BASE + 6)
 * which is drive register.
 * then set the sector count, sector num, cylinder low, cylinder high,
 * registers to zero.
 * the send the identify command (0xEC) to command regsiter (offset 7).
 * then read the status port if the value is zero the drive
 * does not exist
 *
 */


#endif
