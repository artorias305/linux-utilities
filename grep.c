#include "grep.h"

#define MAX_LINE 1024

void to_lowercase(char *dst, const char *src) {
		while (*src) {
				*dst++ = tolower(*src++);
		}
		*dst = '\0';
}

bool match_line(const char *line, const char *pattern, bool fixed, bool icase,
				regex_t *regex) {
		if (fixed) {
				char line_cmp[MAX_LINE], pattern_cmp[MAX_LINE];

				if (icase) {
						to_lowercase(line_cmp, line);
						to_lowercase(pattern_cmp, pattern);
						return strstr(line_cmp, pattern_cmp) != NULL;
				} else {
						return strstr(line, pattern) != NULL;
				}
		} else {
				return regexec(regex, line, 0, NULL, 0) == 0;
		}
}

int main(int argc, char *argv[]) {
		if (argc < 3) {
				fprintf(stderr, "Usage: %s [OPTIONS] pattern file\n", argv[0]);
				return 1;
		}

		bool fixed = false;
		bool extended = false;
		bool invert = false;
		bool show_line_num = false;
		bool icase = false;
		char *pattern = NULL;
		char *filename = NULL;

		for (int i = 1; i < argc; i++) {
				if (argv[i][0] == '-') {
						for (size_t j = 1; argv[i][j]; j++) {
								switch (argv[i][j]) {
								case 'F':
										fixed = true;
										break;
								case 'E':
										extended = true;
										break;
								case 'i':
										icase = true;
										break;
								case 'v':
										invert = true;
										break;
								case 'n':
										show_line_num = true;
										break;
								case 'e':
										if (i + 1 >= argc) {
												fprintf(
													stderr,
													"-e requires a pattern\n");
												return 1;
										}
										pattern = argv[++i];
										break;
								default:
										fprintf(stderr, "Unknown option: -%c\n",
												argv[i][j]);
										return 1;
								}
						}
				} else if (!pattern) {
						pattern = argv[i];
				} else if (!filename) {
						filename = argv[i];
				}
		}

		if (!pattern || !filename) {
				fprintf(stderr, "Missing pattern or filename\n");
				return 1;
		}

		FILE *file = fopen(filename, "r");
		if (!file) {
				perror("File open error");
				return 1;
		}

		regex_t regex;
		int cflags = 0;

		if (!fixed) {
				cflags |= icase ? REG_ICASE : 0;
				cflags |= extended ? REG_EXTENDED : 0;
				int ret = regcomp(&regex, pattern, cflags);
				if (ret != 0) {
						char buf[256];
						regerror(ret, &regex, buf, sizeof(buf));
						fprintf(stderr, "Regex error: %s\n", buf);
						return 1;
				}
		}

		char line[MAX_LINE];
		int line_num = 0;

		while (fgets(line, sizeof(line), file)) {
				line_num++;

				line[strcspn(line, "\n")] = '\0';

				bool matched = match_line(line, pattern, fixed, icase, &regex);
				if (invert)
						matched = !matched;

				if (matched) {
						if (show_line_num)
								printf("%d:", line_num);
						printf("%s\n", line);
				}
		}

		if (!fixed)
				regfree(&regex);
		fclose(file);
		return 0;
}
