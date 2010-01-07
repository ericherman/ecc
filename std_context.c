#include "std_context.h"
#include "std_context_t.h"
#include "c_lib.h"
#include "lex.h"
#include "x86_asm.h"
#include "elf_header.h"
#include "misc.h"

void std_to_string(void *data, char *buf, unsigned int buf_size) {
	std_context_t *this = (std_context_t *) data;
	unsigned int buf_pos = 0;
	str_cpy("std_context_t {\n", buf, buf_size, &buf_pos);
	str_cpy("    source_file: ", buf, buf_size, &buf_pos);
	str_cpy(this->source_file, buf, buf_size, &buf_pos);
	str_cpy("\n", buf, buf_size, &buf_pos);
	str_cpy("    buf: ", buf, buf_size, &buf_pos);
	str_cpy(this->buf, buf, buf_size, &buf_pos);
	str_cpy("\n", buf, buf_size, &buf_pos);
	str_cpy("\n", buf, buf_size, &buf_pos);
	str_cpy("}\n", buf, buf_size, &buf_pos);
}

char std_lex_look_ahead(void *data) {
	std_context_t *this = (std_context_t *) data;
	return lex_look_ahead(this->buf, this->buf_size, &(this->buf_pos));
}

void std_lex_advance(void *data, unsigned int chars) {
	std_context_t *this = (std_context_t *) data;
	this->buf_pos += chars;
}

int std_lex_get_number(void *data) {
	std_context_t *this = (std_context_t *) data;
	const char *sub_str;
	unsigned int sub_str_size;

	sub_str = &(this->buf[this->buf_pos]);
	sub_str_size = this->buf_size - this->buf_pos;

	return lex_get_number(sub_str, sub_str_size, &(this->buf_pos));
}

void std_output_term(void *data, int number) {
	std_context_t *this = (std_context_t *) data;
	output_term(number, this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_add(void *data) {
	std_context_t *this = (std_context_t *) data;
	output_add(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_subtract(void *data) {
	std_context_t *this = (std_context_t *) data;
	output_subtract(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_multiply(void *data) {
	std_context_t *this = (std_context_t *) data;
	output_multiply(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_divide(void *data) {
	std_context_t *this = (std_context_t *) data;
	output_divide(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_statements_complete(void *data) {
	std_context_t *this = (std_context_t *) data;
	output_statements_complete(this->byte_buf, BBUF_MAX,
			&(this->bytes_written));
}

void std_read_line(void *data) {
	std_context_t *this = (std_context_t *) data;
	read_line(this->source_file, this->buf, LINE_MAX, &(this->buf_size));
	this->buf_size += 1;
}

void std_write_file(void *data) {
	std_context_t *this = (std_context_t *) data;
	write_file(this->executable, this->byte_buf, this->bytes_written);
}

void std_output_header(void *data) {
	std_context_t *this = (std_context_t *) data;
	output_header(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}

void std_output_os_return(void *data) {
	std_context_t *this = (std_context_t *) data;
	output_os_return(this->byte_buf, BBUF_MAX, &(this->bytes_written));
}


context_t *alloc_std_context(const char *source_file, const char *executable) {

	context_t *ctx;
	unsigned int size;
	std_context_t *data;

	size = sizeof(struct context_t_);

	ctx = heap_malloc(size);
	if (!ctx) {
		err_msg("could not allocate context_t\n");
		return ctx;
	}

	ctx->lex_look_ahead = std_lex_look_ahead;
	ctx->lex_advance = std_lex_advance;
	ctx->lex_get_number = std_lex_get_number;

	ctx->output_term = std_output_term;
	ctx->output_add = std_output_add;
	ctx->output_subtract = std_output_subtract;
	ctx->output_multiply = std_output_multiply;
	ctx->output_divide = std_output_divide;
	ctx->output_statements_complete = std_output_statements_complete;

	ctx->read_line = std_read_line;
	ctx->write_file = std_write_file;

	ctx->output_header = std_output_header;
	ctx->output_os_return = std_output_os_return;

	ctx->to_string = std_to_string;

	size = sizeof(struct std_context_t_);
	ctx->data = heap_malloc(size);
	if (!ctx->data) {
		err_msg("could not allocate context data\n");
		return ctx;
	}
	data = (std_context_t *) ctx->data;

	data->source_file = source_file;
	data->buf[0] = '\0';
	data->buf_size = 0;
	data->buf_pos = 0;
	data->executable = executable;
	data->bytes_written = 0;

	return ctx;
}

void free_std_context(context_t *ctx) {
	heap_free(ctx->data);
	heap_free(ctx);
}
