#ifndef PROCESS_H
#define PROCESS_H


#include "types.h"


/*****************
 * Process states
 ****************/

#define P_READY    1
#define P_RUNNING  2
#define P_SLEEPING 3
#define P_ZOMBIE   3


/** max number of processes for process table*/
#define NUM_PROC 256

/** max length of the image file name*/
#define MAX_IMG_NAME 16

#define P_SYSTEM 0
#define P_USER   1



typedef struct Process{

    /*
     * This struct defines the main structure of the process.
     *
     * state must be one the pre defined states from above.
     *
     * exit_code should be set before process is either terminated or killed.
     *
     * */
    
    /*
     * READY
     * RUNNING
     * SLEEPING
     * ZOMBIE
     * */
   uint8_t state; 

   /*
    * the size of the page (4KB). In the multitasking OS it has to be a pagetable and even 
    * two pagetables in the modern OS.
    * */
   uint32_t size;
   /*
    * between 0 to 255
    * */
   uint8_t pid;

   /*
    * created by system or user
    * */
   uint8_t type;

   /*
    * file name or image file name which created the process.
    * max length is 15 + 1 (null byte)
    * */
   char name[MAX_IMG_NAME];

   /*
    * a pointer to parent process 
    * */
   struct Process *parent;
   

}Process_t;



Process_t * create_process();

void init_process(const char *name);

void exit_process();

int wait();

int kill();

void sleep();


#endif
