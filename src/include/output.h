#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "main.h"

/* Draw buffer */
void draw_buffer(buffer_t* buffer, pstate_t state);

/* Draw status bar */
void draw_status_bar(buffer_t* buffer, int* ch, pstate_t state);

#endif