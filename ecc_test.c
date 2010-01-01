#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void check_str(const char * actual, const char * expected) {
	if (strcmp(expected, actual) != 0) {
		fprintf(stderr, "expected '%s' but was '%s'\n",
				expected, actual);
		exit(1);
	}
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


typedef struct call_list_t {
	unsigned int calls;
	const char *call[100];
} call_list;

void call_list_print(void * data) {
	call_list * this = (call_list *) data;
	unsigned int i;

	fprintf(stderr, "call_list {\n");
	for (i = 0; i< this->calls; i++) {
		fprintf(stderr, "%d:\t%s\n", i, this->call[i]);
	}
	fprintf(stderr, "}\n");
}

unsigned int times_called(call_list * list, const char * func_name) {
	unsigned int i, called = 0;

	for (i = 0; i< list->calls; i++) {
		if(strcmp(func_name, list->call[i]) == 0) {
			++called;
		}
	}
	return called;
}

unsigned int add_to_call_list(void * data, const char * func_name) {
        call_list * list = (call_list *) data;
        list->call[list->calls++] = func_name;
        return times_called(list, func_name);
}

char fake_lex_look_ahead_1(void * data) {
	unsigned int called;
	called = add_to_call_list(data, "lex_look_ahead");

	if (called == 1) {
		return '+';
	}

	return '2';
}

void fake_lex_advance(void * data, unsigned int chars) {
	add_to_call_list(data, "lex_advance");
	if (chars < 1) {
		fprintf(stderr, "advance: %u?\n", chars);
	}
}

int fake_lex_get_number_1(void * data) {
	unsigned int called;
	called = add_to_call_list(data, "lex_get_number");
	if (called == 1) {
		return 17;
	}
	return 23;
}

void fake_output_term(void * data, int number) {
	add_to_call_list(data, "output_term");
	/* should we store this for assertion? */
	if (number == 0) {
		;
	}
}

void fake_output_add(void * data) {
	add_to_call_list(data, "output_add");
}

void fake_output_subtract(void * data) {
	add_to_call_list(data, "output_subtract");
}

void fake_output_statements_complete(void * data) {
	add_to_call_list(data, "output_statements_complete");
}

void fake_read_line(void * data) {
	add_to_call_list(data, "read_line");
}

void fake_write_file(void * data) {
	add_to_call_list(data, "write_file");
}

void fake_output_header(void * data) {
	add_to_call_list(data, "output_header");
}

void fake_output_os_return(void * data) {
	add_to_call_list(data, "output_os_return");
}

void test_expression_add() {
	context_t local;
	call_list call_list;
	context_t * ctx = &local;

	call_list.calls = 0;
	ctx->data = &call_list;


	ctx->lex_look_ahead = fake_lex_look_ahead_1;
	ctx->lex_advance = fake_lex_advance;
	ctx->lex_get_number = fake_lex_get_number_1;

	ctx->output_term = fake_output_term;
	ctx->output_add = fake_output_add;
	ctx->output_subtract = fake_output_subtract;
	ctx->output_statements_complete = fake_output_statements_complete;

	ctx->read_line = fake_read_line;
	ctx->write_file = fake_write_file;

	ctx->output_header = fake_output_header;
	ctx->output_os_return = fake_output_os_return;

	ctx->print = call_list_print;

	expression(ctx);

	/*
	 * ctx->print(ctx->data);
	 */
	check_unsigned_int(call_list.calls, 8);
	check_str(call_list.call[0],"lex_get_number");
	check_str(call_list.call[1], "output_term");
	check_str(call_list.call[2], "lex_look_ahead");
	check_str(call_list.call[3], "lex_advance");
	check_str(call_list.call[4], "lex_get_number");
	check_str(call_list.call[5], "output_term");
	check_str(call_list.call[6], "output_add");
	check_str(call_list.call[7], "lex_look_ahead");
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
