#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "compiler.h"
#include "mock_context.h"

void test_term_simple() {
	const char *tokens[] = { "23" };
	context_t * ctx = init_fake_context(tokens, 1);
	mock_data * data = (mock_data *) ctx->data;
	char buf[1000];

	term(ctx);

	ctx->to_string(ctx->data, buf, sizeof(buf));

	check_unsigned_ints(data->calls, 4, buf);
	check_strs(data->call[0], "lex_look_ahead", buf);
	check_strs(data->call[1], "lex_get_number", buf);
	check_strs(data->call[2], "output_term", buf);
	check_strs(data->call[3], "lex_look_ahead", buf);

	free_fake_context(ctx);
}

void check_expression_add_subtract(const char ** tokens, unsigned int count,
		const char * op) {

	context_t * ctx = init_fake_context(tokens, count);
	char buf[10000];
	mock_data * data = (mock_data *) ctx->data;

	expression(ctx);

	ctx->to_string(ctx->data, buf, sizeof(buf));

	check_unsigned_ints(data->calls, 12, buf);
	/* term */
        check_strs(data->call[0], "lex_look_ahead", buf);
	check_strs(data->call[1], "lex_get_number", buf);
	check_strs(data->call[2], "output_term", buf);
	check_strs(data->call[3], "lex_look_ahead", buf);
	/* is_add_op */
	check_strs(data->call[4], "lex_look_ahead", buf);
	/* yes, so advance */
	check_strs(data->call[5], "lex_advance", buf);
	/* term again */
	check_strs(data->call[6], "lex_look_ahead", buf);
	check_strs(data->call[7], "lex_get_number", buf);
	check_strs(data->call[8], "output_term", buf);
	check_strs(data->call[9], "lex_look_ahead", buf);
	/* add/subtract them */
	check_strs(data->call[10], op, buf);
	/* is_add_op */
	check_strs(data->call[11], "lex_look_ahead", buf);
	/* no, so finish */

	free_fake_context(ctx);
}

void test_expression_add() {
	const char *tokens[] = { "3", "+", "5" };
	check_expression_add_subtract(tokens, 3, "output_add");
}

void test_expression_subtract() {
	const char *tokens[] = { "2", "-", "1" };
	check_expression_add_subtract(tokens, 3, "output_subtract");
}

void test_factor() {
	const char *tokensv[] = { "2" };
	unsigned int tokensc = 1;
	context_t * ctx = init_fake_context(tokensv, tokensc);
	char buf[10000];
	mock_data * data = (mock_data *) ctx->data;

	factor(ctx);

	ctx->to_string(ctx->data, buf, sizeof(buf));

	check_unsigned_ints(data->calls, 4, buf);
	check_strs(data->call[0], "lex_look_ahead", buf);
	check_strs(data->call[1], "lex_get_number", buf);
	check_strs(data->call[2], "output_term", buf);
	check_strs(data->call[3], "lex_look_ahead", buf);

	free_fake_context(ctx);

}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		printf("%s takes no arguments\n", argv[0]);
		return 1;
	}
	test_term_simple();
	test_expression_add();
	test_expression_subtract();
	test_factor();

	return 0;
}
