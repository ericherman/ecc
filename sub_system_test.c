#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "compiler.h"
#include "std_context.h"
#include "std_context_t.h"

void test_compile_inner() {
	unsigned char full_monty[] = {
		/* ELF HEADER */
		/* MAGIC: */
		/* 127,E,L,F,1,1,1,0 */
		/*   0,0,0,0,0,0,0,0 */
		0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		/* ? */
		0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x54, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x08,
		0x00, 0x80, 0x04, 0x08,


		/* the 0xFF, 0xFF here is program size? */
		0xff, 0xff, 0x00, 0x00,
		/* the 0xFF, 0xFF here is memory size? */
		0xff, 0xff, 0x00, 0x00,

		/* ? */
		0x05, 0x00, 0x00, 0x00,
		0x00, 0x10, 0x00, 0x00,

		/* our program */
		0x68, /* push immediate value */
		0x17, 0x00, 0x00, 0x00, /* 23 */

		/* statemens complete */
		0x5b, /* pop ebx */

		/* program footer */
		0xb8, /* movl %eax */
		0x01, 0x00, 0x00, 0x00, /* 1 == SYS_EXIT */
		0xcd, /* int */
		0x80  /* Linux Kernel */
	};
	context_t * ctx = alloc_std_context("foo", "bar");
	std_context_t * data = (std_context_t *) ctx->data;
	data->buf[0] = '2';
	data->buf[1] = '3';
	data->buf[2] = '\0';
	data->buf_size = 3;

	compile_inner(ctx);

	compare_byte_arrays("full monty", full_monty, sizeof(full_monty),
			data->byte_buf, data->bytes_written);

	free_std_context(ctx);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_compile_inner();

	return 0;
}