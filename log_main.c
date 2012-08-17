#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "log_read.h"
#include "log_format.h"

static void check_log_message (char *buffer, void *data) {
	struct format_token *format = data;
	_Bool matches = 1;

	char *line = buffer;

	if (buffer && format) {
		while (format && *line) {
			if (format->type == T_VARIABLE) {
				char ending = '\0';
				if (format->next->type != T_CHARACTER) {
					matches = 0;
					fprintf (stderr, "Can't have consecutive variables!\n");
					break;
				}
				ending = format->next->data.ch;

				char *begin = line;
				size_t length = 0;
				while (*line != ending && *line != '\0') {
					length++;
					line++;
				}
				
				char *copy = g_strndup (begin, length);
				printf ("\033[33m%d -> %s\033[0m\n", format->data.var, copy);
				g_free (copy);
			} else {
				if (*line != format->data.ch) {
					matches = 0;
					break;
				}
				line++;
			}
			format = format->next;
		}

		if (format) {
			matches = 0;
		}

		printf ("Line[%s]: %s\n\n",
			matches ? "\033[32m OK \033[39m" : "\033[31mFAIL\033[39m",
			buffer);
	}

	free (buffer);
}

int main(void) {
	struct format_token *item = NULL;

	char *format = "$remote_addr - $remote_user [$time_local] \"$request\" "
		"$status $body_bytes_sent \"$http_referer\" \"$http_user_agent\"";

	item = parse_log_format (format);
	log_read_lines (stdin, check_log_message, (void *) item);

	return 0;
}
