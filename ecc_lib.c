#include "ecc_lib.h"
#include "c_lib.h"
#include "elf_header.h"
#include "lex.h"

void output_header(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * length) {

	unsigned char * bytes;
	unsigned int i;

	*length = raw_elf_header_size();
	if (buf_size < *length) {
		err_msg("buf_size too small");
		return;
	}
	bytes = raw_elf_header();
	for (i = 0; i < *length; i++) {
		buf[i] = bytes[i];
	}
}

void output_os_return(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * length) {

	unsigned char * bytes;
	unsigned int i;

	*length += raw_linux_return_size();
	if (buf_size < *length) {
		err_msg("buf_size too small");
		return;
	}
	bytes = raw_linux_return();
	for (i = 0; i < *length; i++) {
		buf[i] = bytes[i];
	}
}

void expression(const char * input,
		unsigned int input_size,
		unsigned char * buffer,
		unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written) {

	term(input, input_size, buffer, buf_size, chars_read, bytes_written);
}

void term(const char * input,
		unsigned int input_size,
		unsigned char * buf,
		unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written) {

	int number;

	if (buf_size < 6) {
		err_msg("buf_size too small");
		return;
	}

	number = lex_get_number(input, input_size, chars_read);
	/* pushl $number */
	buf[0] = 0x68; /* pushl $immediate_value */
	buf[1] = 0xFF & number;
	buf[2] = 0xFF & (number >> 8);
	buf[3] = 0xFF & (number >> 16);
	buf[4] = 0xFF & (number >> 24);
	*bytes_written += 5;
}

void statements_complete(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written) {

	if (buf_size < 1) {
		err_msg("buf_size too small");
		return;
	}
	*bytes_written += 1;
	buf[0] = 0x5b; /* popl %ebx */
}

#define LINE_MAX 1024
#define BUFF_MAX 0xFFFF

void compile(const char * source_file, const char * executable) {
	char line_buf[LINE_MAX];
	unsigned char byte_buffer[BUFF_MAX];
	unsigned int bytes_written;
	unsigned int chars_read;

	read_line(source_file, line_buf, sizeof(line_buf), &chars_read);
	compile_inner(line_buf, chars_read,
			byte_buffer, sizeof(byte_buffer), &bytes_written);
	write_file(executable, byte_buffer, bytes_written);
}

void compile_inner(const char * line_buf, unsigned int line_buf_size,
		unsigned char * byte_buffer, unsigned int byte_buffer_max,
		unsigned int * bytes_written) {

	unsigned int chars_read;

	*bytes_written = 0;

	output_header(byte_buffer, byte_buffer_max, bytes_written);

	expression(line_buf, line_buf_size,
			&byte_buffer[*bytes_written],
			byte_buffer_max - *bytes_written,
			&chars_read, bytes_written);

	statements_complete(&byte_buffer[*bytes_written],
			byte_buffer_max - *bytes_written,
			bytes_written);

	output_os_return(&byte_buffer[*bytes_written],
			byte_buffer_max - *bytes_written,
			bytes_written);
}
