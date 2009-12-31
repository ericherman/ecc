#
CC=gcc
CFLAGS=-c -Werror -ggdb -O0 -Wall -Wextra -pedantic -Wno-long-long
LDFLAGS=

SOURCES=elf_header.c c_lib.c misc.c x86_asm.c lex.c ecc_lib.c
EXEC_SOURCES=$(SOURCES) ecc.c
TEST_SOURCES=$(SOURCES) ecc_test.c

EXEC_OBJECTS=$(EXEC_SOURCES:.c=.o)
TEST_OBJECTS=$(TEST_SOURCES:.c=.o)


EXECUTABLE=ecc
TEST_EXECUTABLE=ecc_test

all: $(EXEC_SOURCES) $(EXECUTABLE) $(TEST_SOURCES) $(TEST_EXECUTABLE)

$(EXECUTABLE): $(EXEC_OBJECTS)
	$(CC) $(LDFLAGS) $(EXEC_OBJECTS) -o $@

$(TEST_EXECUTABLE): $(TEST_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

check:
	./ecc_test

test:
	./functional-test.sh

clean:
	rm -rf *o $(EXECUTABLE) $(TEST_EXECUTABLE) functional-test-*
