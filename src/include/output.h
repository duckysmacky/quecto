#pragma once

#include "main.h"

// this just shuts up gcc so its fine
#ifdef _WIN32
    #define STATUSBAR_R "%04d | %.3llu:%.3llu (%.3llu)"
#else
    #define STATUSBAR_R "%04d | %.3zu:%.3zu (%.3zu)"
#endif // _WIN32

/* Draw buffer */
void draw_buffer(buffer_t* buffer, state_t* state);

/* Draw status bar */
void draw_status_bar(buffer_t* buffer, int* ch, state_t* state);
