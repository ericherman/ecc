/* lex.h
   Copyright (C) 2009, 2010, 2011, 2012, 2015, 2018 Eric Herman <eric@freesa.org>

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
/* lex.h */
#ifndef LEX_H
#define LEX_H

void lex_look_ahead(const char *input, unsigned int input_size,
		    unsigned int *pos, char *output, unsigned int output_size);

void lex_look_ahead_n(const char *input, unsigned int input_size,
		      unsigned int *pos, unsigned int number_ahead,
		      char *output, unsigned int output_size);

#endif /* LEX_H */
