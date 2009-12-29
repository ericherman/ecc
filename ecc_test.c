#include <stdio.h>
#include <stdlib.h>
#include "ecc_lib.h"
#include "elf_header.h"

/* Prototypes */
void compare_byte_arrays(const char * name,
		unsigned char * expected, unsigned int expected_len,
		unsigned char * actual, unsigned int actual_len);

/* Functions */
void test_output_header() {
	unsigned char * expected;
	unsigned expected_len;
	unsigned char actual[128];
	unsigned actual_len;
	unsigned int min;

	expected = raw_elf_header();
	expected_len = raw_elf_header_size();
	min = 45;

	output_header(actual, 128, &actual_len);

	if (actual_len < min) {
		fprintf(stderr, "elf headers are at least %d bytes\n", min);
	}

	compare_byte_arrays("ELF", expected, expected_len, actual, actual_len);
}

void test_output_footer() {
	unsigned char * expected;
	unsigned expected_len;
	unsigned char actual[128];
	unsigned actual_len;

	expected = raw_linux_return();
	expected_len = raw_linux_return_size();

	output_os_return(actual, 128, &actual_len);

	compare_byte_arrays("SYS_EXIT",
			expected, expected_len, actual, actual_len);
}

void compare_byte_arrays(const char * name,
		unsigned char * expected, unsigned int expected_len,
		unsigned char * actual, unsigned int actual_len) {

	unsigned int i;

	if ( actual_len != expected_len ) {
		fprintf(stderr, "length mis-match %d != %d\n",
			actual_len, expected_len);
		goto fail;
	}
	for (i = 0; i < actual_len; i++) {
		if (actual[i] != expected[i]) {
			fprintf(stderr, "buffers differ at %d\n", i);
			goto fail;
		}
	}
	return;

fail:
	fprintf(stderr, "FAIL: %s\n", name);
	for (i = 0; i < actual_len; i++) {
		fprintf(stderr, "actual[%d]=%02x\n", i, actual[i]);
	}
	for (i = 0; i < expected_len; i++) {
		fprintf(stderr, "expected[%d]=%02x\n", i, expected[i]);
	}
	exit(0);
}

void test_compile_simple_statement() {
	unsigned char push_17[] = {
		0x68, /* push immediate value */
		0x11, 0x00, 0x00, 0x00, /* 17 */
	};
	unsigned char pop_ebx[] = { 0x5b };

	char * input = "17";
	unsigned char buffer[128];
	unsigned int chars_read, bytes_written, bytes_written2;

	statements(input, sizeof(input),
			buffer, 128,
			&chars_read, &bytes_written);

	compare_byte_arrays("statements", push_17, sizeof(push_17),
			buffer, bytes_written);

	statements_complete(&buffer[bytes_written], 128-bytes_written, &bytes_written2);
	compare_byte_arrays("statements_complete", pop_ebx, sizeof(pop_ebx),
			&buffer[bytes_written], bytes_written2);
}

void test_compile_inner() {
	unsigned char full_monty[] = {
		/* ELF HEADER ************************************ MAGIC: */
		0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, /* 127,E,L,F,1,1,1,0 */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*   0,0,0,0,0,0,0,0 */
		/* ? */
		0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x54, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x08,
		0x00, 0x80, 0x04, 0x08,
		0xff, 0xff, 0x00, 0x00, /* the 0xFF, 0xFF here is program size? */
		0xff, 0xff, 0x00, 0x00, /* the 0xFF, 0xFF here is memory size? */
		0x05, 0x00, 0x00, 0x00,
		0x00, 0x10, 0x00, 0x00,

		/* our program */
		0x68, /* push immediate value */
		0x17, 0x00, 0x00, 0x00, /* 23 */

		/* statemens complete */
		0x5b, /* pop ebx */

		/* program footer */
		0xb8, /* movl %eax */
		0x01, 0x00, 0x00, 0x00, /* 1 == SYS_EXIT */
		0xcd, /* int */
		0x80  /* Linux Kernel */
	};

	unsigned char byte_buffer[1024];
	unsigned int bytes_written;

	compile_inner("23", 3, byte_buffer, sizeof(byte_buffer), &bytes_written);

	compare_byte_arrays("full monty", full_monty, sizeof(full_monty),
			byte_buffer, bytes_written);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_output_header();
	test_output_footer();
	test_compile_simple_statement();
	test_compile_inner();
	return 0;
}
