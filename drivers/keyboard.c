#include "drivers/keyboard.h"
#include "drivers/screen.h"
#include "kernel/interrupts.h"
#include "kernel/low_level.h"
#include "keycodes.h"
#include "printf.h"
#include "types.h"



#define BUF_SIZE 256

int input_buffer  = 0; // read from keyboard
int output_buffer = 0; // send command to keyboard

uint8_t keyboard_buffer[BUF_SIZE];

/* MUST be static as per each interrupt these modes should preserve their states. */
static uint8_t shift = 0;
static uint8_t alt   = 0;

uint8_t to_keycode(uint8_t scan_code) {


    if (scan_code & 0x80) {
        /* key is released */

        scan_code &= 0x7F;
        
        if (scan_code == LSHIFT || scan_code == RSHIFT) {
            shift = 0;
        }

        if (scan_code == ALT) {
            alt = 0;
        }
    }

    else {
        /* key is pressed */

        if (scan_code == LSHIFT || scan_code == RSHIFT) {
            shift = 1;
        }

        if (scan_code == ALT) {
            alt = 1;
        }
        
        if (shift) {
            return keycodes[scan_code][1];
        }
        else {
            return keycodes[scan_code][0];
        }
    }

    return INVALID;
}

char keyboard_read() {

    if (input_buffer == output_buffer) return -1;

    char c = keyboard_buffer[output_buffer++];
    output_buffer %= BUF_SIZE;


    return c;
}

void keyboard_interrupt_handler(int intr_num, int code) {
   
    uint8_t scan_code = inb(PORT);
    char c = to_keycode(scan_code);

    if (c == INVALID) return;

    //TODO: bug alert here! the last character to read might be overwrited.
    if ((input_buffer + 1) == (output_buffer % BUF_SIZE)) return; 

    keyboard_buffer[input_buffer++] = c;
    

    input_buffer %= BUF_SIZE;
}


void keyboard_init() {
   
    intr_handler_register(33, keyboard_interrupt_handler);
    intr_enable(33);

    printf("[+] keyboard ready\n");

}
