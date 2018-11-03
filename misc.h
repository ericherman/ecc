/* misc.h
   Copyright (C) 2009, 2010, 2011, 2012, 2015, 2018 Eric Herman <eric@freesa.org>

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
/* misc.h */
#ifndef MISC_H
#define MISC_H

int is_whitespace(char c);
int is_number(char c);
int is_alpha(char c);

void str_cpy_offset(const char *src, char *buf, unsigned int buf_size,
		    unsigned int *buf_pos);

char *str_ncpy(char *dest, const char *src, unsigned int limit);

unsigned int str_nlen(const char *s, unsigned int limit);

int str_ncmp(const char *left, unsigned int left_size,
	     const char *right, unsigned int right_size);

int a_to_i(const char *str, unsigned int max_len);

void write_int(unsigned char *buf, int value);
void write_bytes(const char *name,
		 unsigned char *buf, unsigned int buf_size,
		 unsigned int *bytes_written,
		 const unsigned char *to_write, unsigned int count);

#endif /* MISC_H */
