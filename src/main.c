/*
	quecto - text editor made with ncurses
	inspired by nano, cano, nino and kilo
	(not a joke)
	(c) shroomwastaken 2024
*/

#include "include/main.h"

/* INIT */

// TODO - fix "state"

char QUIT = 0;

int main(int argc, char* argv[]) {
	state_t state;

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
	for (size_t i = 0; i < MAX_ROWS; i++) {
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
