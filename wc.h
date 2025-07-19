#ifndef WC_H
#define WC_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int byte_count(FILE *file);
int word_count(FILE *file);
int new_line_count(FILE *file);

#endif
