/* compiler_inner.h
   Copyright (C) 2011, 2012, 2015, 2016, 2018 Eric Herman <eric@freesa.org>

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
/* compiler_inner.h */
#ifndef COMPILER_INNER_H
#define COMPILER_INNER_H

#include "ecc_context.h"

void compile_inner(ecc_context *ctx);

void factor(ecc_context *ctx, const char *token);

void term(ecc_context *ctx, const char *token);

void expression(ecc_context *ctx, const char *token);

void declaration(ecc_context *ctx, const char *token);

void assignment(ecc_context *ctx, const char *token);

#endif /* COMPILER_INNER_H */
