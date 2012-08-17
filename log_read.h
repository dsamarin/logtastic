#ifndef LOG_READ_H
#define LOG_READ_H

void log_read_lines (FILE *input, void (*emit)(char *, void *), void *data);

#endif
