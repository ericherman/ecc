#include "misc.h"
#include "c_lib.h"

int is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_number(char c) {
	return c >= '0' && c <= '9';
}

/*
int is_alpha(char c) {
	return ( c >= 'A' && c <= 'Z' )
	    || ( c >= 'a' && c <= 'z' )
	    || c == '_';
}
*/

void str_cpy(const char *src, char *buf, unsigned int buf_size,
		unsigned int *buf_pos) {

	unsigned int len, max;
	char *dest;

	dest = &buf[*buf_pos];
	max = (buf_size - *buf_pos);

	len = str_len(dest);
	if (len > max) {
		len = max;
	}
	str_ncpy(dest, src, max);
	dest[len -1] = '\0';
	*buf_pos += len;
}
