#
CC=gcc
CFLAGS=-c -Werror -Wall -Wextra -pedantic -Wno-long-long \
 -ggdb -O0 -finstrument-functions
LDFLAGS=

SOURCES=elf_header.c c_lib.c misc.c x86_machine_code_bytes.c \
	names_stack.c x86_machine_code.c lex.c std_context.c compiler.c
EXEC_SOURCES=$(SOURCES) ecc.c
TEST_SOURCES=$(SOURCES) test_util.c
TEST_LEX_SOURCES=$(TEST_SOURCES) lex_test.c
TEST_MISC_SOURCES=$(TEST_SOURCES) misc_test.c
TEST_NAMES_STACK_SOURCES=$(TEST_SOURCES) names_stack_test.c
TEST_MACHINE_CODE_SOURCES=$(TEST_SOURCES) x86_machine_code_test.c
TEST_COMPILER_SOURCES=$(TEST_SOURCES) mock_context.c compiler_test.c
TEST_SUBSYSTEM_SOURCES=$(TEST_SOURCES) sub_system_test.c

EXEC_OBJECTS=$(EXEC_SOURCES:.c=.o)
TEST_COMPILER_OBJECTS=$(TEST_COMPILER_SOURCES:.c=.o)
TEST_LEX_OBJECTS=$(TEST_LEX_SOURCES:.c=.o)
TEST_MISC_OBJECTS=$(TEST_MISC_SOURCES:.c=.o)
TEST_NAMES_STACK_OBJECTS=$(TEST_NAMES_STACK_SOURCES:.c=.o)
TEST_MACHINE_CODE_OBJECTS=$(TEST_MACHINE_CODE_SOURCES:.c=.o)
TEST_SUBSYSTEM_OBJECTS=$(TEST_SUBSYSTEM_SOURCES:.c=.o)

EXECUTABLE=ecc
TEST_LEX=lex_test
TEST_MISC=misc_test
TEST_COMPILER=compiler_test
TEST_NAMES_STACK=names_stack_test
TEST_MACHINE_CODE=x86_machine_code_test
TEST_SUBSYSTEM=sub_system_test

# extracted from https://github.com/torvalds/linux/blob/master/scripts/Lindent
LINDENT=indent -npro -kr -i8 -ts8 -sob -l80 -ss -ncs -cp1 -il0

all: $(EXEC_SOURCES) $(EXECUTABLE) $(TEST_SOURCES) \
	$(TEST_MISC) $(TEST_LEX) $(TEST_NAMES_STACK) $(TEST_MACHINE_CODE) \
	$(TEST_COMPILER) $(TEST_NAMES_STACK) $(TEST_SUBSYSTEM)

$(EXECUTABLE): $(EXEC_OBJECTS)
	$(CC) $(LDFLAGS) $(EXEC_OBJECTS) -o $@

$(TEST_LEX): $(TEST_LEX_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_LEX_OBJECTS) -o $@

$(TEST_MISC): $(TEST_MISC_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_MISC_OBJECTS) -o $@

$(TEST_NAMES_STACK): $(TEST_NAMES_STACK_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_NAMES_STACK_OBJECTS) -o $@

$(TEST_MACHINE_CODE): $(TEST_MACHINE_CODE_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_MACHINE_CODE_OBJECTS) -o $@

$(TEST_COMPILER): $(TEST_COMPILER_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_COMPILER_OBJECTS) -o $@

$(TEST_SUBSYSTEM): $(TEST_SUBSYSTEM_OBJECTS)
	$(CC) $(LDFLAGS) $(TEST_SUBSYSTEM_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

check:
	./$(TEST_MISC)
	./$(TEST_LEX)
	./$(TEST_NAMES_STACK)
	./$(TEST_MACHINE_CODE)
	./$(TEST_COMPILER)
	./$(TEST_SUBSYSTEM)

test:
	./functional-test.sh

tidy:
	$(LINDENT) \
		-T FILE \
		-T size_t \
		-T context_t \
		-T mock_data \
		-T names_stack_t \
		*.h *.c

clean:
	rm -rf *o $(EXECUTABLE) $(TEST_MISC) $(TEST_LEX) $(TEST_MACHINE_CODE) \
		$(TEST_NAMES_STACK) $(TEST_COMPILER) $(TEST_SUBSYSTEM) \
		functional-test-* *~
