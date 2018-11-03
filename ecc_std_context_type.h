/* ecc_std_context_type.h
   Copyright (C) 2016, 2018 Eric Herman <eric@freesa.org>

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
/* ecc_std_context.h */
#ifndef ECC_STD_CONTEXT_TYPE_H
#define ECC_STD_CONTEXT_TYPE_H

#define FILE_MAX 1024
#define TOKEN_MAX 100
#define BBUF_MAX 0xFFFF

#include "names_stack.h"

typedef struct ecc_std_context_s {
	/* need to investigate memory mapped files */
	const char *source_file;
	char buf[FILE_MAX];
	unsigned int buf_size;
	unsigned int buf_pos;

	char next_token[TOKEN_MAX];

	names_stack *names_stack;

	const char *out_file;
	unsigned char byte_buf[BBUF_MAX];
	unsigned int bytes_written;
} ecc_std_context;

#endif /* ECC_STD_CONTEXT_TYPE_H */
