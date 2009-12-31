#include "compiler.h"
#include "c_lib.h"
#include "elf_header.h"
#include "lex.h"
#include "x86_asm.h"

int is_add_op(const char c) {
	return c == '+' || c == '-';
}

void expression(const char * input,
		unsigned int input_size,
		unsigned char * buffer,
		unsigned int buf_size,
		unsigned int * chars_read,
		unsigned int * bytes_written) {

	char c;

	term(input, input_size, buffer, buf_size, chars_read, bytes_written);

	while (is_add_op(c = lex_look_ahead(input, input_size, chars_read))) {

		*chars_read += 1;

		term(input, input_size, buffer, buf_size,
				chars_read, bytes_written);

		switch (c) {
		case '+':
			output_add(buffer, buf_size, bytes_written);
			break;
		case '-':
			output_subtract(buffer, buf_size, bytes_written);
		}
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

	sub_str = &input[*chars_read];
	sub_str_size = input_size - *chars_read;

	number = lex_get_number(sub_str, sub_str_size, chars_read);

	output_term(number, buf, buf_size, bytes_written);
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

	output_statements_complete(byte_buffer, byte_buffer_max, bytes_written);

	output_os_return(byte_buffer, byte_buffer_max, bytes_written);
}
