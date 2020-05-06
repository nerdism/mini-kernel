
this file contains this mini kernel developers toughts and questions on their mind
which they go through the time


how can we program a bare metal machine (a computer which has not a installed os) ?

512 byte of every hard disk or usb or i think every storage device is called MBR (master boot record)
so when we turn on the computer the 512 byte of the default boot device (usb, hard disk, cdrom) is loaded
in to the main memory (ram) but that 512 must have spcial 2 byte at the end of it to be reconized as 
mbr. the last two byte of the mbr must be 0x55AA (but why ?)

where exactly bios loads mbr in to memory?
it loads mbr to address 0x7c00 (but why)
as far as i know its a standard for ibm computer to load at that place



