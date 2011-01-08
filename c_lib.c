#include "c_lib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void *heap_malloc(unsigned int size)
{
	return malloc(size);
}

void heap_free(void *ptr)
{
	free(ptr);
}

void err_msg(const char *msg)
{
	fprintf(stderr, "%s", msg);
}

void die()
{
	int status = 1;
	exit(status);
}

void read_line(const char *in_file,
	       char *line_buf, unsigned int buf_size, unsigned int *chars_read)
{
	FILE *file;

	file = fopen(in_file, "r");
	if (file == NULL) {
		fprintf(stderr, "Error opening file %s", in_file);
		exit(1);
	}

	fgets(line_buf, buf_size, file);
	*chars_read = strlen(line_buf);
	fclose(file);
}

void write_file(const char *out_file,
		unsigned char *byte_buffer, unsigned int buf_size)
{

	FILE *file;

	file = fopen(out_file, "w");
	fwrite(byte_buffer, 1, buf_size, file);
	fclose(file);

	chmod(out_file, S_IRWXU | S_IRWXG | S_IRWXO);
}
