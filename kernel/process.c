#include "kernel/process.h"


Process_t *current_process = 0;


Process_t * create_process() {

    /*
     * this function is responsible for allocating memory for the process
     * and set the process's fields with default values. 
     *
     * */
    return 0;
}

void init_process(const char *name) {

    /*
     * initialize the process fields with proper values
     * @name indicates which image file name or file created this process 
     *
     * */
    return;
}

void exit_process() {
    return;
}

int wait() {

    return 0;
}

int kill() {
    return 0;
}

void sleep() {
    return;
}
