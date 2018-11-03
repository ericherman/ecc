/* x86_machine_code.c
   Copyright (C) 2011, 2012, 2018 Eric Herman <eric@freesa.org>

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
#include "x86_machine_code.h"
#include "c_lib.h"
#include "x86_machine_code_bytes.h"
#include "misc.h"

void output_term(int number, unsigned char *buf, unsigned int buf_size,
		 unsigned int *bytes_written)
{
	unsigned char bytes[5];

	bytes[0] = get_pushl_op();
	write_int(&bytes[1], number);

	write_bytes("term", buf, buf_size, bytes_written, bytes, 5);
}

void output_statements_complete(unsigned char *buf, unsigned int buf_size,
				unsigned int *bytes_written)
{
	write_bytes("return val", buf, buf_size, bytes_written,
		    get_return_ops(), get_return_ops_len());
}

void output_add(unsigned char *buf, unsigned int buf_size,
		unsigned int *bytes_written)
{
	write_bytes("add", buf, buf_size, bytes_written,
		    get_addl_ops(), get_addl_ops_len());
}

void output_subtract(unsigned char *buf, unsigned int buf_size,
		     unsigned int *bytes_written)
{
	write_bytes("subtract", buf, buf_size, bytes_written,
		    get_subl_ops(), get_subl_ops_len());
}

void output_multiply(unsigned char *buf, unsigned int buf_size,
		     unsigned int *bytes_written)
{
	write_bytes("multiply", buf, buf_size, bytes_written,
		    get_imull_ops(), get_imull_ops_len());
}

void output_divide(unsigned char *buf, unsigned int buf_size,
		   unsigned int *bytes_written)
{
	write_bytes("divide", buf, buf_size, bytes_written,
		    get_idiv_ops(), get_idiv_ops_len());
}

void output_stack_enter(unsigned char *buf, unsigned int buf_size,
			unsigned int *bytes_written)
{
	write_bytes("stack enter", buf, buf_size, bytes_written,
		    get_stack_enter_ops(), get_stack_enter_ops_len());
}

void output_stack_allocate(unsigned int bytes_to_allocate,
			   unsigned char *buf, unsigned int buf_size,
			   unsigned int *bytes_written)
{
	unsigned char bytes[5];

	bytes[0] = 0x85;
	/* 81 /5 id SUB r/m32,imm32 Subtract imm32 from r/m32 */
	write_int(&bytes[1], bytes_to_allocate);

	write_bytes("stack allocate", buf, buf_size, bytes_written, bytes, 5);
}

void output_stack_assign_int(unsigned int depth, int number,
			     unsigned char *buf, unsigned int buf_size,
			     unsigned int *bytes_written)
{
	unsigned char bytes[9];

	bytes[0] = get_movl_ebp_op();

	write_int(&bytes[1], depth);
	write_int(&bytes[5], number);

	write_bytes("stack assign int", buf, buf_size, bytes_written, bytes, 9);
}

void output_stack_leave(unsigned char *buf, unsigned int buf_size,
			unsigned int *bytes_written)
{
	write_bytes("stack leave", buf, buf_size, bytes_written,
		    get_stack_leave_ops(), get_stack_leave_ops_len());
}
