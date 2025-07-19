#include "wc.h"

/* wc - print newline, word and byte counts for each file */

int byte_count(FILE *file) {
	char byte;
	int count = 0;
	while ((byte = fgetc(file)) != EOF) {
		count++;
	}
	rewind(file);
	return count;
}

int word_count(FILE *file) {
	char line[1024];
	int count = 0;
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
	char ch;
	int count = 0;
	while ((ch = fgetc(file)) != EOF) {
		if (ch == '\n')
			count++;
	}
	rewind(file);
	return count;
}

int main(int argc, char *argv[]) {
	int totalNewLine = 0;
	int totalWord = 0;
	int totalByte = 0;
	for (int i = 1; i < argc; i++) {
		char *filename = argv[i];
		FILE *file = fopen(filename, "r");
		if (!file) {
			fprintf(stderr, "Error reading file: '%s'\n", filename);
			return 1;
		}
		int newline_count = new_line_count(file);
		int wordCount = word_count(file);
		int byteCount = byte_count(file);
		totalNewLine += newline_count;
		totalWord += wordCount;
		totalByte += byteCount;
		printf("%d %d %d %s\n", newline_count, wordCount, byteCount, filename);
	}
	printf("%d %d %d total\n", totalNewLine, totalWord, totalByte);
	return 0;
}
