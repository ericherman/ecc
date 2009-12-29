#include "misc.h"

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
