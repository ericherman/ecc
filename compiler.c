#include "compiler.h"

int is_add_op(const char c);
int is_multiply_op(const char c);


void expression(context_t * ctx) {
	char c;

	if ( is_add_op(ctx->lex_look_ahead(ctx->data)) ) {
		/* okay, we have a "-(foo)" situation */
		/* let's just slip-stream a zero. */
		ctx->output_term(ctx->data, 0);
	} else {
		term(ctx);
	}
	while (is_add_op(c = ctx->lex_look_ahead(ctx->data))) {

		ctx->lex_advance( ctx->data, 1 );

		term(ctx);

		switch (c) {
		case '+':
			ctx->output_add(ctx->data);
			break;
		case '-':
			ctx->output_subtract(ctx->data);
		}
	}
}

void factor(context_t * ctx) {
	char c;
	int number;

	c = ctx->lex_look_ahead(ctx->data);
	if (c == '(') {
		ctx->lex_advance( ctx->data, 1 );
		expression(ctx);
		/* eat close paren */
		ctx->lex_advance( ctx->data, 1 );
	} else {
		number = ctx->lex_get_number(ctx->data);
		ctx->output_term( ctx->data, number );
	}
}

void term(context_t * ctx) {
	char c;
	factor(ctx);
	while( is_multiply_op( c = ctx->lex_look_ahead(ctx->data) ) ) {

		ctx->lex_advance( ctx->data, 1 );

		factor(ctx);

		switch (c) {
		case '*':
			ctx->output_multiply(ctx->data);
			break;
		case '/':
			ctx->output_divide(ctx->data);
		}
	}
}

int is_add_op(const char c) {
	return c == '+' || c == '-';
}

int is_multiply_op(const char c) {
	return c == '*' || c == '/';
}

void compile(context_t * ctx) {
	ctx->read_line(ctx->data);
	compile_inner(ctx);
	ctx->write_file(ctx->data);
}

void compile_inner(context_t * ctx) {
	ctx->output_header(ctx->data);

	expression(ctx);

	ctx->output_statements_complete(ctx->data);
	ctx->output_os_return(ctx->data);
}
