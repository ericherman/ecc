/* x86_asm.h */
#ifndef _X86_ASM_H_
#define _X86_ASM_H_

void output_term(int number,
		unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written);

void output_statements_complete(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written);

void output_add(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written);

void output_subtract(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written);

void output_multiply(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written);

void output_divide(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written);

#endif /* _X86_ASM_H_ */
