#include "ecc_std_context_type.h"
#include "ecc_std_context.h"
#include "c_lib.h"
#include "lex.h"
#include "x86_machine_code.h"
#include "elf_header.h"
#include "misc.h"
#include "names_stack.h"

/* convenience function to hide casting clutter */
inline ecc_std_context *_std_context(ecc_context *ctx)
{
	return (ecc_std_context *) ctx->data;
}

void std_to_string(ecc_context *ctx, char *buf, unsigned int buf_size)
{
	ecc_std_context *this;
	unsigned int buf_pos;

	this = _std_context(ctx);
	buf_pos = 0;

	str_cpy_offset("ecc_std_context {\n", buf, buf_size, &buf_pos);
	str_cpy_offset("    source_file: ", buf, buf_size, &buf_pos);
	str_cpy_offset(this->source_file, buf, buf_size, &buf_pos);
	str_cpy_offset("\n", buf, buf_size, &buf_pos);
	str_cpy_offset("    buf: ", buf, buf_size, &buf_pos);
	str_cpy_offset(this->buf, buf, buf_size, &buf_pos);
	str_cpy_offset("\n", buf, buf_size, &buf_pos);
	str_cpy_offset("\n", buf, buf_size, &buf_pos);
	str_cpy_offset("}\n", buf, buf_size, &buf_pos);
}

const char *std_lex_look_ahead(ecc_context *ctx)
{
	ecc_std_context *this;
	unsigned int *pos;

	this = _std_context(ctx);
	pos = &(this->buf_pos);
	lex_look_ahead(this->buf, this->buf_size, pos, this->next_token,
		       TOKEN_MAX);
	return this->next_token;
}

void std_lex_advance(ecc_context *ctx, unsigned int chars)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	this->buf_pos += chars;
}

void std_stack_enter(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	stack_enter(this->names_stack);
}

void std_stack_assign_name(ecc_context *ctx, const char *name)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	stack_name_add(this->names_stack, name);
}

unsigned int std_stack_name_pos(ecc_context *ctx, const char *name)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	return stack_name_pos(this->names_stack, name);
}

unsigned int std_stack_frame_size(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	return stack_frame_size(this->names_stack);
}

void std_stack_leave(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	stack_leave(this->names_stack);
}

void std_output_term(ecc_context *ctx, int number)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_term(number, this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_add(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_add(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_subtract(ecc_context *ctx)
{
	ecc_std_context *this = _std_context(ctx);
	output_subtract(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_multiply(ecc_context *ctx)
{
	ecc_std_context *this = _std_context(ctx);
	output_multiply(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_divide(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_divide(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_stack_enter(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_stack_enter(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_stack_allocate(ecc_context *ctx, unsigned int bytes)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_stack_allocate(bytes, this->byte_buf, BBUF_MAX,
			      &(this->bytes_written));
}

void std_output_stack_assign_int(ecc_context *ctx, unsigned int depth, int num)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_stack_assign_int(depth, num, this->byte_buf, BBUF_MAX,
				&(this->bytes_written));
}

void std_output_stack_leave(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_stack_leave(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_statements_complete(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_statements_complete(this->byte_buf, BBUF_MAX,
				   &(this->bytes_written));
}

void std_read_file(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	read_file(this->source_file, this->buf, FILE_MAX, &(this->buf_size));
	this->buf_size += 1;
}

void std_write_file(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	write_file(this->out_file, this->byte_buf, this->bytes_written);
}

void std_output_header(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	output_header(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_os_return(ecc_context *ctx)
{
	ecc_std_context *this = _std_context(ctx);
	output_os_return(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

ecc_context *alloc_std_context(const char *source_file, const char *out_file)
{

	ecc_context *ctx;
	unsigned int size;
	ecc_std_context *data;
	names_stack *names_stack;

	size = sizeof(ecc_context);

	ctx = heap_malloc(size);
	if (!ctx) {
		err_msg("could not allocate ecc_context\n");
		die();
	}

	ctx->lex_look_ahead = std_lex_look_ahead;
	ctx->lex_advance = std_lex_advance;

	ctx->stack_enter = std_stack_enter;
	ctx->stack_assign_name = std_stack_assign_name;
	ctx->stack_name_pos = std_stack_name_pos;
	ctx->stack_frame_size = std_stack_frame_size;
	ctx->stack_leave = std_stack_leave;

	ctx->output_term = std_output_term;
	ctx->output_add = std_output_add;
	ctx->output_subtract = std_output_subtract;
	ctx->output_multiply = std_output_multiply;
	ctx->output_divide = std_output_divide;
	ctx->output_stack_enter = std_output_stack_enter;
	ctx->output_stack_allocate = std_output_stack_allocate;
	ctx->output_stack_assign_int = std_output_stack_assign_int;
	ctx->output_stack_leave = std_output_stack_leave;
	ctx->output_statements_complete = std_output_statements_complete;

	ctx->read_file = std_read_file;
	ctx->write_file = std_write_file;

	ctx->output_header = std_output_header;
	ctx->output_os_return = std_output_os_return;

	ctx->to_string = std_to_string;

	size = sizeof(ecc_std_context);
	ctx->data = heap_malloc(size);
	if (!ctx->data) {
		err_msg("could not allocate context data\n");
		die();
	}
	data = (ecc_std_context *) ctx->data;

	data->source_file = source_file;
	data->buf[0] = '\0';
	data->buf_size = 0;
	data->buf_pos = 0;
	data->next_token[0] = '\0';
	data->out_file = out_file;
	data->bytes_written = 0;

	names_stack = stack_new();
	data->names_stack = names_stack;

	return ctx;
}

void free_std_context(ecc_context *ctx)
{
	ecc_std_context *this;

	this = _std_context(ctx);
	stack_destroy(this->names_stack);
	this->names_stack = 0;
	heap_free(ctx->data);
	heap_free(ctx);
}
