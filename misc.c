#include "misc.h"
#include "c_lib.h"

int is_whitespace(char c)
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int is_number(char c)
{
	return c >= '0' && c <= '9';
}

int is_alpha(char c)
{
	return (c >= 'A' && c <= 'Z')
	    || (c >= 'a' && c <= 'z');
}

void str_cpy_offset(const char *src, char *buf, unsigned int buf_size,
		    unsigned int *buf_pos)
{

	unsigned int len;
	unsigned int max;
	char *dest;

	dest = &buf[*buf_pos];
	max = (buf_size - *buf_pos);

	len = str_nlen(dest, max);
	if (len > max) {
		len = max;
	}
	str_ncpy(dest, src, max);
	dest[len - 1] = '\0';
	*buf_pos += len;
}

char *str_ncpy(char *dest, const char *src, unsigned int limit)
{
	unsigned int pos;

	for (pos = 0; pos < (limit - 1); pos++) {
		if (src[pos] == '\0') {
			break;
		}
		dest[pos] = src[pos];
	}

	do {
		dest[pos] = '\0';
	} while (++pos < limit);

	return dest;
}

unsigned int str_nlen(const char *s, unsigned int limit)
{
	unsigned int i;

	i = 0;
	while (limit-- > 0) {
		if (s[i] == '\0') {
			break;
		}
		i++;
	}

	return i;
}

int str_ncmp(const char *left, unsigned int left_size,
	     const char *right, unsigned int right_size)
{
	int diff;
	unsigned int i;

	diff = 0;
	i = 0;

	while (i < left_size && i < right_size) {
		diff = left[i] - right[i];
		if (diff != 0) {
			return diff;
		}
		if (left[i] == '\0') {
			return 0;
		}
		i++;
	}
	return left_size - right_size;
}

void write_int(unsigned char *buf, int value)
{
	buf[0] = 0xFF & value;
	buf[1] = 0xFF & (value >> 8);
	buf[2] = 0xFF & (value >> 16);
	buf[3] = 0xFF & (value >> 24);
}

void write_bytes(const char *name,
		 unsigned char *buf, unsigned int buf_size,
		 unsigned int *bytes_written,
		 const unsigned char *to_write, unsigned int count)
{
	unsigned int i;

	if (buf_size < (*bytes_written) + count) {
		err_msg("buf_size too small for ");
		err_msg(name);
		err_msg("\n");
		die();
	}
	for (i = 0; i < count; i++) {
		buf[(*bytes_written)++] = to_write[i];
	}
}
