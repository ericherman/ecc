#include "c_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void err_msg(const char * msg) {
	fprintf(stderr, msg);
}

void read_line(const char * source_file,
		char * line_buf, unsigned int buf_size,
		unsigned int * chars_read) {

	FILE * file = fopen(source_file, "r");
	if (file == NULL) {
		fprintf(stderr, "Error opening file %s", source_file);
		exit(1);
	}

	fgets(line_buf, buf_size, file);
	*chars_read = strlen(line_buf);
	fclose(file);
}

void write_file(const char * executable,
		unsigned char * byte_buffer, unsigned int buf_size) {

	FILE * file = fopen(executable, "w");
	fwrite(byte_buffer, 1, buf_size, file);
	fclose(file);
	chmod(executable, S_IRWXU | S_IRWXG | S_IRWXO);
}
