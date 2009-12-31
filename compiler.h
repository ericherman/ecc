/* compiler.h */
#ifndef _COMPILER_H_
#define _COMPILER_H_

void compile(const char * input_file, const char * executable);

void compile_inner(const char * line_buf, unsigned int line_buf_size,
		unsigned char * byte_buffer, unsigned int byte_buffer_max,
		unsigned int * total_bytes_written);

void term(const char * input, unsigned int input_size,
		unsigned char * buf, unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written);

void expression(const char * input, unsigned int input_size,
		unsigned char * buf, unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written);

#endif /* _COMPILER_H_ */

