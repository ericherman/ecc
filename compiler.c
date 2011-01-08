#include "compiler.h"
#include "misc.h"

#define TOKEN_MAX 1000

int is_add_op(const char c);
int is_multiply_op(const char c);

void statement(context_t * ctx)
{
	expression(ctx);
}

void expression(context_t * ctx)
{
	const char *token;
	char previous_token[TOKEN_MAX];

	token = ctx->lex_look_ahead(ctx);
	if (token[0] == '-') {
		/* okay, we have a "-(foo)" situation */
		/* let's just slip-stream a zero. */
		ctx->output_term(ctx, 0);
	} else {
		term(ctx);
	}
	while (1) {
		token = ctx->lex_look_ahead(ctx);
		if (!is_add_op(token[0])) {
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
		expression(ctx);
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
		if (!is_multiply_op(token[0])) {
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

int is_add_op(const char c)
{
	return c == '+' || c == '-';
}

int is_multiply_op(const char c)
{
	return c == '*' || c == '/';
}

void compile(context_t * ctx)
{
	ctx->read_line(ctx);
	compile_inner(ctx);
	ctx->write_file(ctx);
}

void compile_inner(context_t * ctx)
{
	const char *token;

	ctx->output_header(ctx);

	while (1) {
		token = ctx->lex_look_ahead(ctx);
		if (token[0] == '\0') {
			break;
		}
		statement(ctx);
	}

	ctx->output_statements_complete(ctx);
	ctx->output_os_return(ctx);
}
