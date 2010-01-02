#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "compiler.h"
#include "context.h"

typedef struct call_list_t {
	unsigned int calls;
	const char *call[100];
} call_list;

void call_list_to_string(void * data, char * buf, unsigned int buf_size) {
	call_list * this = (call_list *) data;
	unsigned int i, len;

	sprintf(buf, "call_list {\n");
	for (i = 0; i< this->calls; i++) {
		len = strlen(buf);
		sprintf(&buf[len], "%d:\t%s\n", i, this->call[i]);
	}
	len = strlen(buf);
	sprintf(&buf[len], "}\n");
	if (len > buf_size) {
		fprintf(stderr, "len > buf_size: %u > %u\n", len, buf_size);
	}
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

char fake_lex_look_ahead(void * data) {
	add_to_call_list(data, "lex_look_ahead");
	return '\0';
}

void fake_lex_advance(void * data, unsigned int chars) {
	add_to_call_list(data, "lex_advance");
	if (chars < 1) {
		fprintf(stderr, "advance: %u?\n", chars);
	}
}

int fake_lex_get_number(void * data) {
	unsigned int called;
	called = add_to_call_list(data, "lex_get_number");
	return called;
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

context_t * init_fake_context(context_t * ctx, call_list * list) {
	list->calls = 0;
	ctx->data = list;

	ctx->lex_look_ahead = fake_lex_look_ahead;
	ctx->lex_advance = fake_lex_advance;
	ctx->lex_get_number = fake_lex_get_number;

	ctx->output_term = fake_output_term;
	ctx->output_add = fake_output_add;
	ctx->output_subtract = fake_output_subtract;
	ctx->output_statements_complete = fake_output_statements_complete;

	ctx->read_line = fake_read_line;
	ctx->write_file = fake_write_file;

	ctx->output_header = fake_output_header;
	ctx->output_os_return = fake_output_os_return;

	ctx->to_string = call_list_to_string;

	return ctx;
}

void test_term_simple() {
	context_t local;
	call_list local_data;
	context_t * ctx = init_fake_context(&local, &local_data);
	char buf[1000];

	term(ctx);

	ctx->to_string(ctx->data, buf, sizeof(buf));

	check_unsigned_ints(local_data.calls, 4, buf);
	check_strs(local_data.call[0], "lex_look_ahead", buf);
	check_strs(local_data.call[1], "lex_get_number", buf);
	check_strs(local_data.call[2], "output_term", buf);
	check_strs(local_data.call[3], "lex_look_ahead", buf);
}

void check_expression_add_subtract(context_t * ctx, const char * op) {
	char buf[10000];
	call_list * calls = (call_list *) ctx->data;
	ctx->to_string(ctx->data, buf, sizeof(buf));

	check_unsigned_ints(calls->calls, 12, buf);
	/* term */
        check_strs(calls->call[0], "lex_look_ahead", buf);
	check_strs(calls->call[1], "lex_get_number", buf);
	check_strs(calls->call[2], "output_term", buf);
	check_strs(calls->call[3], "lex_look_ahead", buf);
	/* is_add_op */
	check_strs(calls->call[4], "lex_look_ahead", buf);
	/* yes, so advance */
	check_strs(calls->call[5], "lex_advance", buf);
	/* term again */
	check_strs(calls->call[6], "lex_look_ahead", buf);
	check_strs(calls->call[7], "lex_get_number", buf);
	check_strs(calls->call[8], "output_term", buf);
	check_strs(calls->call[9], "lex_look_ahead", buf);
	/* add/subtract them */
	check_strs(calls->call[10], op, buf);
	/* is_add_op */
	check_strs(calls->call[11], "lex_look_ahead", buf);
	/* no, so finish */
}

char fake_lex_look_ahead_2(void * data) {
	unsigned int called;
	called = add_to_call_list(data, "lex_look_ahead");

	if (called == 3) {
		return '+';
	}

	return '2';
}

void test_expression_add() {
	context_t local;
	call_list local_data;
	context_t * ctx = init_fake_context(&local, &local_data);

	ctx->lex_look_ahead = fake_lex_look_ahead_2;

	expression(ctx);

	check_expression_add_subtract(ctx, "output_add");
}

char fake_lex_look_ahead_3(void * data) {
	unsigned int called;
	called = add_to_call_list(data, "lex_look_ahead");

	if (called == 3) {
		return '-';
	}

	return '2';
}

void test_expression_subtract() {
	context_t local;
	call_list local_data;
	context_t * ctx = init_fake_context(&local, &local_data);

	ctx->lex_look_ahead = fake_lex_look_ahead_3;

	expression(ctx);

	check_expression_add_subtract(ctx, "output_subtract");
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_term_simple();
	test_expression_add();
	test_expression_subtract();

	return 0;
}
