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

int main(int argc, char *argv[])
{
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}

	test_str_nlen();
	test_str_ncpy();
	return 0;
}
