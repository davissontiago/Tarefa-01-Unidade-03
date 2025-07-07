#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h> 

void init_display(void);
void clear_display(void);
void message_display(const char *message, int line);
void update_display(void);

#endif