/* c_lib.h
   Copyright (C) 2009, 2010, 2011, 2015, 2018 Eric Herman <eric@freesa.org>

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
/* c_lib.h */
#ifndef C_LIB_H
#define C_LIB_H

/*
 * This is stuff needed from libc.
 * since a compiler is needed to build libc,
 * we may need to implement these ourselves for the esssential calls
 * or implement them as no-ops for the non-essential calls.
 *
 * This serves as a place to centralize thinking about these.
 */

void *heap_malloc(unsigned int size);
void heap_free(void *ptr);

void err_msg(const char *msg);
void die(void);

void read_file(const char *in_file,
	       char *file_buf, unsigned int buf_size, unsigned int *chars_read);

void write_file(const char *out_file,
		unsigned char *byte_buffer, unsigned int buf_size);

#endif /* C_LIB_H */
