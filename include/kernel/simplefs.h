#ifndef SIMPLEFS_H
#define SIMPLEFS_H

#include "types.h"



/* load_inode() */
/* save_inode() */

/* allocate_free_block() */
/* save_nth_block() */


/* static void debug   (Disk *disk); */
/* static bool format  (Disk *disk); */

bool	simplefs_mount   ();

/* int32_t     create_inode  (); */
/* bool        remove  (uint32_t inumber); */
/* int32_t     stat    (uint32_t inumber); */

/* int32_t     read    (uint32_t inumber, */ 
/* 		     char *data, */ 
/* 		     uint32_t length, */ 
/* 		     uint32_t offset); */

/* int32_t     write   (uint32_t inumber, char *data, uint32_t length, uint32_t offset); */

#endif
