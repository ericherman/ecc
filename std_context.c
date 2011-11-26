#include "std_context.h"
#include "std_context_t.h"
#include "c_lib.h"
#include "lex.h"
#include "x86_machine_code.h"
#include "elf_header.h"
#include "misc.h"

/* convenience function to hide casting clutter */
inline std_context_t *_std_context(context_t * ctx)
{
	return (std_context_t *) ctx->data;
}

void std_to_string(context_t * ctx, char *buf, unsigned int buf_size)
{
	std_context_t *this;
	unsigned int buf_pos;

	this = _std_context(ctx);
	buf_pos = 0;

	str_cpy_offset("std_context_t {\n", buf, buf_size, &buf_pos);
	str_cpy_offset("    source_file: ", buf, buf_size, &buf_pos);
	str_cpy_offset(this->source_file, buf, buf_size, &buf_pos);
	str_cpy_offset("\n", buf, buf_size, &buf_pos);
	str_cpy_offset("    buf: ", buf, buf_size, &buf_pos);
	str_cpy_offset(this->buf, buf, buf_size, &buf_pos);
	str_cpy_offset("\n", buf, buf_size, &buf_pos);
	str_cpy_offset("\n", buf, buf_size, &buf_pos);
	str_cpy_offset("}\n", buf, buf_size, &buf_pos);
}

const char *std_lex_look_ahead(context_t * ctx)
{
	std_context_t *this;
	unsigned int *pos;

	this = _std_context(ctx);
	pos = &(this->buf_pos);
	lex_look_ahead(this->buf, this->buf_size, pos, this->next_token,
		       TOKEN_MAX);
	return this->next_token;
}

void std_lex_advance(context_t * ctx, unsigned int chars)
{
	std_context_t *this;

	this = _std_context(ctx);
	this->buf_pos += chars;
}

int std_lex_get_number(context_t * ctx)
{
	std_context_t *this;
	const char *sub_str;
	unsigned int sub_str_size;

	this = _std_context(ctx);
	sub_str = &(this->buf[this->buf_pos]);
	sub_str_size = this->buf_size - this->buf_pos;

	return lex_get_number(sub_str, sub_str_size, &(this->buf_pos));
}

void std_output_term(context_t * ctx, int number)
{
	std_context_t *this;

	this = _std_context(ctx);
	output_term(number, this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_add(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	output_add(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_subtract(context_t * ctx)
{
	std_context_t *this = _std_context(ctx);
	output_subtract(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_multiply(context_t * ctx)
{
	std_context_t *this = _std_context(ctx);
	output_multiply(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_divide(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	output_divide(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_stack_enter(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	output_stack_enter(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_stack_leave(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	output_stack_leave(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_statements_complete(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	output_statements_complete(this->byte_buf, BBUF_MAX,
				   &(this->bytes_written));
}

void std_read_file(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	read_file(this->source_file, this->buf, FILE_MAX, &(this->buf_size));
	this->buf_size += 1;
}

void std_write_file(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	write_file(this->out_file, this->byte_buf, this->bytes_written);
}

void std_output_header(context_t * ctx)
{
	std_context_t *this;

	this = _std_context(ctx);
	output_header(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_os_return(context_t * ctx)
{
	std_context_t *this = _std_context(ctx);
	output_os_return(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

context_t *alloc_std_context(const char *source_file, const char *out_file)
{

	context_t *ctx;
	unsigned int size;
	std_context_t *data;

	size = sizeof(struct context_t_);

	ctx = heap_malloc(size);
	if (!ctx) {
		err_msg("could not allocate context_t\n");
		die();
	}

	ctx->lex_look_ahead = std_lex_look_ahead;
	ctx->lex_advance = std_lex_advance;
	ctx->lex_get_number = std_lex_get_number;

	ctx->output_term = std_output_term;
	ctx->output_add = std_output_add;
	ctx->output_subtract = std_output_subtract;
	ctx->output_multiply = std_output_multiply;
	ctx->output_divide = std_output_divide;
	ctx->output_stack_enter = std_output_stack_enter;
	ctx->output_stack_leave = std_output_stack_leave;
	ctx->output_statements_complete = std_output_statements_complete;

	ctx->read_file = std_read_file;
	ctx->write_file = std_write_file;

	ctx->output_header = std_output_header;
	ctx->output_os_return = std_output_os_return;

	ctx->to_string = std_to_string;

	size = sizeof(struct std_context_t_);
	ctx->data = heap_malloc(size);
	if (!ctx->data) {
		err_msg("could not allocate context data\n");
		die();
	}
	data = (std_context_t *) ctx->data;

	data->source_file = source_file;
	data->buf[0] = '\0';
	data->buf_size = 0;
	data->buf_pos = 0;
	data->next_token[0] = '\0';
	data->out_file = out_file;
	data->bytes_written = 0;

	return ctx;
}

void free_std_context(context_t * ctx)
{
	heap_free(ctx->data);
	heap_free(ctx);
}
