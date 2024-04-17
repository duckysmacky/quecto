#ifndef MAIN_H
#define MAIN_H

// OS-dependent libs
#ifdef _WIN32
    #include <ncurses/curses.h>
    #include <stdio.h>
#else
    #include <curses.h>
#endif

#include <assert.h>
#include <string.h>
#include <stdlib.h>

// key defines
#define ctrl(x) ((x) & 0x1f)

#define ESCAPE 27
#define ENTER 10
#define TAB 9

// ctrl + arrow has to be compared with the keyname() function
#define CTRLUP "kUP5"
#define CTRLDOWN "kDN5"
#define CTRLLEFT "kLFT5"
#define CTRLRIGHT "kRIT5"

// limit defines
#define MAX_ROWS 1024
#define MAX_STRING_SIZE 1025

#define ESCDELAY 10

/* DATA */

typedef struct ROW {
	size_t index;
	size_t size;
	char* contents;
} row_t;

typedef struct BUFFER {
	row_t rows[MAX_ROWS];
	size_t row_index;
	size_t cur_pos;
	size_t row_s;
	char* filename;
} buffer_t;

#endif