/* ecc_std_context.h
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
#ifndef ECC_STD_CONTEXT_H
#define ECC_STD_CONTEXT_H

#include "ecc_context.h"

ecc_context *alloc_std_context(const char *source_file, const char *out_file);

void free_std_context(ecc_context *ptr);

#endif /* ECC_STD_CONTEXT_H */
