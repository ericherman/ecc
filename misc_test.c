/* misc_test.c
   Copyright (C) 2010, 2011, 2012, 2018 Eric Herman <eric@freesa.org>

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
#include "test_util.h"
#include "misc.h"
#include <string.h>

void test_str_nlen()
{
	const char *input = "foo";
	const char *input2 = "fo\0bar";
	const char *input3 = "";
	unsigned int result, expected_result;

	result = str_nlen(input, sizeof(input));
	expected_result = strlen(input);
	check_unsigned_ints(result, expected_result, "compare with libc");
	check_unsigned_int(result, 3);

	result = str_nlen(input2, sizeof(input2));
	expected_result = strlen(input2);
	check_unsigned_int(result, expected_result);

	result = str_nlen(input3, sizeof(input3));
	expected_result = strlen(input3);
	check_unsigned_int(result, expected_result);

	result = str_nlen(input, 2);
	check_unsigned_ints(result, 2, "limit to 2 not 3");
}

void test_str_ncpy()
{
	const char *src = "foo";
	char dest[80];
	char *result, *expected_result;

	dest[60] = 'X';
	check_unsigned_int(sizeof(dest), 80);
	check_char(dest[60], 'X');

	result = str_ncpy(dest, src, sizeof(dest));
	check_strs(dest, src, "1");
	check_ptrs(result, dest, "2");
	check_chars(dest[60], '\0', "should null-out rest of buffer");

	expected_result = strncpy(dest, src, sizeof(dest));
	check_strs(dest, src, "3");
	check_ptrs(result, expected_result, "4");
}

void test_str_ncmp()
{
	const char *str1 = "foo";
	const char *str2 = "foo2";
	const char *str3 = "fun";
	int result;

	result = str_ncmp(str1, sizeof(str1), str1, sizeof(str1));
	check_int(result, 0);
	result = str_ncmp(str1, sizeof(str1), str2, sizeof(str2));
	check_int(result, 0);
	result = str_ncmp(str1, sizeof(str1), str3, sizeof(str3));
	check_int(result, 0);
}

void test_a_to_i()
{
	int result, expected_result;

	result = a_to_i("7", 2);
	expected_result = 7;
	check_int(result, expected_result);

	result = a_to_i("4211", 5);
	expected_result = 4211;
	check_int(result, expected_result);

	result = a_to_i(" -2 ", 5);
	expected_result = -2;
	check_int(result, expected_result);
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}

	test_str_nlen();
	test_str_ncpy();
	test_a_to_i();

	return 0;
}
