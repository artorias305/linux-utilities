#include "cat.h"

bool show_ends = false;
bool number_nonblank = false;
bool number_lines = false;
bool show_tabs = false;		   // implement
bool show_nonprinting = false; // implement

void cat_file(FILE *file, const char *filename) {
	if (file == NULL) {
		fprintf(stderr, "Error opening file '%s'.\n", filename);
		return;
	}
	int column = 1;
	int line = 1;
	bool is_nonblank = false;
	char ch;
	while ((ch = fgetc(file)) != EOF) {
		if (ch != '\n') {
			is_nonblank = true;
		}
		if (column == 1) {
			if (number_nonblank && is_nonblank) {
				printf("%6d\t", line);
			} else if (number_lines && !number_nonblank) {
				printf("%6d\t", line);
			}
		}
		if (ch == '\n' && show_ends) {
			printf("$");
		}
		if (ch == '\t' && show_tabs) {
			printf("^I");
		} else {
			printf("%c", ch);
		}
		if (ch == '\n') {
			column = 1;
			line++;
			is_nonblank = false;
		} else {
			column++;
		}
	}
	fclose(file);
}

bool is_flag(const char *argv) {
	if (argv[0] == '-' && argv[1] != '\0') {
		return true;
	}
	return false;
}

void parse_flag(const char *arg) {
	for (size_t i = 1; i < strlen(arg); i++) {
		if (arg[i] == 'E') {
			show_ends = true;
		}
		if (arg[i] == 'b') {
			number_nonblank = true;
		}
		if (arg[i] == 'n') {
			number_lines = true;
		}
		if (arg[i] == 'T') {
			show_tabs = true;
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc == 1) {
		printf("Usage: cat [-E] [FILE]...\n");
		exit(EXIT_SUCCESS);
	}
	for (int i = 0; i < argc; i++) {
		if (is_flag(argv[i])) {
			parse_flag(argv[i]);
		}
	}
	for (int i = 1; i < argc; i++) {
		if (!is_flag(argv[i])) {
			FILE *file = fopen(argv[i], "r");
			cat_file(file, argv[i]);
		}
	}
	return 0;
}
