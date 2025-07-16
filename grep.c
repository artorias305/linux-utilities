#include "grep.h"
#include <regex.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		fprintf(stderr, "Usage: %s <pattern> <filename>\n", argv[0]);
		return 1;
	}

	const char *pattern = argv[1];
	const char *filename = argv[2];

	regex_t regex;
	int value = regcomp(&regex, pattern, REG_EXTENDED);
	if (value != 0) {
		char error_msg[256];
		regerror(value, &regex, error_msg, sizeof(error_msg));
		fprintf(stderr, "Regex compilation failed: %s\n", error_msg);
		return 1;
	}

	FILE *file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "Failed to open file: '%s'\n", filename);
		regfree(&regex);
		return 1;
	}

	char line[1024];
	while (fgets(line, sizeof(line), file)) {
		if (regexec(&regex, line, 0, NULL, 0) == 0) {
			printf("%s", line);
		}
	}

	fclose(file);
	regfree(&regex);
	return 0;
}
