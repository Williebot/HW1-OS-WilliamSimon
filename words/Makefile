.DEFAULT_GOAL := default

CC?=gcc
CFLAGS?=-Wall
SOURCES=main.c word_count.c
BINARIES=words

# Compile the executable
executable:
	$(CC) $(CFLAGS) $(SOURCES) -o $(BINARIES)

# Clean up
clean:
	rm -f $(BINARIES)

default: executable
