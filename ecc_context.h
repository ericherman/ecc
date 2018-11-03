/* ecc_context.h
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
/* ecc_context.h */
#ifndef ECC_CONTEXT_H
#define ECC_CONTEXT_H

typedef struct ecc_context_s {

	void *data;

	const char *(*lex_look_ahead) (struct ecc_context_s *ctx);
	void (*lex_advance) (struct ecc_context_s *ctx, unsigned int chars);

	void (*stack_enter) (struct ecc_context_s *ctx);
	void (*stack_assign_name) (struct ecc_context_s *ctx, const char *name);
	unsigned int (*stack_name_pos) (struct ecc_context_s *ctx,
					const char *name);
	unsigned int (*stack_frame_size) (struct ecc_context_s *ctx);
	void (*stack_leave) (struct ecc_context_s *ctx);

	void (*output_term) (struct ecc_context_s *ctx, int number);
	void (*output_add) (struct ecc_context_s *ctx);
	void (*output_subtract) (struct ecc_context_s *ctx);
	void (*output_multiply) (struct ecc_context_s *ctx);
	void (*output_divide) (struct ecc_context_s *ctx);
	void (*output_stack_enter) (struct ecc_context_s *ctx);
	void (*output_stack_allocate) (struct ecc_context_s *ctx,
				       unsigned int bytes);
	void (*output_stack_assign_int) (struct ecc_context_s *ctx,
					 unsigned int depth, int number);
	void (*output_stack_leave) (struct ecc_context_s *ctx);
	void (*output_statements_complete) (struct ecc_context_s *ctx);

	void (*read_file) (struct ecc_context_s *ctx);
	void (*write_file) (struct ecc_context_s *ctx);

	void (*output_header) (struct ecc_context_s *ctx);
	void (*output_os_return) (struct ecc_context_s *ctx);

	void (*to_string) (struct ecc_context_s *ctx, char *buf,
			   unsigned int buf_size);

} ecc_context;

#endif /* ECC_CONTEXT_H */
