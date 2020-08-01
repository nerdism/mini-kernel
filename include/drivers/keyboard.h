#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h"

uint8_t to_keycode(uint8_t scan_code);
char keyboard_read();
void keyboard_interrupt_handler();
void keyboard_init();

#endif
