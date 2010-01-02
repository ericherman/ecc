#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "elf_header.h"
#include "x86_asm.h"

unsigned char addl_ops[] = {
	0x5a, /* popl %edx */
	0x58, /* popl %eax */
	0x01, 0xd0, /* addl %edx, %eax */
	0x50, /* pushl %eax */
};

unsigned char subl_ops[] = {
	0x5a, /* popl %edx */
	0x58, /* popl %eax */
	0x29, 0xd0, /* subl %edx, %eax */
	0x50, /* pushl %eax */
};

unsigned char imull_ops[] = {
	0x5a, /* popl %edx */
	0x58, /* popl %eax */
	0x0f, 0xaf, 0xc2, /* imull %edx, %eax */
	0x50, /* pushl %eax */
};

unsigned char push_17[] = {
	0x68, /* push immediate value */
	0x11, 0x00, 0x00, 0x00, /* 17 */
};

unsigned char push_23[] = {
	0x68, /* push immediate value */
	0x17, 0x00, 0x00, 0x00, /* 23 */
};

unsigned char idiv_ops[] = {
	0xba, 0x00, 0x00, 0x00, 0x00, /* movl $0, %edx */
	0x5b, /* popl %ebx */
	0x58, /* popl %eax */
	0xf7, 0xfb, /* idiv %ebx */
	0x50, /* pushl %eax */
};

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

void test_output_statements_complete() {
	unsigned char pop_ebx[] = { 0x5b };
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_statements_complete(buffer, 128, &bytes_written);
	compare_byte_arrays("statements_complete", pop_ebx, sizeof(pop_ebx),
			buffer, bytes_written);
}

void test_output_footer() {
	unsigned char * expected;
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

	compare_byte_arrays("output_add", addl_ops, sizeof(addl_ops),
			buffer, bytes_written);
}

void test_output_subtract() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_subtract(buffer, 128, &bytes_written);

	compare_byte_arrays("output_subtract", subl_ops, sizeof(subl_ops),
			buffer, bytes_written);
}

void test_output_multiply() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_multiply(buffer, 128, &bytes_written);

	compare_byte_arrays("output_multiply", imull_ops, sizeof(imull_ops),
			buffer, bytes_written);
}

void test_output_divide() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_divide(buffer, 128, &bytes_written);

	compare_byte_arrays("output_divide", idiv_ops, sizeof(idiv_ops),
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
