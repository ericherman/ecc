#include "elf_header.h"
#include "c_lib.h"

unsigned char elf_header[] = {
	/* MAGIC: */
	0x7f, 0x45, 0x4c, 0x46, 0x01, 0x01, 0x01, 0x00, /* 127,E,L,F,1,1,1,0 */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, /*   0,0,0,0,0,0,0,0 */
	/* ? */
	0x02, 0x00, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x54, 0x80, 0x04, 0x08, 0x34, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x34, 0x00, 0x20, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x04, 0x08,
	0x00, 0x80, 0x04, 0x08,
	0xff, 0xff, 0x00, 0x00, /* the 0xFF, 0xFF here is program size? */
	0xff, 0xff, 0x00, 0x00, /* the 0xFF, 0xFF here is memory size? */
	0x05, 0x00, 0x00, 0x00,
	0x00, 0x10, 0x00, 0x00
};

/* contents %ebx will be returned as the return code */
unsigned char sys_exit[] = {
	0xb8, /* movl %eax */
	0x01, 0x00, 0x00, 0x00, /* 1 == SYS_EXIT */
	0xcd, /* int */
	0x80  /* Linux Kernel */
};

unsigned char * raw_elf_header() {
	return elf_header;
}

unsigned int raw_elf_header_size() {
	return sizeof(elf_header);
}

unsigned char * raw_linux_return() {
	return sys_exit;
}

unsigned int raw_linux_return_size() {
	return sizeof(sys_exit);
}

void output_header(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * length) {

	unsigned char * bytes;
	unsigned int i;

	*length = raw_elf_header_size();
	if (buf_size < *length) {
		err_msg("buf_size too small for output_header\n");
		return;
	}
	bytes = raw_elf_header();
	for (i = 0; i < *length; i++) {
		buf[i] = bytes[i];
	}
}

void output_os_return(unsigned char * buf,
		unsigned int buf_size,
		unsigned int * bytes_written) {

	unsigned char * bytes;
	unsigned int i, length;

	length = raw_linux_return_size();
	if (buf_size < *bytes_written + length) {
		err_msg("buf_size too small for output_os_return\n");
		return;
	}
	bytes = raw_linux_return();
	for (i = 0; i < length; i++) {
		buf[(*bytes_written)++] = bytes[i];
	}
}
