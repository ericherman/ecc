#include "misc.h"
#include "c_lib.h"

int is_whitespace(char c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_number(char c) {
	return c >= '0' && c <= '9';
}

int is_alpha(char c) {
	return ( c >= 'A' && c <= 'Z' )
	    || ( c >= 'a' && c <= 'z' );
}

void str_cpy_offset(const char *src, char *buf, unsigned int buf_size,
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

char *str_ncpy(char *dest, const char *src, unsigned int limit) {
	unsigned int pos;

	for(pos = 0; pos < (limit - 1); pos++) {
		if (src[pos] == '\0') {
			break;
		}
		dest[pos] = src[pos];
	}
	dest[pos] = '\0';
	return dest + pos;
}

unsigned int str_len(const char *s) {
	unsigned int i = 0;
	while (1) {
		if (s[i] == '\0') {
			break;
		}
		i++;
		if (i == 0) {
			break;
		}
	}

	return i;
}
