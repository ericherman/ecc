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
		err_msg("buf_size too small for output_header\n");
		return;
	}
	bytes = raw_elf_header();
	for (i = 0; i < *length; i++) {
		buf[i] = bytes[i];
	}
}

void output_os_return(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char * bytes;
	unsigned int i, length;

	length = raw_linux_return_size();
	if (buf_size < *bytes_written + length) {
		err_msg("buf_size too small for output_os_return\n");
		return;
	}
	bytes = raw_linux_return();
	for (i = 0; i < length; i++) {
		buf[(*bytes_written)++] = bytes[i];
	}
}

void expression(const char * input,
		unsigned int input_size,
		unsigned char * buffer,
		unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written) {


	term(input, input_size, buffer, buf_size, chars_read, bytes_written);

	while (lex_look_ahead(input, input_size, chars_read) == '+') {
		*chars_read += 1;
		term(input, input_size, buffer, buf_size,
				chars_read, bytes_written);

		output_add(buffer, buf_size, bytes_written);
	}
}

void term(const char * input,
		unsigned int input_size,
		unsigned char * buf,
		unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written) {

	int number;
	const char * sub_str;
	unsigned int sub_str_size;
	unsigned char b0, b1, b2, b3;

	if (buf_size < (*bytes_written) + 6) {
		err_msg("buf_size too small for term\n");
		return;
	}

	sub_str = &input[*chars_read];
	sub_str_size = input_size - *chars_read;

	number = lex_get_number(sub_str, sub_str_size, chars_read);
	b0 = 0xFF & number;
	b1 = 0xFF & (number >> 8);
	b2 = 0xFF & (number >> 16);
	b3 = 0xFF & (number >> 24);

	/* pushl $number */
	buf[(*bytes_written)++] = 0x68; /* pushl $immediate_value */
	buf[(*bytes_written)++] = b0;
	buf[(*bytes_written)++] = b1;
	buf[(*bytes_written)++] = b2;
	buf[(*bytes_written)++] = b3;
}

void statements_complete(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written) {

	if (buf_size < *bytes_written + 1) {
		err_msg("buf_size too small for statements_complete\n");
		return;
	}
	buf[(*bytes_written)++] = 0x5b; /* popl %ebx */
}

void output_add(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written) {

	if (buf_size < 5 + *bytes_written) {
		err_msg("buf_size too small for output_add\n");
		return;
	}
        buf[(*bytes_written)++] = 0x5a; /* popl %edx */
        buf[(*bytes_written)++] = 0x58; /* popl %eax */

	buf[(*bytes_written)++] = 0x01; /* addl %edx, %eax */
	buf[(*bytes_written)++] = 0xd0; /*   continued     */

	buf[(*bytes_written)++] = 0x50; /* pushl %eax */
}

#define LINE_MAX 1024
#define BUFF_MAX 0xFFFF

void compile(const char * source_file, const char * executable) {
	char line_buf[LINE_MAX];
	unsigned char byte_buffer[BUFF_MAX];
	unsigned int bytes_written = 0;
	unsigned int chars_read = 0;

	read_line(source_file, line_buf, sizeof(line_buf), &chars_read);
	compile_inner(line_buf, chars_read+1,
			byte_buffer, sizeof(byte_buffer), &bytes_written);
	write_file(executable, byte_buffer, bytes_written);
}

void compile_inner(const char * line_buf, unsigned int line_buf_size,
		unsigned char * byte_buffer, unsigned int byte_buffer_max,
		unsigned int * bytes_written) {

	unsigned int chars_read = 0;

	*bytes_written = 0;

	output_header(byte_buffer, byte_buffer_max, bytes_written);

	expression(line_buf, line_buf_size,
			byte_buffer, byte_buffer_max,
			&chars_read, bytes_written);

	statements_complete(byte_buffer, byte_buffer_max, bytes_written);

	output_os_return(byte_buffer, byte_buffer_max, bytes_written);
}
