#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define MAX_LINE 1024
#define MAX_LINES 10000

char getch(void) {
	struct termios oldt, newt;
	char ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	newt.c_cc[VMIN] = 1;
	newt.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	ch = getchar();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}

char **read_file(FILE *file, size_t *line_count) {
	char **lines = malloc(MAX_LINES * sizeof(char *));
	if (!lines) {
		fprintf(stderr, "Error allocating memory for lines.\n");
		exit(1);
	}
	char buffer[MAX_LINE];
	size_t count = 0;
	while (count < MAX_LINES && fgets(buffer, MAX_LINE, file)) {
		buffer[strcspn(buffer, "\n")] = '\0';
		lines[count] = strdup(buffer);
		if (!lines[count]) {
			fprintf(stderr, "Memory allocation failed.\n");
			exit(1);
		}
		count++;
	}
	*line_count = count;
	return lines;
}

void free_lines(char **lines, size_t line_count) {
	for (size_t i = 0; i < line_count; i++) {
		free(lines[i]);
	}
	free(lines);
}

void render_lines(char **lines, size_t line_count, size_t current_line) {
	system("clear");
	for (size_t i = 0; i <= current_line && i < line_count; i++) {
		printf("%s\n", lines[i]);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: '%s' path/to/file\n", argv[0]);
		return 1;
	}
	FILE *file = fopen(argv[1], "r");
	if (file == NULL) {
		fprintf(stderr, "Error reading file '%s'.\n", argv[1]);
		return 1;
	}
	size_t line_count;
	char **lines = read_file(file, &line_count);
	fclose(file);

	if (line_count == 0) {
		fprintf(stderr, "Empty file '%s'.\n", argv[1]);
		free_lines(lines, line_count);
	}
	size_t current_line = 0;
	bool running = true;
	render_lines(lines, line_count, current_line);
	while (running) {
		char move = getch();
		switch (move) {
		case 'j':
			if (current_line < line_count - 1) {
				current_line++;
				render_lines(lines, line_count, current_line);
			}
			break;
		case 'k':
			if (current_line > 0) {
				current_line--;
				render_lines(lines, line_count, current_line);
			}
			break;
		case 'G':
			current_line = line_count - 1;
			render_lines(lines, line_count, current_line);
			break;
		case 'g':
			current_line = 0;
			render_lines(lines, line_count, current_line);
			break;
		case 'q':
			running = false;
			break;
		}
	}
	free_lines(lines, line_count);
	system("clear");
	return 0;
}
