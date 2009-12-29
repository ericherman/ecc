#include "lex.h"
#include "c_lib.h"
#include "misc.h"

int lex_get_number(const char * str, unsigned int max_len, unsigned int *len) {
	unsigned int i, found, negative;
	int result;

	result = 0;
	found = 0;
	negative = 0;
	for(i = 0; i < max_len; i++) {
		if (!found && is_whitespace(str[i])) {
			continue;
		}
		if (!found && !negative && str[i] == '-') {
			negative = 1;
		} else if (is_number(str[i])) {
			found = 1;
			result = result * 10 + (str[i] - '0');
		} else {
			break;
		}

	}
	(*len) = i;

	if ( !found ) {
		err_msg("unable to parse '");
		err_msg(str);
		err_msg("'\n");
	}
	if (negative) {
		result = -result;
	}

	return result;
}

