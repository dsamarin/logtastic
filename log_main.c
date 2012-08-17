#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log_format.h"

/* 
68.104.1.129 - - [16/Aug/2012:12:30:02 -0700] "GET /testing/finder/FileModel.php?action=read&path=%2Ffiles%2Fsource%2Fcoreutils-7.4%2Fsrc&start=128 HTTP/1.1" 200 99 "http://eboyjr.oftn.org:8080/files/source/coreutils-7.4/src/?pp=tail.c" "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.1 (KHTML, like Gecko) Chrome/21.0.1180.77 Safari/537.1"
*/

#define BUFFER_SIZE (4096)

void parse_log_item (char *buffer) {
	struct format_token *token = NULL;

	char *format = buffer;

	while ((token = parse_format_token (&format))) {
		switch (token->type) {
		case T_CHARACTER:
			putchar (token->data.ch);
			break;
		case T_VARIABLE:
			printf ("${%d}", token->data.var);
			break;
		default:
			printf ("{unreachable}");
		}
	}
	putchar ('\n');

	free (buffer);
}

void readlines (FILE *input, void (*emit)(char *)) {
	int ch;

	char *buffer = NULL;
	size_t cursor = 0;
	size_t length = 0;

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
			(*emit) (buffer);
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
}

int main(void) {
	readlines (stdin, &parse_log_item);
	return 0;
}