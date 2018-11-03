/* test_util.h
   Copyright (C) 2010, 2011, 2015, 2018 Eric Herman <eric@freesa.org>

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
/* test_util.h */
#ifndef TEST_UTIL_H
#define TEST_UTIL_H

void check_chars(char actual, char expected, const char *msg);
void check_char(char actual, char expected);

void check_ints(int actual, int expected, const char *msg);
void check_int(int actual, int expected);

void check_strs(const char *actual, const char *expected, const char *msg);
void check_str(const char *actual, const char *expected);

void check_ptrs(const void *actual, const void *expected, const char *msg);
void check_ptr(const void *actual, const void *expected);

void check_unsigned_ints(unsigned int actual, unsigned int expected,
			 const char *msg);
void check_unsigned_int(unsigned int actual, unsigned int expected);

void check_byte_arrays(unsigned char *actual, unsigned int actual_len,
		       unsigned char *expected, unsigned int expected_len,
		       const char *msg);
void check_byte_array(unsigned char *actual, unsigned int actual_len,
		      unsigned char *expected, unsigned int expected_len);

#endif /* TEST_UTIL_H */
