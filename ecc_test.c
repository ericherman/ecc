#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
#include "elf_header.h"
#include "lex.h"
#include "x86_asm.h"
#include "context.h"
#include "std_context.h"
#include "std_context_t.h"

unsigned char addl_ops[] = {
	0x5a, /* popl %edx */
	0x58, /* popl %eax */
	0x01, 0xd0, /* addl %edx, %eax */
	0x50, /* pushl %eax */
};

unsigned char subl_ops[] = {
	0x5a, /* popl %edx */
	0x58, /* popl %eax */
	0x29, 0xd0, /* subl %edx, %eax */
	0x50, /* pushl %eax */
};

unsigned char imull_ops[] = {
	0x5a, /* popl %edx */
	0x58, /* popl %eax */
	0x0f, 0xaf, 0xc2, /* imull %edx, %eax */
	0x50, /* pushl %eax */
};

unsigned char push_17[] = {
	0x68, /* push immediate value */
	0x11, 0x00, 0x00, 0x00, /* 17 */
};

unsigned char push_23[] = {
	0x68, /* push immediate value */
	0x17, 0x00, 0x00, 0x00, /* 23 */
};

void check_char(char actual, char expected) {
	if (expected != actual) {
		fprintf(stderr, "expected '%c' but was '%c'\n",
				expected, actual);
		exit(1);
	}
}

void check_unsigned_int(unsigned int actual, unsigned int expected) {
	if (expected != actual) {
		fprintf(stderr, "expected %d but was %d\n", expected, actual);
		exit(1);
	}
}

void compare_byte_arrays(const char * name,
		unsigned char * expected, unsigned int expected_len,
		unsigned char * actual, unsigned int actual_len) {

	unsigned int i;

	if ( actual_len != expected_len ) {
		fprintf(stderr, "actual/expected length mis-match %d != %d\n",
			actual_len, expected_len);
		goto fail;
	}
	for (i = 0; i < actual_len; i++) {
		if (actual[i] != expected[i]) {
			fprintf(stderr, "buffers differ at %d\n", i);
			goto fail;
		}
	}
	return;

fail:
	fprintf(stderr, "FAIL: %s\n", name);
	for (i = 0; i < actual_len; i++) {
		fprintf(stderr, "actual[%d]=%02x\n", i, actual[i]);
	}
	for (i = 0; i < expected_len; i++) {
		fprintf(stderr, "expected[%d]=%02x\n", i, expected[i]);
	}
	exit(1);
}

void test_output_header() {
	unsigned char * expected;
	unsigned expected_len;
	unsigned char actual[128];
	unsigned actual_len;
	unsigned int min;

	expected = raw_elf_header();
	expected_len = raw_elf_header_size();
	min = 45;

	output_header(actual, 128, &actual_len);

	if (actual_len < min) {
		fprintf(stderr, "elf headers are at least %d bytes\n", min);
	}

	compare_byte_arrays("ELF", expected, expected_len, actual, actual_len);
}

void test_output_footer() {
	unsigned char * expected;
	unsigned int expected_len;
	unsigned char actual[128];
	unsigned int actual_len = 0;

	expected = raw_linux_return();
	expected_len = raw_linux_return_size();

	output_os_return(actual, 128, &actual_len);

	compare_byte_arrays("SYS_EXIT",
			expected, expected_len, actual, actual_len);
}

void test_term_simple() {

	context_t * ctx = alloc_std_context("foo", "bar");
	std_context_t * data = (std_context_t *) ctx->data;
	data->buf[0] = '1';
	data->buf[1] = '7';
	data->buf[2] = '\0';
	data->buf_size = 3;

	term(ctx);

	compare_byte_arrays("term", push_17, sizeof(push_17),
			data->byte_buf, data->bytes_written);

	free_std_context(ctx);
}

void test_statements_complete() {
	unsigned char pop_ebx[] = { 0x5b };
	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_statements_complete(buffer, 128, &bytes_written);
	compare_byte_arrays("statements_complete", pop_ebx, sizeof(pop_ebx),
			buffer, bytes_written);
}

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

void test_output_add() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_add(buffer, 128, &bytes_written);

	compare_byte_arrays("output_add", addl_ops, sizeof(addl_ops),
			buffer, bytes_written);
}

void test_output_subtract() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_subtract(buffer, 128, &bytes_written);

	compare_byte_arrays("output_subtract", subl_ops, sizeof(subl_ops),
			buffer, bytes_written);
}

void test_output_multiply() {

	unsigned char buffer[128];
	unsigned int bytes_written = 0;

	output_multiply(buffer, 128, &bytes_written);

	compare_byte_arrays("output_subtract", imull_ops, sizeof(imull_ops),
			buffer, bytes_written);
}

void test_expression_add() {
	unsigned int i, expected_read, expected_bytes;
	context_t * ctx = alloc_std_context("foo", "bar");
	std_context_t * data = (std_context_t *) ctx->data;

	data->buf[0] = '1';
	data->buf[1] = '7';
	data->buf[2] = '+';
	data->buf[3] = '2';
	data->buf[4] = '3';
	data->buf[5] = '\0';
	data->buf_size = 6;

	expression(ctx);

	expected_read = 5;
	check_unsigned_int(data->buf_pos, expected_read);

	expected_bytes = sizeof(push_17) + sizeof(push_23) + sizeof(addl_ops);
	check_unsigned_int(data->bytes_written, expected_bytes);

	compare_byte_arrays("term1", push_17, sizeof(push_17),
			data->byte_buf, sizeof(push_17));

	i = sizeof(push_17);
	compare_byte_arrays("term2", push_23, sizeof(push_23),
			&data->byte_buf[i], sizeof(push_23));

	i = sizeof(push_17) + sizeof(push_23);
	compare_byte_arrays("add", addl_ops, sizeof(addl_ops),
			&data->byte_buf[i], sizeof(addl_ops));

	free_std_context(ctx);
}

/* lex tests */
void test_lex_look_ahead() {
	const char * input = "1+ 2";
	unsigned int pos, expected_pos;
	char result, expected_result;

	pos = 0;
	result = lex_look_ahead(input, sizeof(input), &pos);
	expected_pos = 0;
	expected_result = '1';
	check_unsigned_int(pos, expected_pos);
	check_char(result, expected_result);

	pos = 1;
	result = lex_look_ahead(input, sizeof(input), &pos);
	expected_pos = 1;
	expected_result = '+';
	check_unsigned_int(pos, expected_pos);
	check_char(result, expected_result);

	pos = 2;
	result = lex_look_ahead(input, sizeof(input), &pos);
	expected_result = '2';
	expected_pos = 3;
	check_unsigned_int(pos, expected_pos);
	check_char(result, expected_result);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_output_header();
	test_output_footer();
	test_term_simple();
	test_statements_complete();
	test_compile_inner();
	test_output_add();
	test_expression_add();
	test_output_subtract();
	test_output_multiply();

	test_lex_look_ahead();
	return 0;
}
