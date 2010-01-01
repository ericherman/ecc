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

char fake_lex_look_ahead_1(void * data) {
	unsigned int called;
	called = add_to_call_list(data, "lex_look_ahead");

	if (called == 1) {
		return '1';
	}

	return '\0';
}

void test_term_simple() {
	context_t local;
	call_list call_list;
	context_t * ctx = init_fake_context(&local, &call_list);
	char buf[1000];

	ctx->lex_look_ahead = fake_lex_look_ahead_1;
	term(ctx);

	ctx->to_string(ctx->data, buf, sizeof(buf));

	/* check_unsigned_ints(call_list.calls, 3, buf); */
	check_strs(call_list.call[0],"lex_get_number", buf);
	check_strs(call_list.call[1], "output_term", buf);
	/* check_strs(call_list.call[2], "lex_look_ahead", buf); */
}

char fake_lex_look_ahead_2(void * data) {
	unsigned int called;
	called = add_to_call_list(data, "lex_look_ahead");

	if (called == 1) {
		return '+';
	}

	return '2';
}

void test_expression_add() {
	context_t local;
	call_list call_list;
	context_t * ctx = init_fake_context(&local, &call_list);
	char buf[10000];

	ctx->lex_look_ahead = fake_lex_look_ahead_2;

	expression(ctx);


	ctx->to_string(ctx->data, buf, sizeof(buf));

	check_unsigned_ints(call_list.calls, 8, buf);
	check_strs(call_list.call[0],"lex_get_number", buf);
	check_strs(call_list.call[1], "output_term", buf);
	check_strs(call_list.call[2], "lex_look_ahead", buf);
	check_strs(call_list.call[3], "lex_advance", buf);
	check_strs(call_list.call[4], "lex_get_number", buf);
	check_strs(call_list.call[5], "output_term", buf);
	check_strs(call_list.call[6], "output_add", buf);
	check_strs(call_list.call[7], "lex_look_ahead", buf);
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_term_simple();
	test_expression_add();

	return 0;
}
