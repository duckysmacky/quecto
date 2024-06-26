/* OUTPUT */

#include "include/output.h"

void draw_buffer(buffer_t* buffer, state_t* state) {
	for (size_t i = 0; i <= buffer->row_s; i++) {
		mvprintw(i, 0, "%s", buffer->rows[i].contents);
	}
	move(state->crow, state->ccol);
}

void draw_status_bar(buffer_t* buffer, int* ch, state_t* state) {
	mvprintw(
		state->y - 1, state->x - 30, STATUSBAR_R,
		*ch, buffer->row_index + 1, buffer->cur_pos + 1, buffer->row_s + 1
	);
	move(state->crow, state->ccol);
}
