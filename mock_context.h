/* mock_context.h
   Copyright (C) 2010, 2011, 2012, 2015, 2016, 2018 Eric Herman <eric@freesa.org>

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
/* mock_context.h */
#ifndef MOCK_CONTEXT_H
#define MOCK_CONTEXT_H

#include "ecc_context.h"

typedef struct mock_data_s {
	const char *call[100];
	unsigned int calls;
	int track_lookahead;

	char no_token[1];

	int term[100];
	unsigned int terms;

	const char **token;
	unsigned int tokens;
	unsigned int token_pos;

	unsigned int mock_return_uint_vals[100];
	unsigned int mock_return_uint_val_c;

} mock_data;

ecc_context *init_fake_context(const char **token, unsigned int tokens);
void free_fake_context(ecc_context *ctx);

#endif /* MOCK_CONTEXT_H */
