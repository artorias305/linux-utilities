#ifndef WC_H
#define WC_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int byte_count(FILE *file);
int char_count(FILE *file);
int word_count(FILE *file);
int new_line_count(FILE *file);
bool is_flag(const char *arg);
void parse_flag(const char *arg);
void print_counts(int lines, int words, int bytes, int chars,
				  const char *filename, bool multiple_files);

#endif
