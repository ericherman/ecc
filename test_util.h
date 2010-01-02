/* test_util.h */
#ifndef _TEST_UTIL_H_
#define _TEST_UTIL_H_

void check_chars(char actual, char expected, const char * msg);
void check_char(char actual, char expected);

void check_unsigned_ints(unsigned int actual, unsigned int expected,
		const char * msg);
void check_unsigned_int(unsigned int actual, unsigned int expected);

void check_ints(int actual, int expected, const char * msg);
void check_int(int actual, int expected);

void check_strs(const char * actual, const char * expected, const char * msg);
void check_str(const char * actual, const char * expected);

void compare_byte_arrays(const char * msg,
		unsigned char * expected, unsigned int expected_len,
		unsigned char * actual, unsigned int actual_len);

#endif /* _TEST_UTIL_H_ */

