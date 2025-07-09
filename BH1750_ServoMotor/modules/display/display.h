#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h> 

void display_init(void);
void display_clear(void);
void display_message(const char *message, int line);
void display_update(void);

#endif