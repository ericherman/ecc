#include <stdio.h>
#include "test_util.h"
#include "lex.h"

void test_lex_look_ahead() {
	const char *input = "1+ 2";
	unsigned int pos, expected_pos;
	char result, expected_result;

	pos = 0;
	result = lex_look_ahead(input, sizeof(input), &pos);
	expected_pos = 0;
	expected_result = '1';
	check_unsigned_int(pos, expected_pos);
	check_char(result, expected_result);

	pos = 1;
	result = lex_look_ahead(input, sizeof(input), &pos);
	expected_pos = 1;
	expected_result = '+';
	check_unsigned_int(pos, expected_pos);
	check_char(result, expected_result);

	pos = 2;
	result = lex_look_ahead(input, sizeof(input), &pos);
	expected_result = '2';
	expected_pos = 3;
	check_unsigned_int(pos, expected_pos);
	check_char(result, expected_result);
}

void test_lex_get_number() {
	const char *input = "7+ 4211 * -2";
	unsigned int pos, expected_pos;
	int result, expected_result;

	pos = 0;
	result = lex_get_number(&input[pos], sizeof(input), &pos);
	expected_pos = 1;
	expected_result = 7;
	check_unsigned_int(pos, expected_pos);
	check_int(result, expected_result);

	pos = 3;
	result = lex_get_number(&input[pos], sizeof(input), &pos);
	expected_pos = 7;
	expected_result = 4211;
	check_unsigned_int(pos, expected_pos);
	check_int(result, expected_result);

	pos = 10;
	result = lex_get_number(&input[pos], sizeof(input), &pos);
	expected_result = -2;
	expected_pos = 12;
	check_unsigned_int(pos, expected_pos);
	check_int(result, expected_result);
}

void test_lex_get_name_length() {
	const char *input = "foo_5 fx_4-2";
	unsigned int result, expected_result;

	result = lex_get_name_length(input, sizeof(input));
	expected_result = 5;
	check_unsigned_int(result, expected_result);

	input += 6;
	result = lex_get_name_length(input, sizeof(input));
	expected_result = 4;
	check_unsigned_int(result, expected_result);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}

	test_lex_look_ahead();
	test_lex_get_number();
	test_lex_get_name_length();
	return 0;
}
