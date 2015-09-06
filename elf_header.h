/* elf_header.h */
#ifndef ELF_HEADER_H
#define ELF_HEADER_H

void output_header(unsigned char *buf, unsigned int buf_size,
		   unsigned int *bytes_written);

void output_os_return(unsigned char *buf, unsigned int buf_size,
		      unsigned int *bytes_written);

unsigned char *raw_elf_header();
unsigned int raw_elf_header_size();

unsigned char *raw_linux_return();
unsigned int raw_linux_return_size();

#endif /* ELF_HEADER_H */
