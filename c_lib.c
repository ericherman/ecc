/* c_lib.c
   Copyright (C) 2009, 2010, 2011, 2018 Eric Herman <eric@freesa.org>

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
#include "c_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void *heap_malloc(unsigned int size)
{
	return malloc(size);
}

void heap_free(void *ptr)
{
	free(ptr);
}

void err_msg(const char *msg)
{
	fprintf(stderr, "%s", msg);
}

void die(void)
{
	int status;

	status = 1;
	exit(status);
}

void read_file(const char *in_file,
	       char *file_buf, unsigned int buf_size, unsigned int *chars_read)
{
	FILE *file;

	file = fopen(in_file, "r");
	if (file == NULL) {
		fprintf(stderr, "Error opening file %s", in_file);
		exit(1);
	}

	fgets(file_buf, buf_size, file);
	*chars_read = strlen(file_buf);
	fclose(file);
}

void write_file(const char *out_file,
		unsigned char *byte_buffer, unsigned int buf_size)
{

	FILE *file;

	file = fopen(out_file, "w");
	fwrite(byte_buffer, 1, buf_size, file);
	fclose(file);

	chmod(out_file, S_IRWXU | S_IRWXG | S_IRWXO);
}
