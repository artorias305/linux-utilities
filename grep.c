#include "grep.h"

bool includes(const char *line, const char *pattern) {
	return strstr(line, pattern) != NULL;
}

void print_file(FILE *file, const char *pattern) {
	char ch;
	char line[MAX_LINE];
	while (fgets(line, sizeof(line), file)) {
		if (includes(line, pattern)) {
			char *start = line;
			char *pos = strstr(start, pattern);

			while (pos != NULL) {
				*pos = '\0';
				printf("%s", start);

				printf("\033[31m%s\033[0m", pattern);

				start = pos + strlen(pattern);
				pos = strstr(start, pattern);
			}

			printf("%s", start);
		}
	}
}

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: grep 'string_pattern' path/to/file");
		return 1;
	}
	FILE *file = fopen(argv[2], "r");
	if (file == NULL) {
		fprintf(stderr, "Error opening file '%s'.", argv[1]);
		return 1;
	}
	char *pattern = argv[1];
	print_file(file, pattern);
	fclose(file);
	return 0;
}
