#include "x86_asm.h"
#include "c_lib.h"

void output_term(int number,
		unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char b0, b1, b2, b3;

	if (buf_size < (*bytes_written) + 6) {
		err_msg("buf_size too small for term\n");
		return;
	}

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

void output_statements_complete(unsigned char * buf,
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

void output_subtract(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written) {

	if (buf_size < 5 + *bytes_written) {
		err_msg("buf_size too small for output_subtract\n");
		return;
	}
	buf[(*bytes_written)++] = 0x5a; /* popl %edx */
	buf[(*bytes_written)++] = 0x58; /* popl %eax */

	buf[(*bytes_written)++] = 0x29; /* subl %edx, %eax */
	buf[(*bytes_written)++] = 0xd0; /*   continued     */

	buf[(*bytes_written)++] = 0x50; /* pushl %eax */
}

void output_multiply(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written) {

	if (buf_size < 6 + *bytes_written) {
		err_msg("buf_size too small for output_subtract\n");
		return;
	}
	buf[(*bytes_written)++] = 0x5a; /* popl %edx */
	buf[(*bytes_written)++] = 0x58; /* popl %eax */

	buf[(*bytes_written)++] = 0x0f; /* imull %edx, %eax */
	buf[(*bytes_written)++] = 0xaf; /*   continued     */
	buf[(*bytes_written)++] = 0xc2; /*   continued     */

	buf[(*bytes_written)++] = 0x50; /* pushl %eax */
}

