#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "main.h"

#ifdef _WIN32
    #define STATUSBAR_R "%04d | %.3llu:%.3llu (%.3llu)"
#else
    #define STATUSBAR_R "%04d | %.3zu:%.3zu (%.3zu)"
#endif // _WIN32

/* Draw buffer */
void draw_buffer(buffer_t* buffer, pstate_t state);

/* Draw status bar */
void draw_status_bar(buffer_t* buffer, int* ch, pstate_t state);

#endif