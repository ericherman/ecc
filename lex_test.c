#include <stdio.h>
#include "test_util.h"
#include "lex.h"

void test_lex_look_ahead() {
	const char * input = "1+ 2";
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

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}

	test_lex_look_ahead();
	return 0;
}