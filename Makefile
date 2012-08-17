CC = gcc
CFLAGS = -Wall -Werror -std=c99 -pedantic -Os
LDFLAGS =

SOURCES = log_main.c log_format.c
OBJECTS = $(SOURCES:.c=.o)
EXE = logtastic

CFLAGS += `pkg-config --cflags glib-2.0`
LDFLAGS += `pkg-config --libs glib-2.0`

all: $(EXE)

$(EXE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $(EXE)
	rm -rf $(OBJECTS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXE)
