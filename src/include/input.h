#ifndef __INPUT_H__
#define __INPUT_H__

#include "main.h"

/* Shift rows left */
void shift_rows_left(buffer_t* buf, size_t index);

/* Shift rows right */
void shift_rows_right(buffer_t* buf, size_t index);

/* Shift single row left */
void shift_row_left(row_t* row, size_t index);

/* Shift single row right */
void shift_row_right(row_t* row, size_t index);

/* Append rows */
void append_rows(row_t* a, row_t* b);

/* Delete and append a row */
void delete_and_append_row(buffer_t* buf, size_t index);

/* Create and cut a row */
void create_and_cut_row(buffer_t* buf, size_t dest_index, size_t* str_s, size_t index);

/*
    Process keypress
    @param ch keyboard key
    @param buffer buffer
*/
void process_keypress(int ch, buffer_t* buffer);

#endif