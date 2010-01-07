#
CC=gcc
CFLAGS=-c -Werror -ggdb -O0 -Wall -Wextra -pedantic -Wno-long-long
LDFLAGS=

SOURCES=elf_header.c c_lib.c misc.c x86_asm_bytes.c x86_asm.c lex.c \
	std_context.c compiler.c
EXEC_SOURCES=$(SOURCES) ecc.c
TEST_SOURCES=$(SOURCES) test_util.c
TEST_LEX_SOURCES=$(TEST_SOURCES) lex_test.c
TEST_ASM_SOURCES=$(TEST_SOURCES) x86_asm_test.c
TEST_COMPILER_SOURCES=$(TEST_SOURCES) mock_context.c compiler_test.c
TEST_SUBSYSTEM_SOURCES=$(TEST_SOURCES) sub_system_test.c

EXEC_OBJECTS=$(EXEC_SOURCES:.c=.o)
TEST_SUBSYSTEM_OBJECTS=$(TEST_SUBSYSTEM_SOURCES:.c=.o)
TEST_COMPILER_OBJECTS=$(TEST_COMPILER_SOURCES:.c=.o)
TEST_LEX_OBJECTS=$(TEST_LEX_SOURCES:.c=.o)
TEST_ASM_OBJECTS=$(TEST_ASM_SOURCES:.c=.o)


EXECUTABLE=ecc
TEST_LEX=lex_test
TEST_COMPILER=compiler_test
TEST_ASM=x86_asm_test
TEST_SUBSYSTEM=sub_system_test

all: $(EXEC_SOURCES) $(EXECUTABLE) $(TEST_SOURCES) $(TEST_LEX) $(TEST_ASM) $(TEST_COMPILER) $(TEST_SUBSYSTEM)

$(EXECUTABLE): $(EXEC_OBJECTS)
	$(CC) $(LDFLAGS) $(EXEC_OBJECTS) -o $@

$(TEST_LEX): $(TEST_LEX_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_LEX_OBJECTS) -o $@

$(TEST_ASM): $(TEST_ASM_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_ASM_OBJECTS) -o $@

$(TEST_COMPILER): $(TEST_COMPILER_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_COMPILER_OBJECTS) -o $@

$(TEST_SUBSYSTEM): $(TEST_SUBSYSTEM_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_SUBSYSTEM_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

check:
	./$(TEST_LEX)
	./$(TEST_ASM)
	./$(TEST_COMPILER)
	./$(TEST_SUBSYSTEM)

test:
	./functional-test.sh

clean:
	rm -rf *o $(EXECUTABLE) $(TEST_LEX) $(TEST_ASM) $(TEST_COMPILER) $(TEST_SUBSYSTEM) functional-test-*
