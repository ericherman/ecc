#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "test_util.h"
#include "compiler.h"
#include "mock_context.h"

void check_expected_calls(context_t * ctx, const char *test_name,
			  const char **expected_calls, unsigned int count)
{

	mock_data *data = (mock_data *) ctx->data;
	unsigned int i, len;
	char buf[10000];

	sprintf(buf, "%s:\nexpected_calls {\n", test_name);
	for (i = 0; i < count; i++) {
		len = strlen(buf);
		sprintf(&buf[len], "%d:\t%s\n", i, expected_calls[i]);
	}
	len = strlen(buf);
	sprintf(&buf[len], "}\n");
	len = strlen(buf);
	ctx->to_string(ctx, &buf[len], sizeof(buf) - len);

	check_unsigned_ints(data->calls, count, buf);
	for (i = 0; i < count; i++) {
		check_strs(data->call[i], expected_calls[i], buf);
	}
}

void check_expected_terms(context_t * ctx, const char *test_name,
			  int *expected_terms, unsigned int count)
{

	mock_data *data = (mock_data *) ctx->data;
	unsigned int i, len;
	char buf[10000];

	sprintf(buf, "%s:\nexpected_terms {\n", test_name);
	for (i = 0; i < count; i++) {
		len = strlen(buf);
		sprintf(&buf[len], "%d:\t%d\n", i, expected_terms[i]);
	}
	len = strlen(buf);
	sprintf(&buf[len], "}\n");
	len = strlen(buf);
	ctx->to_string(ctx, &buf[len], sizeof(buf) - len);

	check_unsigned_ints(data->terms, count, buf);
	for (i = 0; i < count; i++) {
		check_ints(data->term[i], expected_terms[i], buf);
	}
}

void test_term_simple()
{
	const char *tokensv[] = { "23" };
	unsigned int tokensc = 1;
	const char *expected_calls[] = {
		/* call factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		/* look ahead for multiply */
		"lex_look_ahead",
	};
	unsigned int count = 4;
	context_t *ctx;

	ctx = init_fake_context(tokensv, tokensc);

	term(ctx);

	check_expected_calls(ctx, "test_term_simple", expected_calls, count);

	free_fake_context(ctx);
}

void check_expression_add_subtract(const char *test_name,
				   const char **tokensv, unsigned int tokensc,
				   const char *op)
{

	const char *expected_calls[] = {
		"lex_look_ahead",
		/* term */
		/* call factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
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
		/* add/subtract them */
		"ADD_SUBTRACT_DUMMY",
		/* is_add_op */
		"lex_look_ahead",
		/* no, so finish */
	};
	unsigned int count = 13;
	context_t *ctx = init_fake_context(tokensv, tokensc);

	expected_calls[11] = op;

	expression(ctx);

	check_expected_calls(ctx, test_name, expected_calls, count);
	free_fake_context(ctx);
}

void test_expression_add()
{
	const char *tokens[] = { "3", "+", "5" };
	check_expression_add_subtract("test_expression_add",
				      tokens, 3, "output_add");
}

void test_expression_subtract()
{
	const char *tokens[] = { "2", "-", "1" };
	check_expression_add_subtract("test_expression_subtract",
				      tokens, 3, "output_subtract");
}

void test_three_add_op_expr()
{
	const char *tokensv[] = { "16", "+", "4", "-", "3" };
	unsigned int tokensc = 5;

	const char *expected_calls[] = {
		/* term calls factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
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
		/* output subtract */
		"output_subtract",
		/* is add op? */
		"lex_look_ahead",
		/* no, done. */
	};
	unsigned int count = 19;
	context_t *ctx = init_fake_context(tokensv, tokensc);

	expression(ctx);

	check_expected_calls(ctx, "three_add_op_expr", expected_calls, count);

	free_fake_context(ctx);

}

void test_factor()
{
	const char *tokensv[] = { "2" };
	unsigned int tokensc = 1;
	const char *expected_calls[] = {
		/* is paren? */
		"lex_look_ahead",
		/* no */
		"lex_get_number",
		"output_term",
	};
	unsigned int count = 3;

	context_t *ctx = init_fake_context(tokensv, tokensc);

	factor(ctx);

	check_expected_calls(ctx, "test_factor", expected_calls, count);

	free_fake_context(ctx);
}

void check_two_factor_term(const char *test_name,
			   const char **tokensv, unsigned int tokensc,
			   const char *op)
{

	const char *expected_calls[] = {
		/* term calls factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		/* is_mult_op */
		"lex_look_ahead",
		/* yes, advance */
		"lex_advance",
		/* load next factor */
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		/* output the multiply */
		"MULTIPLY_DIVIDE_DUMMY",
		/* is_mulitply_op? */
		"lex_look_ahead",
		/* no, so finish */
	};
	unsigned int count = 10;
	context_t *ctx = init_fake_context(tokensv, tokensc);

	expected_calls[8] = op;

	term(ctx);

	check_expected_calls(ctx, test_name, expected_calls, count);

	free_fake_context(ctx);
}

void test_mul_two_factor_term()
{
	const char *tokensv[] = { "2", "*", "3" };
	check_two_factor_term("mul_two_factor_term", tokensv, 3,
			      "output_multiply");
}

void test_div_two_factor_term()
{
	const char *tokensv[] = { "2", "/", "3" };
	check_two_factor_term("div_two_factor_term", tokensv, 3,
			      "output_divide");
}

void test_paren_factor()
{
	const char *tokensv[] = { "(", "2", ")" };
	unsigned int tokensc = 3;

	const char *expected_calls[] = {
		/* is paren? */
		"lex_look_ahead",
		/* yes, eat open paren (advance) */
		"lex_advance",
		/* call expression */
		"lex_look_ahead",
		/*   call term */
		/*     call factor */
		/*       is paren? */
		"lex_look_ahead",
		/*       no */
		"lex_get_number",
		"output_term",
		/*   is_mul_op? */
		"lex_look_ahead",
		/* is_add_op? */
		"lex_look_ahead",
		/* no. exit expression */
		/* back in factor, eat close paren */
		"lex_advance",
		/* exit factor */
	};
	unsigned int count = 9;
	context_t *ctx = init_fake_context(tokensv, tokensc);

	factor(ctx);

	check_expected_calls(ctx, "test_paren_factor", expected_calls, count);

	free_fake_context(ctx);
}

void test_negative_expr()
{
	const char *tokensv[] = { "-", "(", "23", ")" };
	unsigned int tokensc = 4;
	const char *expected_calls[] = {
		/* is add_op? */
		"lex_look_ahead",
		/* yes, output a 0 */
		"output_term",
		/* is add_op? */
		"lex_look_ahead",
		/* yes, eat leading minus */
		"lex_advance",
		/* is paren? */
		"lex_look_ahead",
		/* yes, eat paren */
		"lex_advance",
		/* call expression */
		/* is add op? */
		"lex_look_ahead",
		/* no call term */
		/* is paren? */
		"lex_look_ahead",
		/* no, output 23 */
		"lex_get_number",
		"output_term",
		/* is multiply? */
		"lex_look_ahead",
		/* no, is add_op? */
		"lex_look_ahead",
		/* no, exit expression */
		/* eat closing paren */
		"lex_advance",
		/* is mulitiply? */
		"lex_look_ahead",
		/* no, perform add_op */
		"output_subtract",
		/* is add_op? */
		"lex_look_ahead",
		/* no exit loop */
	};

	unsigned int count = 16;
	int expect_termsv[] = { 0, 23 };
	unsigned int termsc = 2;

	context_t *ctx = init_fake_context(tokensv, tokensc);

	expression(ctx);

	check_expected_calls(ctx, "test_negative_expr", expected_calls, count);
	check_expected_terms(ctx, "test_negative_expr", expect_termsv, termsc);

	free_fake_context(ctx);
}

void test_compile_inner()
{
	const char *tokensv[] = { "4" };
	unsigned int tokensc = 1;
	const char *expected_calls[] = {
		"output_header",
		"lex_look_ahead",
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		"lex_look_ahead",
		"output_statements_complete",
		"output_os_return",
	};

	unsigned int count = 9;
	int expect_termsv[] = { 4 };
	unsigned int termsc = 1;

	context_t *ctx = init_fake_context(tokensv, tokensc);

	compile_inner(ctx);

	check_expected_calls(ctx, "test_negative_expr", expected_calls, count);
	check_expected_terms(ctx, "test_negative_expr", expect_termsv, termsc);

	free_fake_context(ctx);
}

void test_compile()
{
	const char *tokensv[] = { "7" };
	unsigned int tokensc = 1;
	const char *expected_calls[] = {
		"read_line",
		"output_header",
		"lex_look_ahead",
		"lex_look_ahead",
		"lex_get_number",
		"output_term",
		"lex_look_ahead",
		"lex_look_ahead",
		"output_statements_complete",
		"output_os_return",
		"write_file"
	};

	unsigned int count = 11;
	int expect_termsv[] = { 7 };
	unsigned int termsc = 1;

	context_t *ctx = init_fake_context(tokensv, tokensc);

	compile(ctx);

	check_expected_calls(ctx, "test_negative_expr", expected_calls, count);
	check_expected_terms(ctx, "test_negative_expr", expect_termsv, termsc);

	free_fake_context(ctx);
}

int main(int argc, char *argv[])
{
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
	test_paren_factor();
	test_negative_expr();
	test_compile_inner();
	test_compile();

	return 0;
}
