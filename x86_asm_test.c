#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "elf_header.h"
#include "x86_asm.h"
#include "x86_asm_bytes.h"

unsigned char push_17[] = {
	0x68, /* push immediate value */
	0x11, 0x00, 0x00, 0x00, /* 17 */
};

unsigned char push_23[] = {
	0x68, /* push immediate value */
	0x17, 0x00, 0x00, 0x00, /* 23 */
};

void test_output_header() {
	unsigned char *expected;
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

void test_output_statements_complete() {
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_statements_complete(buffer, 128, &bytes_written);
	compare_byte_arrays("statements_complete",
			get_return_ops(), get_return_ops_len(),
			buffer, bytes_written);
}

void test_output_footer() {
	unsigned char *expected;
	unsigned int expected_len;
	unsigned char actual[128];
	unsigned int actual_len = 0;

	expected = raw_linux_return();
	expected_len = raw_linux_return_size();

	output_os_return(actual, 128, &actual_len);

	compare_byte_arrays("SYS_EXIT",
			expected, expected_len, actual, actual_len);
}

void test_output_add() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_add(buffer, 128, &bytes_written);

	compare_byte_arrays("output_add",
			get_addl_ops(), get_addl_ops_len(),
			buffer, bytes_written);
}

void test_output_subtract() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_subtract(buffer, 128, &bytes_written);

	compare_byte_arrays("output_subtract",
			get_subl_ops(), get_subl_ops_len(),
			buffer, bytes_written);
}

void test_output_multiply() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_multiply(buffer, 128, &bytes_written);

	compare_byte_arrays("output_multiply",
			get_imull_ops(), get_imull_ops_len(),
			buffer, bytes_written);
}

void test_output_divide() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_divide(buffer, 128, &bytes_written);

	compare_byte_arrays("output_divide",
			get_idiv_ops(), get_idiv_ops_len(),
			buffer, bytes_written);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_output_header();
	test_output_statements_complete();
	test_output_footer();
	test_output_add();
	test_output_subtract();
	test_output_multiply();
	test_output_divide();

	return 0;
}
