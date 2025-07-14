#ifndef GREP_H_
#define GREP_H_

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_LINE 1024

bool includes(const char *line, const char *pattern);

void print_file(FILE *file, const char *pattern);

#endif
