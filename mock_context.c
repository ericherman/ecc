#include "mock_context.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "misc.h"

mock_data *_mock_data(context_t * ctx)
{
	return (mock_data *) ctx->data;
}

void mock_data_to_string(context_t * ctx, char *buf, unsigned int buf_size)
{
	mock_data *this = _mock_data(ctx);
	unsigned int i, len;

	sprintf(buf, "mock_data {\n");
	len = strlen(buf);
	sprintf(&buf[len], "track lex_look_ahead: %d\n", this->track_lookahead);
	len = strlen(buf);
	sprintf(&buf[len], "calls:\n");
	for (i = 0; i < this->calls; i++) {
		len = strlen(buf);
		sprintf(&buf[len], "%d:\t%s\n", i, this->call[i]);
	}
	len = strlen(buf);
	sprintf(&buf[len], "terms:\n");
	for (i = 0; i < this->terms; i++) {
		len = strlen(buf);
		sprintf(&buf[len], "%d:\t%d\n", i, this->term[i]);
	}
	len = strlen(buf);
	sprintf(&buf[len], "}\n");
	if (len > buf_size) {
		fprintf(stderr, "len > buf_size: %u > %u\n", len, buf_size);
	}
}

unsigned int times_called(mock_data * list, const char *func_name)
{
	unsigned int i, called = 0;

	for (i = 0; i < list->calls; i++) {
		if (strcmp(func_name, list->call[i]) == 0) {
			++called;
		}
	}
	return called;
}

void add_to_mock_data(context_t * ctx, const char *func_name)
{
	mock_data *list = _mock_data(ctx);
	list->call[list->calls++] = func_name;
}

int _last_was_look_ahead(context_t * ctx)
{
	mock_data *list = _mock_data(ctx);
	const char *last;

	if (list->calls == 0) {
		return 0;
	}
	last = list->call[list->calls - 1];
	return (strcmp(last, "lex_look_ahead") == 0) ? 1 : 0;
}

const char *fake_lex_look_ahead(context_t * ctx)
{
	mock_data *list = _mock_data(ctx);

	if (list->track_lookahead) {
		if (list->track_lookahead > 1 || !_last_was_look_ahead(ctx)) {
			add_to_mock_data(ctx, "lex_look_ahead");
		}
	}
	if (list->token_pos < list->tokens) {
		return list->token[list->token_pos];
	}
	return list->no_token;
}

void fake_lex_advance(context_t * ctx, unsigned int chars)
{
	mock_data *list = _mock_data(ctx);

	add_to_mock_data(ctx, "lex_advance");
	if (chars != strlen(list->token[list->token_pos])) {
		fprintf(stderr, "advance: %u?\n", chars);
	}
	list->token_pos += 1;
}

void fake_stack_enter(context_t * ctx)
{
	add_to_mock_data(ctx, "stack_enter");
}

void fake_stack_assign_name(context_t * ctx, const char *name)
{
	if (is_number(name[0])) {
		fprintf(stderr, "name '%s' starts with digit\n", name);
	}
	add_to_mock_data(ctx, "stack_assign_name");
}

unsigned int fake_stack_name_pos(context_t * ctx, const char *name)
{
	mock_data *this = _mock_data(ctx);

	if (is_number(name[0])) {
		fprintf(stderr, "name '%s' starts with digit\n", name);
	}

	add_to_mock_data(ctx, "stack_name_pos");
	return this->mock_return_uint_vals[this->mock_return_uint_val_c++];
}

unsigned int fake_stack_frame_size(context_t * ctx)
{
	mock_data *this = _mock_data(ctx);

	add_to_mock_data(ctx, "stack_frame_size");
	return this->mock_return_uint_vals[this->mock_return_uint_val_c++];
}

void fake_stack_leave(context_t * ctx)
{
	add_to_mock_data(ctx, "stack_leave");
}

void fake_output_term(context_t * ctx, int number)
{
	mock_data *list = _mock_data(ctx);
	add_to_mock_data(ctx, "output_term");
	list->term[list->terms++] = number;
}

void fake_output_add(context_t * ctx)
{
	add_to_mock_data(ctx, "output_add");
}

void fake_output_subtract(context_t * ctx)
{
	add_to_mock_data(ctx, "output_subtract");
}

void fake_output_multiply(context_t * ctx)
{
	add_to_mock_data(ctx, "output_multiply");
}

void fake_output_divide(context_t * ctx)
{
	add_to_mock_data(ctx, "output_divide");
}

void fake_output_stack_enter(context_t * ctx)
{
	add_to_mock_data(ctx, "output_stack_enter");
}

void fake_output_stack_allocate(context_t * ctx, unsigned int bytes)
{
	if (bytes == 0) {
		fprintf(stderr, "allocating zero bytes?\n");
	}
	add_to_mock_data(ctx, "output_stack_allocate");
}

void fake_output_stack_assign_int(context_t * ctx, unsigned int depth, int val)
{
	mock_data *list = _mock_data(ctx);
	if (depth == 0) {
		fprintf(stderr, "depth of zero?\n");
	}
	add_to_mock_data(ctx, "output_stack_assign_int");
	list->term[list->terms++] = val;
}

void fake_output_stack_leave(context_t * ctx)
{
	add_to_mock_data(ctx, "output_stack_leave");
}

void fake_output_statements_complete(context_t * ctx)
{
	add_to_mock_data(ctx, "output_statements_complete");
}

void fake_read_file(context_t * ctx)
{
	add_to_mock_data(ctx, "read_file");
}

void fake_write_file(context_t * ctx)
{
	add_to_mock_data(ctx, "write_file");
}

void fake_output_header(context_t * ctx)
{
	add_to_mock_data(ctx, "output_header");
}

void fake_output_os_return(context_t * ctx)
{
	add_to_mock_data(ctx, "output_os_return");
}

context_t *init_fake_context(const char **token, unsigned int tokens)
{
	context_t *ctx = malloc(sizeof(struct context_t_));
	mock_data *list = malloc(sizeof(struct mock_data_t));

	list->token = token;
	list->tokens = tokens;
	list->token_pos = 0;
	list->no_token[0] = '\0';
	list->terms = 0;
	list->calls = 0;
	list->track_lookahead = 1;
	list->mock_return_uint_val_c = 0;

	ctx->data = list;

	ctx->lex_look_ahead = fake_lex_look_ahead;
	ctx->lex_advance = fake_lex_advance;

	ctx->stack_enter = fake_stack_enter;
	ctx->stack_assign_name = fake_stack_assign_name;
	ctx->stack_name_pos = fake_stack_name_pos;
	ctx->stack_frame_size = fake_stack_frame_size;
	ctx->stack_leave = fake_stack_leave;

	ctx->output_term = fake_output_term;
	ctx->output_add = fake_output_add;
	ctx->output_subtract = fake_output_subtract;
	ctx->output_multiply = fake_output_multiply;
	ctx->output_divide = fake_output_divide;
	ctx->output_stack_enter = fake_output_stack_enter;
	ctx->output_stack_allocate = fake_output_stack_allocate;
	ctx->output_stack_assign_int = fake_output_stack_assign_int;
	ctx->output_stack_leave = fake_output_stack_leave;
	ctx->output_statements_complete = fake_output_statements_complete;

	ctx->read_file = fake_read_file;
	ctx->write_file = fake_write_file;

	ctx->output_header = fake_output_header;
	ctx->output_os_return = fake_output_os_return;

	ctx->to_string = mock_data_to_string;

	return ctx;
}

void free_fake_context(context_t * ctx)
{
	free(ctx->data);
	free(ctx);
}
