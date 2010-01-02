#include "x86_asm.h"
#include "c_lib.h"

void write_bytes(const char * name,
		unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written,
		const unsigned char * to_write,
		unsigned int count) {

	unsigned int i;
	if (buf_size < (*bytes_written) + count) {
		err_msg("buf_size too small for ");
		err_msg(name);
		err_msg("\n");
		return;
	}
	for (i = 0; i < count; i++) {
		buf[(*bytes_written)++] = to_write[i];
	}
}

void output_term(int number,
		unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char bytes[5];

	bytes[0] = 0x68; /* pushl $immediate_value */
	bytes[1] = 0xFF & number;
	bytes[2] = 0xFF & (number >> 8);
	bytes[3] = 0xFF & (number >> 16);
	bytes[4] = 0xFF & (number >> 24);

	write_bytes("term", buf, buf_size, bytes_written, bytes, 5);
}

void output_statements_complete(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char bytes[] = { 0x5b /* popl %ebx */ };

	write_bytes("return val", buf, buf_size, bytes_written, bytes, 1);
}

void output_add(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char bytes[] = {
		0x5a, /* popl %edx */
		0x58, /* popl %eax */
		0x01, 0xd0, /* addl %edx, %eax */
		0x50, /* pushl %eax */
	};
	write_bytes("add", buf, buf_size, bytes_written, bytes, 5);
}

void output_subtract(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char bytes[] = {
		0x5a, /* popl %edx */
		0x58, /* popl %eax */
		0x29, 0xd0, /* subl %edx, %eax */
		0x50, /* pushl %eax */
	};
	write_bytes("subtract", buf, buf_size, bytes_written, bytes, 5);
}

void output_multiply(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char bytes[] = {
		0x5a, /* popl %edx */
		0x58, /* popl %eax */
		0x0f, 0xaf, 0xc2, /* imull %edx, %eax */
		0x50, /* pushl %eax */
	};
	write_bytes("multiply", buf, buf_size, bytes_written, bytes, 6);
}

void output_divide(unsigned char * buf, unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char bytes[] = {
		0xba, 0x00, 0x00, 0x00, 0x00, /* movl $0, %edx */
		0x5b, /* popl %ebx */
		0x58, /* popl %eax */
		0x0f, 0xfb, /* idiv %ebx */
		0x50, /* pushl %eax */
	};
	write_bytes("divide", buf, buf_size, bytes_written, bytes, 10);
}
