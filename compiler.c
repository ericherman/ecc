#include "compiler.h"
#include "compiler_inner.h"
#include "misc.h"
#include "c_lib.h"

#define TOKEN_MAX 1000

int is_add_precedence_op(const char *token);
int is_multiply_precedence_op(const char *token);
int is_declaration(const char *token);
void done_declaring(context_t * ctx);

void statement(context_t * ctx)
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

void declaration(context_t * ctx, const char *token)
{
	ctx->lex_advance(ctx, str_nlen(token, TOKEN_MAX));
	token = ctx->lex_get_name(ctx);
	ctx->stack_assign_name(ctx, token);
	token = ctx->lex_look_ahead(ctx);
	if (!(is_declaration(token))) {
		done_declaring(ctx);
	}
}

void assignment(context_t * ctx, const char *variable)
{
	const char *token;
	unsigned int var_pos;
	int number;

	/* advance past variable */
	ctx->lex_advance(ctx, str_nlen(variable, TOKEN_MAX));

	token = ctx->lex_look_ahead(ctx);
	if (token[0] != '=') {
		err_msg(token);
		die();
	}
	ctx->lex_advance(ctx, str_nlen(token, TOKEN_MAX));

	token = ctx->lex_look_ahead(ctx);
	if (is_number(token[0])) {
		number = ctx->lex_get_number(ctx);
		var_pos = ctx->stack_name_pos(ctx, variable);
		ctx->output_stack_assign_int(ctx, var_pos, number);
	} else {
		err_msg("not supported\n");
		die();
	}
}

void expression(context_t * ctx, const char *token)
{
	char previous_token[TOKEN_MAX];

	if (token[0] == '-') {
		/* okay, we have a "-(foo)" situation */
		/* let's just slip-stream a zero. */
		ctx->output_term(ctx, 0);
	} else {
		term(ctx);
	}
	while (1) {
		token = ctx->lex_look_ahead(ctx);
		if (!is_add_precedence_op(token)) {
			break;
		}

		str_ncpy(previous_token, token, TOKEN_MAX);
		ctx->lex_advance(ctx, 1);

		term(ctx);

		switch (previous_token[0]) {
		case '+':
			ctx->output_add(ctx);
			break;
		case '-':
			ctx->output_subtract(ctx);
		}
	}
}

void factor(context_t * ctx)
{
	const char *token;
	int number;

	token = ctx->lex_look_ahead(ctx);
	if (token[0] == '(') {
		ctx->lex_advance(ctx, 1);
		expression(ctx, token);
		/* eat close paren */
		ctx->lex_advance(ctx, 1);
	} else {
		number = ctx->lex_get_number(ctx);
		ctx->output_term(ctx, number);
	}
}

void term(context_t * ctx)
{
	const char *token;
	char previous_token[TOKEN_MAX];

	factor(ctx);
	while (1) {
		token = ctx->lex_look_ahead(ctx);
		if (!is_multiply_precedence_op(token)) {
			break;
		}

		str_ncpy(previous_token, token, TOKEN_MAX);
		ctx->lex_advance(ctx, 1);

		factor(ctx);

		switch (previous_token[0]) {
		case '*':
			ctx->output_multiply(ctx);
			break;
		case '/':
			ctx->output_divide(ctx);
		}
	}
}

void done_declaring(context_t * ctx)
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

void compile(context_t * ctx)
{
	ctx->read_file(ctx);
	compile_inner(ctx);
	ctx->write_file(ctx);
}

void compile_inner(context_t * ctx)
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
