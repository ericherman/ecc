/* ecc_lib.h */
#ifndef _ECC_LIB_H_
#define _ECC_LIB_H_

void compile(const char * input_file, const char * executable);

void compile_inner(const char * line_buf, unsigned int line_buf_size,
		unsigned char * byte_buffer, unsigned int byte_buffer_max,
		unsigned int * total_bytes_written);

void output_header(unsigned char * buffer,
		unsigned int buf_size,
		unsigned int * bytes_written);

void output_os_return(unsigned char * buffer,
		unsigned int buf_size,
		unsigned int * bytes_written);

void term(const char * input,
		unsigned int input_size,
		unsigned char * buffer,
		unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written);

void statements_complete(unsigned char * buffer,
		unsigned int buf_size,
		unsigned int * bytes_written);

#endif /* _ECC_LIB_H_ */

