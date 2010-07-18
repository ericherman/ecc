#include <stdio.h>
#include "test_util.h"
#include "misc.h"
#include <string.h>

void test_str_len() {
	const char *input = "foo";
	const char *input2 = "fo\0bar";
	const char *input3 = "";
	unsigned int result, expected_result;

	result = str_len(input);
	expected_result = strlen(input);
	check_unsigned_ints(result, expected_result, "compare with libc");
	check_unsigned_int(result, 3);

	result = str_len(input2);
	expected_result = strlen(input2);
	check_unsigned_int(result, expected_result);

	result = str_len(input3);
	expected_result = strlen(input3);
	check_unsigned_int(result, expected_result);
}

void test_str_ncpy() {
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

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}

	test_str_len();
	test_str_ncpy();
	return 0;
}
