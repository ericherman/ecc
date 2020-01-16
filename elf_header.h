/* elf_header.h
   Copyright (C) 2009, 2010, 2011, 2015, 2018 Eric Herman <eric@freesa.org>

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
/* elf_header.h */
#ifndef ELF_HEADER_H
#define ELF_HEADER_H

void output_header(unsigned char *buf, unsigned int buf_size,
		   unsigned int *bytes_written);

void output_os_return(unsigned char *buf, unsigned int buf_size,
		      unsigned int *bytes_written);

unsigned char *raw_elf_header(void);
unsigned int raw_elf_header_size(void);

unsigned char *raw_linux_return(void);
unsigned int raw_linux_return_size(void);

#endif /* ELF_HEADER_H */
