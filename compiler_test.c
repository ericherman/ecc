#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "compiler.h"
#include "mock_context.h"


void check_expected_calls(context_t * ctx, const char * test_name,
		const char ** expected_calls, unsigned int count) {

	mock_data * data = (mock_data *) ctx->data;
	unsigned int i, len;
	char buf[10000];

	sprintf(buf, "%s:\nexpected_calls {\n", test_name);
	for(i = 0; i < count; i++) {
		len = strlen(buf);
		sprintf(&buf[len], "%d:\t%s\n", i, expected_calls[i]);
	}
	len = strlen(buf);
	sprintf(&buf[len], "}\n");
	len = strlen(buf);
	ctx->to_string(ctx->data, &buf[len], sizeof(buf)-len);

	check_unsigned_ints(data->calls, count, buf);
	for (i = 0; i < count; i++) {
		check_strs(data->call[i], expected_calls[i], buf);
	}
}


void test_term_simple() {
	const char *tokensv[] = { "23" };
	unsigned int tokensc = 1;
	const char *expected_calls[] = {
		/* call factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		/* look ahead for multiply */
		"lex_look_ahead",
	};
	unsigned int count = 5;

	context_t * ctx = init_fake_context(tokensv, tokensc);

	term(ctx);

	check_expected_calls(ctx, "test_term_simple", expected_calls, count);

	free_fake_context(ctx);
}

void check_expression_add_subtract(const char * test_name,
		const char ** tokensv, unsigned int tokensc,
		const char * op) {

	const char *expected_calls[] = {
		/* term */
		/* call factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		/* term look ahead for multiply */
		"lex_look_ahead",
		/* is_add_op */
		"lex_look_ahead",
		/* yes, so advance */
		"lex_advance",
		/* term again */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		"lex_look_ahead",
		/* add/subtract them */
		"ADD_SUBTRACT_DUMMY",
		/* is_add_op */
		"lex_look_ahead",
		/* no, so finish */
	};
	unsigned int count = 14;
	context_t * ctx = init_fake_context(tokensv, tokensc);

	expected_calls[12] = op;

	expression(ctx);

	check_expected_calls(ctx, test_name, expected_calls, count);
	free_fake_context(ctx);
}

void test_expression_add() {
	const char *tokens[] = { "3", "+", "5" };
	check_expression_add_subtract("test_expression_add",
			tokens, 3, "output_add");
}

void test_expression_subtract() {
	const char *tokens[] = { "2", "-", "1" };
	check_expression_add_subtract("test_expression_subtract",
			tokens, 3, "output_subtract");
}

void test_three_add_op_expr() {
	const char *tokensv[] = { "16", "+", "4", "-", "3" };
	unsigned int tokensc = 5;

	const char *expected_calls[] = {
		/* term calls factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		/* is_mult_op */
		"lex_look_ahead",
		/* no, is add op? */
		"lex_look_ahead",
		/* yes */
		"lex_advance",
		/* load next term */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		"lex_look_ahead",
		/* output add */
		"output_add",
		/* is add op? */
		"lex_look_ahead",
		/* yes */
		"lex_advance",
		/* load next term */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		"lex_look_ahead",
		/* output subtract */
		"output_subtract",
		/* is add op? */
		"lex_look_ahead",
		/* no, done. */
	};
	unsigned int count = 22;
	context_t * ctx = init_fake_context(tokensv, tokensc);

	expression(ctx);

	check_expected_calls(ctx, "three_add_op_expr", expected_calls, count);

	free_fake_context(ctx);

}

void test_factor() {
	const char *tokensv[] = { "2" };
	unsigned int tokensc = 1;
	const char *expected_calls[] = {
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
	};
	unsigned int count = 4;

	context_t * ctx = init_fake_context(tokensv, tokensc);

	factor(ctx);

	check_expected_calls(ctx, "test_factor", expected_calls, count);

	free_fake_context(ctx);
}

void check_two_factor_term(const char * test_name,
		const char ** tokensv, unsigned int tokensc,
		const char * op) {

	const char *expected_calls[] = {
		/* term calls factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		/* is_mult_op */
		"lex_look_ahead",
		/* yes, advance */
		"lex_advance",
		/* load next factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		/* output the multiply */
		"MULTIPLY_DIVIDE_DUMMY",
		/* is_mulitply_op? */
		"lex_look_ahead",
		/* no, so finish */
	};
	unsigned int count = 12;
	context_t * ctx = init_fake_context(tokensv, tokensc);

	expected_calls[10] = op;

	term(ctx);

	check_expected_calls(ctx, test_name, expected_calls, count);

	free_fake_context(ctx);
}

void test_mul_two_factor_term() {
	const char *tokensv[] = { "2", "*", "3" };
	check_two_factor_term("mul_two_factor_term", tokensv, 3,
			"output_multiply");
}

void test_div_two_factor_term() {
	const char *tokensv[] = { "2", "/", "3" };
	check_two_factor_term("div_two_factor_term", tokensv, 3,
			"output_divide");
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
	test_mul_two_factor_term();
	test_div_two_factor_term();

	return 0;
}
