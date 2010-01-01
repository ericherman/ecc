#include "compiler.h"


int is_add_op(const char c) {
	return c == '+' || c == '-';
}

void expression(context_t * ctx) {
	char c;
	term(ctx);

	while (is_add_op(c = (ctx)->lex_look_ahead( (ctx)->data ))) {

		(ctx)->lex_advance( (ctx)->data, 1 );

		term(ctx);

		switch (c) {
		case '+':
			(ctx)->output_add( (ctx)->data );
			break;
		case '-':
			(ctx)->output_subtract( (ctx)->data );
		}
	}
}

void term(context_t * ctx) {
	int number;

	number = (ctx)->lex_get_number( (ctx)->data );
	(ctx)->output_term( (ctx)->data, number );
}

void compile(context_t * ctx) {
	(ctx)->read_line( (ctx)->data );
	compile_inner(ctx);
	(ctx)->write_file( (ctx)->data );
}

void compile_inner(context_t * ctx) {
	(ctx)->output_header( (ctx)->data );

	expression(ctx);

	(ctx)->output_statements_complete( (ctx)->data );
	(ctx)->output_os_return( (ctx)->data );
}
