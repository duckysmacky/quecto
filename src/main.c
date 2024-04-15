/*
	quecto - text editor made with ncurses
	inspired by nano, cano, nino and kilo
	(not a joke)
	(c) shroomwastaken 2024
*/

#include <curses.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// key defines
#define ctrl(x) ((x) & 0x1f)

#define ESCAPE 27
#define ENTER 10
#define TAB 9

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


struct STATE {
	int y, x;
	int crow, ccol;
} state;

char QUIT = 0;

/* INPUT */

// most of these are borrowed from cano

void shift_rows_left(buffer_t* buf, size_t index) {
	assert(buf->row_s + 1 < MAX_ROWS);
	for(size_t i = index; i < buf->row_s; i++) {
		buf->rows[i] = buf->rows[i + 1];
	}
	buf->rows[buf->row_s].size = 0;
	buf->row_s--;
}

void shift_rows_right(buffer_t* buf, size_t index) {
	assert(buf->row_s + 1 < MAX_ROWS);
	char* new = calloc(MAX_STRING_SIZE, sizeof(char));
	for(size_t i = buf->row_s + 1; i > index; i--) {
		buf->rows[i] = buf->rows[i - 1];
	}
	buf->rows[index] = (row_t) {0};
	buf->rows[index].contents = new;
	buf->rows[index].index = index;
	buf->row_s++;
}

void shift_row_left(row_t* row, size_t index) {
	for(size_t i = index; i < row->size; i++) {
		row->contents[i] = row->contents[i + 1];
	}
	row->size--;
}

void shift_row_right(row_t* row, size_t index) {
	row->size++;
	for(size_t i = row->size; i > index; i--) {
		row->contents[i] = row->contents[i - 1];
	}
}

void append_rows(row_t* a, row_t* b) {
	assert(a->size + b->size < MAX_STRING_SIZE);
	for(size_t i = 0; i < b->size; i++) {
		a->contents[(i + a->size)] = b->contents[i];
	}
	a->size = a->size + b->size;
}

void delete_and_append_row(buffer_t* buf, size_t index) {
	append_rows(&buf->rows[index-1], &buf->rows[index]);
	shift_rows_left(buf, index);
}

void create_and_cut_row(buffer_t* buf, size_t dest_index, size_t* str_s, size_t index) {
	assert(index < MAX_STRING_SIZE);
	assert(dest_index > 0);
	size_t final_s = *str_s - index;
	char* temp = calloc(final_s, sizeof(char));
	size_t temp_len = 0;
	for(size_t i = index; i < *str_s; i++) {
		temp[temp_len++] = buf->rows[dest_index-1].contents[i];
		buf->rows[dest_index-1].contents[i] = '\0';
	}
	shift_rows_right(buf, dest_index);
	strncpy(buf->rows[dest_index].contents, temp, sizeof(char) * final_s);
	buf->rows[dest_index].size = final_s;
	*str_s = index;
	free(temp);
}

void process_keypress(int ch, buffer_t* buffer) {
	if (ch == ctrl('s') || ch == ctrl('S')) {
		FILE* file = fopen(buffer->filename, "w");
		for (size_t i = 0; i <= buffer->row_s; i++) {
			fwrite(buffer->rows[i].contents, buffer->rows[i].size, 1, file);
			fputc('\n', file);
		}
		fclose(file);
		QUIT = 1;
	} else if (ch == KEY_DC) {
		// ill add it later
	} else if (ch == KEY_BACKSPACE) {
		if (buffer->cur_pos == 0) {
			if (buffer->row_index != 0) {
				row_t* cur = &buffer->rows[--buffer->row_index];
				buffer->cur_pos = cur->size;
				move(buffer->row_index, buffer->cur_pos);
				delete_and_append_row(buffer, cur->index + 1);
			}
		} else {
			row_t* cur = &buffer->rows[buffer->row_index];
			shift_row_left(cur, --buffer->cur_pos);
			move(state.y, buffer->cur_pos);
		}
	} else if (ch == ENTER) {
		row_t* cur = &buffer->rows[buffer->row_index];
		create_and_cut_row(buffer, buffer->row_index + 1, &cur->size, buffer->cur_pos);
		buffer->row_index++;
		buffer->cur_pos = 0;
		move(buffer->row_index, buffer->cur_pos);
	} else if (ch == ctrl(ch)) {
		// dont add ctrl characters to buffer
		// this wont work for ctrl + j and ctrl + m cause theyre the same as enter
	} else if (ch == KEY_UP) {
		if (buffer->row_index != 0) buffer->row_index--;
		// snap cursor to beginning of line if at first line
		else buffer->cur_pos = 0;
	} else if (ch == KEY_DOWN) {
		if (buffer->row_index < buffer->row_s) buffer->row_index++;
		// snap cursor to eol if at last line
		else buffer->cur_pos = buffer->rows[buffer->row_index].size;
	} else if (ch == KEY_LEFT) {
		if (buffer->cur_pos != 0) { buffer->cur_pos--; }
		// go to previous line if at beginning of line and not at first line
		else if (buffer->row_index != 0) {
			buffer->row_index--;
			buffer->cur_pos = buffer->rows[buffer->row_index].size;
		}
	} else if (ch == KEY_RIGHT) {
		buffer->cur_pos++;
		// go to next line if at eol and not at last line
		if (buffer->cur_pos > buffer->rows[buffer->row_index].size && buffer->row_index < buffer->row_s) {
			buffer->row_index++; buffer->cur_pos = 0;
		}
	} else {
		row_t* cur = &buffer->rows[buffer->row_index];
		shift_row_right(cur, buffer->cur_pos);
		cur->contents[buffer->cur_pos++] = (char) ch; // probably bad
		move(state.y, buffer->cur_pos);
	}
	// clamp the cursor to line end
	if (buffer->cur_pos > buffer->rows[buffer->row_index].size)
		buffer->cur_pos = buffer->rows[buffer->row_index].size;
	// move cursor
	state.ccol = buffer->cur_pos;
	state.crow = buffer->row_index;
	move(state.crow, state.ccol);
}

/* OUTPUT */

void draw_buffer(buffer_t* buffer) {
	for (size_t i = 0; i <= buffer->row_s; i++) {
		mvprintw(i, 0, "%s", buffer->rows[i].contents);
	}
	move(state.crow, state.ccol);
}

void draw_status_bar(buffer_t* buffer, int* ch) {
	mvprintw(
		state.y - 1, state.x - 20, "%.4zu | %.3zu:%.3zu (%.3zu)",
		*ch, buffer->row_index + 1, buffer->cur_pos + 1, buffer->row_s + 1
	);
	move(state.crow, state.ccol);
}

/* INIT */

void read_file_to_buffer(buffer_t* buffer, char* filename) {
	buffer->filename = filename;
	FILE* file = fopen(filename, "a+");
	if (file == NULL) {
		endwin();
		printf("error: could not open file %s", filename);
		exit(1);
	}
	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	fseek(file, 0, SEEK_SET);

	if (length != 0) {
		char* buf = malloc(sizeof(char) * length);
		fread(buf, sizeof(char) * length, 1, file);
		for (size_t i = 0; i < length - 1; i++) {
			if (buf[i] == '\n') {
				buffer->row_s++;
				continue;
			}
			buffer->rows[buffer->row_s].contents[buffer->rows[buffer->row_s].size++] = buf[i];
		}
	}
	fclose(file);
}

int main(int argc, char* argv[]) {
	// check if given name
	char* filename = NULL;
	if (argc > 1) filename = argv[1];

	// terminal setup
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
	getmaxyx(stdscr, state.y, state.x);
	set_escdelay(ESCDELAY);

	// buffer init
	buffer_t buffer = {0};
	for (size_t i; i < MAX_ROWS; i++) {
		buffer.rows[i].contents = calloc(MAX_STRING_SIZE, sizeof(char));
	}
	if (filename != NULL) read_file_to_buffer(&buffer, filename);
	else buffer.filename = "out.txt";

	// main loop
	int ch = 0;
	while (ch != ctrl('q') && QUIT != 1) {
		clear();
		getmaxyx(stdscr, state.y, state.x);
		refresh();
		draw_status_bar(&buffer, &ch);
		draw_buffer(&buffer);
		ch = getch();
		process_keypress(ch, &buffer);
		getyx(stdscr, state.crow, state.ccol);
	}

	refresh();
	endwin();

	return 0;
}
