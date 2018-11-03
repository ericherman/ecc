/* x86_machine_code.h
   Copyright (C) 2011, 2012, 2015, 2018 Eric Herman <eric@freesa.org>

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
/* x86_machine_code.h */
#ifndef X86_MACHINE_CODE_H
#define X86_MACHINE_CODE_H

void output_term(int number,
		 unsigned char *buf, unsigned int buf_size,
		 unsigned int *bytes_written);

void output_statements_complete(unsigned char *buf, unsigned int buf_size,
				unsigned int *bytes_written);

void output_add(unsigned char *buf, unsigned int buf_size,
		unsigned int *bytes_written);

void output_subtract(unsigned char *buf, unsigned int buf_size,
		     unsigned int *bytes_written);

void output_multiply(unsigned char *buf, unsigned int buf_size,
		     unsigned int *bytes_written);

void output_divide(unsigned char *buf, unsigned int buf_size,
		   unsigned int *bytes_written);

void output_stack_enter(unsigned char *buf, unsigned int buf_size,
			unsigned int *bytes_written);

void output_stack_allocate(unsigned int bytes,
			   unsigned char *buf, unsigned int buf_size,
			   unsigned int *bytes_written);

void output_stack_assign_int(unsigned int depth, int number,
			     unsigned char *buf, unsigned int buf_size,
			     unsigned int *bytes_written);

void output_stack_leave(unsigned char *buf, unsigned int buf_size,
			unsigned int *bytes_written);

#endif /* X86_MACHINE_CODE_H */
