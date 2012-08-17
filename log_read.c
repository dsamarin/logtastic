#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log_read.h"


void log_read_lines (FILE *input, void (*emit)(char *, void *), void *data) {
	int ch;

	char *buffer = NULL;
	size_t cursor = 0;
	size_t length = 0;

#define BUFFER_SIZE (4096)

	setvbuf (input, NULL, _IOLBF, BUFFER_SIZE);

	while ((ch = fgetc (input)) != EOF) {

		if (!buffer) {
			buffer = malloc (BUFFER_SIZE);
			if (!buffer) {
				return;
			}

			cursor = 0;
			length = BUFFER_SIZE;
			memset (buffer, '\0', length);
		}

		if (ch == '\n') {
			(*emit) (buffer, data);
			buffer = NULL;
		} else {
			buffer[cursor++] = ch;

			if (cursor >= length) {
				char *tmp = NULL;
				size_t increase = length;

				length = length + increase;

				tmp = realloc (buffer, length);
				if (!tmp) {
					free (buffer);
					return;
				}

				buffer = tmp;
				memset (buffer + increase, '\0', increase);
			}
		}
	}

#undef BUFFER_SIZE

}

