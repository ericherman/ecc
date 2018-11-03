/* sub_system_test.c
   Copyright (C) 2010, 2011, 2016, 2018 Eric Herman <eric@freesa.org>

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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "compiler.h"
#include "compiler_inner.h"
#include "ecc_context.h"
#include "ecc_std_context.h"
#include "ecc_std_context_type.h"

void test_compile_inner()
{
	unsigned char full_monty[] = {
		/* ELF HEADER */
		/* MAGIC: */
		/* 127,E,L,F,1,1,1,0 */
		/*   0,0,0,0,0,0,0,0 */
		0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* ? */
		0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x54, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x28, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x08,
		0x00, 0x80, 0x04, 0x08,

		/* the 0x66 is the program size */
		0x66, 0x00, 0x00, 0x00,
		/* the 0x66 is the memory size */
		0x66, 0x00, 0x00, 0x00,

		/* ? */
		0x05, 0x00, 0x00, 0x00,
		0x00, 0x10, 0x00, 0x00,

		/* our program */

		/* stack enter */
		0x55,		/* pushl %ebp */
		0x89, 0xe5,	/* movl %esp, %ebp */

		0x68,		/* push immediate value */
		0x17, 0x00, 0x00, 0x00,	/* 23 */

		/* statements complete */
		0x5b,		/* pop ebx */
		/* stack leave */
		0x5d,		/* popl %ebp */

		/* program footer */
		0xb8,		/* movl %eax */
		0x01, 0x00, 0x00, 0x00,	/* 1 == SYS_EXIT */
		0xcd,		/* int */
		0x80		/* Linux Kernel */
	};
	ecc_context *ctx = alloc_std_context("foo", "bar");
	ecc_std_context *data = (ecc_std_context *) ctx->data;
	data->buf[0] = '2';
	data->buf[1] = '3';
	data->buf[2] = '\0';
	data->buf_size = 3;

	compile_inner(ctx);

	check_byte_arrays(data->byte_buf, data->bytes_written,
			  full_monty, sizeof(full_monty), "full monty");

	free_std_context(ctx);
}

int main(int argc, char *argv[])
{
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_compile_inner();

	return 0;
}
