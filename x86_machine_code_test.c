/* x86_machine_code_test.c
   Copyright (C) 2011, 2018 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

	https://www.gnu.org/licenses/gpl-3.0.txt
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "elf_header.h"
#include "x86_machine_code.h"
#include "x86_machine_code_bytes.h"

unsigned char push_17[] = {
	0x68,			/* push immediate value */
	0x11, 0x00, 0x00, 0x00,	/* 17 */
};

unsigned char push_23[] = {
	0x68,			/* push immediate value */
	0x17, 0x00, 0x00, 0x00,	/* 23 */
};

void test_output_header()
{
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

	check_byte_arrays(actual, actual_len, expected, expected_len, "ELF");
}

void test_output_statements_complete()
{
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_statements_complete(buffer, 128, &bytes_written);
	check_byte_arrays(buffer, bytes_written,
			  get_return_ops(), get_return_ops_len(),
			  "statements_complete");
}

void test_output_footer()
{
	unsigned char *expected;
	unsigned int expected_len;
	unsigned char actual[128];
	unsigned int actual_len = 0;

	expected = raw_linux_return();
	expected_len = raw_linux_return_size();

	output_os_return(actual, 128, &actual_len);

	check_byte_arrays(actual, actual_len, expected, expected_len,
			  "SYS_EXIT");
}

void test_output_add()
{
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_add(buffer, 128, &bytes_written);

	check_byte_array(buffer, bytes_written,
			 get_addl_ops(), get_addl_ops_len());
}

void test_output_subtract()
{
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_subtract(buffer, 128, &bytes_written);

	check_byte_array(buffer, bytes_written,
			 get_subl_ops(), get_subl_ops_len());
}

void test_output_multiply()
{
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_multiply(buffer, 128, &bytes_written);

	check_byte_array(buffer, bytes_written,
			 get_imull_ops(), get_imull_ops_len());
}

void test_output_divide()
{
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_divide(buffer, 128, &bytes_written);

	check_byte_array(buffer, bytes_written,
			 get_idiv_ops(), get_idiv_ops_len());
}

int main(int argc, char *argv[])
{
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
