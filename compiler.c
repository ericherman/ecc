/* compiler.c
   Copyright (C) 2009, 2010, 2011, 2012, 2016, 2018 Eric Herman <eric@freesa.org>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

	https://www.gnu.org/licenses/gpl-3.0.txt
 */
#include "compiler.h"
#include "compiler_inner.h"
#include "misc.h"
#include "c_lib.h"

#define TOKEN_MAX 1000

int is_add_precedence_op(const char *token);
int is_multiply_precedence_op(const char *token);
int is_declaration(const char *token);
int is_valid_name(const char *token);
void done_declaring(ecc_context *ctx);

void statement(ecc_context *ctx)
{
	const char *token;

	token = ctx->lex_look_ahead(ctx);
	if (is_declaration(token)) {
		declaration(ctx, token);
		return;
	}
	if (is_alpha(token[0])) {
		assignment(ctx, token);
		return;
	}
	expression(ctx, token);
}

void declaration(ecc_context *ctx, const char *token)
{
	ctx->lex_advance(ctx, str_nlen(token, TOKEN_MAX));
	token = ctx->lex_look_ahead(ctx);
	if (!(is_valid_name(token))) {
		err_msg("token '");
		err_msg(token);
		err_msg("' not a valid name\n");
		die();
	}
	ctx->stack_assign_name(ctx, token);
	token = ctx->lex_look_ahead(ctx);
	if (!(is_declaration(token))) {
		done_declaring(ctx);
	}
}

void assignment(ecc_context *ctx, const char *variable)
{
	const char *token;
	unsigned int var_pos;
	int number;

	/* advance past variable */
	ctx->lex_advance(ctx, str_nlen(variable, TOKEN_MAX));

	token = ctx->lex_look_ahead(ctx);
	if (token[0] != '=') {
		err_msg("token '");
		err_msg(token);
		err_msg("' is not an '='\n");
		die();
	}
	ctx->lex_advance(ctx, str_nlen(token, TOKEN_MAX));

	token = ctx->lex_look_ahead(ctx);
	if (is_number(token[0])) {
		number = a_to_i(token, str_nlen(token, TOKEN_MAX));
		ctx->lex_advance(ctx, str_nlen(token, TOKEN_MAX));
		var_pos = ctx->stack_name_pos(ctx, variable);
		ctx->output_stack_assign_int(ctx, var_pos, number);
	} else {
		err_msg("not supported\n");
		die();
	}
}

void expression(ecc_context *ctx, const char *token)
{
	char previous_token[TOKEN_MAX];

	if (token[0] == '-') {
		/* okay, we have a "-(foo)" situation */
		/* let's just slip-stream a zero. */
		ctx->output_term(ctx, 0);
	} else {
		term(ctx, token);
	}
	while (1) {
		token = ctx->lex_look_ahead(ctx);
		if (!is_add_precedence_op(token)) {
			break;
		}

		str_ncpy(previous_token, token, TOKEN_MAX);
		ctx->lex_advance(ctx, 1);
		token = ctx->lex_look_ahead(ctx);

		term(ctx, token);

		switch (previous_token[0]) {
		case '+':
			ctx->output_add(ctx);
			break;
		case '-':
			ctx->output_subtract(ctx);
		}
	}
}

void factor(ecc_context *ctx, const char *token)
{
	int number;
	unsigned int negate;

	if (token[0] == '(') {
		ctx->lex_advance(ctx, 1);
		token = ctx->lex_look_ahead(ctx);
		expression(ctx, token);
		/* eat close paren */
		ctx->lex_advance(ctx, 1);
	} else {
		if ((token[0] == '-') && (str_nlen(token, TOKEN_MAX) == 1)) {
			negate = 1;
			ctx->lex_advance(ctx, 1);
			token = ctx->lex_look_ahead(ctx);
		} else {
			negate = 0;
		}
		number = a_to_i(token, str_nlen(token, TOKEN_MAX));
		if (negate) {
			number = -number;
		}
		ctx->lex_advance(ctx, str_nlen(token, TOKEN_MAX));
		ctx->output_term(ctx, number);
	}
}

void term(ecc_context *ctx, const char *token)
{
	char previous_token[TOKEN_MAX];

	factor(ctx, token);
	while (1) {
		token = ctx->lex_look_ahead(ctx);
		if (!is_multiply_precedence_op(token)) {
			break;
		}

		str_ncpy(previous_token, token, TOKEN_MAX);
		ctx->lex_advance(ctx, 1);
		token = ctx->lex_look_ahead(ctx);

		factor(ctx, token);

		switch (previous_token[0]) {
		case '*':
			ctx->output_multiply(ctx);
			break;
		case '/':
			ctx->output_divide(ctx);
		}
	}
}

void done_declaring(ecc_context *ctx)
{
	unsigned int frame_size;
	unsigned int bytes;
	unsigned int variable_size;

	variable_size = 4;
	frame_size = ctx->stack_frame_size(ctx);
	if (frame_size == 0) {
		return;
	}
	bytes = variable_size * frame_size;

	ctx->output_stack_allocate(ctx, bytes);
}

int is_add_precedence_op(const char *token)
{
	char c;

	c = token[0];
	return c == '+' || c == '-';
}

int is_multiply_precedence_op(const char *token)
{
	char c;

	c = token[0];
	return c == '*' || c == '/';
}

int is_declaration(const char *token)
{
	return 0 == str_ncmp("int", 4, token, TOKEN_MAX);
}

int is_valid_name(const char *token)
{
	char c;

	c = token[0];
	return c == '_' || is_alpha(c);
}

void compile(ecc_context *ctx)
{
	ctx->read_file(ctx);
	compile_inner(ctx);
	ctx->write_file(ctx);
}

void compile_inner(ecc_context *ctx)
{
	const char *token;

	ctx->output_header(ctx);

	ctx->stack_enter(ctx);
	ctx->output_stack_enter(ctx);
	while (1) {
		token = ctx->lex_look_ahead(ctx);
		if (token[0] == '\0') {
			break;
		}

		statement(ctx);

		token = ctx->lex_look_ahead(ctx);
		if (token[0] == ';') {
			/* eat the ';' token */
			ctx->lex_advance(ctx, 1);
		}
	}

	ctx->output_statements_complete(ctx);
	ctx->stack_leave(ctx);
	ctx->output_stack_leave(ctx);
	ctx->output_os_return(ctx);
}
