#include "lex.h"
#include "c_lib.h"
#include "misc.h"

int _is_operator(char c);
int _is_statement_terminator(char c);

int lex_get_number(const char *str, unsigned int max_len, unsigned int *len)
{
	unsigned int i;
	unsigned int found;
	unsigned int negative;
	int result;

	result = 0;
	found = 0;
	negative = 0;
	for (i = 0; i < max_len; i++) {
		if (!found && is_whitespace(str[i])) {
			continue;
		}
		if (!found && !negative && str[i] == '-') {
			negative = 1;
		} else if (is_number(str[i])) {
			found = 1;
			result = (result * 10) + (str[i] - '0');
		} else {
			break;
		}

	}
	(*len) += i;

	if (!found) {
		err_msg("lex_get_number unable to parse '");
		err_msg(str);
		err_msg("'\n");
		die();
	}
	if (negative) {
		result = -result;
	}

	return result;
}

void lex_look_ahead(const char *input, unsigned int input_len,
		    unsigned int *pos, char *output, unsigned int output_size)
{
	unsigned int i;
	unsigned int end;
	char last;
	char next;

	i = 0;
	end = *pos + input_len;
	last = '\0';
	next = '\0';

	while (is_whitespace(input[*pos])) {
		*pos += 1;
		if (*pos >= input_len - 1) {
			break;
		}
	}

	output[0] = '\0';
	while (input[*pos] && (*pos + i) < end) {
		last = next;
		next = input[*pos + i];
		if (is_whitespace(next)) {
			break;
		}
		if (i > 0) {
			if (is_alpha(last) && !is_alpha(next)) {
				break;
			}
			if (is_number(last) && !is_number(next)) {
				break;
			}
			if (_is_statement_terminator(next)) {
				break;
			}
			if (_is_operator(last)) {
				break;
			}
		}
		if (i >= output_size - 1) {
			err_msg("not enough room for next token: '");
			err_msg(&(input[*pos]));
			err_msg("'\n");
			die();
		}
		output[i] = next;
		output[i + 1] = '\0';
		i++;
	}
}

void lex_look_ahead_n(const char *input, unsigned int input_len,
		      unsigned int *pos, unsigned int num_ahead,
		      char *output, unsigned int output_size)
{
	unsigned int i;

	output[0] = '\0';
	for (i = 0; i < num_ahead; i++) {
		*pos += str_nlen(output, output_size);
		lex_look_ahead(input, input_len, pos, output, output_size);
	}
}

int _is_operator(char c)
{
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '('
	    || c == ')' || c == '=';
}

int _is_statement_terminator(char c)
{
	return c == ';';
}
