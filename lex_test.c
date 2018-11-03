/* lex_test.c
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
#include <string.h>
#include "test_util.h"
#include "lex.h"

void check_token(const char *input, unsigned int input_size,
		 unsigned int *pos,
		 const char *expected_result, unsigned int expected_pos)
{
	char result[80];

	lex_look_ahead(input, input_size, pos, result, sizeof(result));
	check_unsigned_ints(*pos, expected_pos, "expected pos");
	check_strs(result, expected_result, "token");
}

void test_lex_look_ahead_1()
{
	const char *input = "1+ 2";
	unsigned int len, pos, expected_pos;
	char *expected_result;

	len = strlen(input);

	pos = 0;
	expected_pos = 0;
	expected_result = "1";
	check_token(input, len, &pos, expected_result, expected_pos);

	pos = 1;
	expected_pos = 1;
	expected_result = "+";
	check_token(input, len, &pos, expected_result, expected_pos);

	pos = 2;
	expected_pos = 3;
	expected_result = "2";
	check_token(input, len, &pos, expected_result, expected_pos);
}

void test_lex_look_ahead_2()
{
	const char *input = "231+ 1421 - 7";
	unsigned int len, pos;

	len = strlen(input);

	pos = 0;
	check_token(input, len, &pos, "231", 0);
	pos += 3;
	check_token(input, len, &pos, "+", pos);
	pos += 1;
	check_token(input, len, &pos, "1421", pos + 1);
	pos += 4;
	check_token(input, len, &pos, "-", pos + 1);
	pos += 1;
	check_token(input, len, &pos, "7", pos + 1);
}

void test_lex_look_ahead_3()
{
	const char *input = "int a=2; int b=17;";
	unsigned int len, pos;

	len = strlen(input);

	pos = 0;
	check_token(input, len, &pos, "int", 0);
	pos += 3;
	check_token(input, len, &pos, "a", pos + 1);
	pos += 1;
	check_token(input, len, &pos, "=", pos);
	pos += 1;
	check_token(input, len, &pos, "2", pos);
	pos += 1;
	check_token(input, len, &pos, ";", pos);
	pos += 1;
	check_token(input, len, &pos, "int", pos + 1);
	pos += 3;
	check_token(input, len, &pos, "b", pos + 1);
	pos += 1;
	check_token(input, len, &pos, "=", pos);
	pos += 1;
	check_token(input, len, &pos, "17", pos);
}

void test_lex_look_ahead_n_1()
{
	const char *input = "foo= 2;int b;";
	unsigned int len, pos;
	char result[80];

	len = strlen(input);

	pos = 0;
	lex_look_ahead_n(input, len, &pos, 2, result, sizeof(result));
	check_strs(result, "=", "test_lex_look_ahead_n_1");
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}

	test_lex_look_ahead_1();
	test_lex_look_ahead_2();
	test_lex_look_ahead_3();
	test_lex_look_ahead_n_1();
	return 0;
}
