/* ecc.c
   Copyright (C) 2009, 2010, 2011, 2016, 2018 Eric Herman <eric@freesa.org>

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
#include "ecc_std_context.h"
#include "compiler.h"

int main(int argc, char *argv[])
{
	ecc_context *ctx;

	if (argc < 3) {
		err_msg("usage: ");
		err_msg(argv[0]);
		err_msg(" sourcefile executable\n");
		return 1;
	}

	ctx = alloc_std_context(argv[1], argv[2]);

	compile(ctx);

	free_std_context(ctx);

	return 0;
}
