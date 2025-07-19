#include "wc.h"

bool print_bytes = false;
bool print_chars = false;
bool print_lines = false;
bool print_words = false;

int byte_count(FILE *file) {
	int count = 0;
	while (fgetc(file) != EOF) {
		count++;
	}
	rewind(file);
	return count;
}

int char_count(FILE *file) {
	int count = 0;
	int c;
	while ((c = fgetc(file)) != EOF) {
		count++;
	}
	rewind(file);
	return count;
}

int word_count(FILE *file) {
	int count = 0;
	char line[1024];
	while (fgets(line, sizeof(line), file)) {
		char *word = strtok(line, " \t\n");
		while (word != NULL) {
			count++;
			word = strtok(NULL, " \t\n");
		}
	}
	rewind(file);
	return count;
}

int new_line_count(FILE *file) {
	int count = 0;
	int ch;
	while ((ch = fgetc(file)) != EOF) {
		if (ch == '\n') {
			count++;
		}
	}
	rewind(file);
	return count;
}

bool is_flag(const char *arg) { return arg[0] == '-' && arg[1] != '\0'; }

void parse_flag(const char *arg) {
	for (size_t i = 1; i < strlen(arg); i++) {
		switch (arg[i]) {
		case 'c':
			print_bytes = true;
			break;
		case 'm':
			print_chars = true;
			break;
		case 'l':
			print_lines = true;
			break;
		case 'w':
			print_words = true;
			break;
		default:
			fprintf(stderr, "Unknown flag: -%c\n", arg[i]);
			break;
		}
	}
}

void print_counts(int lines, int words, int bytes, int chars,
				  const char *filename, bool multiple_files) {
	bool no_flags =
		!print_lines && !print_words && !print_bytes && !print_chars;

	if (no_flags || print_lines)
		printf("%7d", lines);
	if (no_flags || print_words)
		printf("%7d", words);
	if (no_flags || print_bytes)
		printf("%7d", bytes);
	if (print_chars)
		printf("%7d", chars);
	printf(" %s\n", filename);
}

int main(int argc, char *argv[]) {
	int total_lines = 0, total_words = 0, total_bytes = 0, total_chars = 0;
	bool multiple_files = false;
	int file_count = 0;

	for (int i = 1; i < argc; i++) {
		if (is_flag(argv[i])) {
			parse_flag(argv[i]);
		} else {
			file_count++;
		}
	}
	if (file_count > 1)
		multiple_files = true;

	if (file_count == 0) {
		FILE *file = stdin;
		int lines = new_line_count(file);
		int words = word_count(file);
		int bytes = byte_count(file);
		int chars = char_count(file);
		print_counts(lines, words, bytes, chars, "", false);
		return 0;
	}

	for (int i = 1; i < argc; i++) {
		if (is_flag(argv[i]))
			continue;

		char *filename = argv[i];
		FILE *file = fopen(filename, "r");
		if (!file) {
			fprintf(stderr, "Error reading file: '%s'\n", filename);
			continue;
		}

		int lines = new_line_count(file);
		int words = word_count(file);
		int bytes = byte_count(file);
		int chars = char_count(file);
		fclose(file);

		print_counts(lines, words, bytes, chars, filename, multiple_files);

		total_lines += lines;
		total_words += words;
		total_bytes += bytes;
		total_chars += chars;
	}

	if (multiple_files) {
		print_counts(total_lines, total_words, total_bytes, total_chars,
					 "total", true);
	}

	return 0;
}
