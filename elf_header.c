#include "elf_header.h"
#include "c_lib.h"

unsigned char elf_header[] = {
	/* MAGIC: */
	0x7f, 0x45, 0x4c, 0x46, /* 127, E, L, F, */
	0x01, 0x01, 0x01, 0x00, /* Class (32), little endian, version, OS ABI */
	0x00, 0x00, 0x00, 0x00,	0x00, 0x00, 0x00, 0x00, /* padding */

	0x02, 0x00, /* e_type EXEC (Executable file) */
	0x03, 0x00, /* e_machine Intel 80386 */
	0x01, 0x00, 0x00, 0x00, /* e_version (always 1) */
	0x54, 0x80, 0x04, 0x08, /* e_entry (addr of _start) */
	0x34, 0x00, 0x00, 0x00, /* e_phoff (program header offset */
	0x00, 0x00, 0x00, 0x00, /* e_shoff (section header table offset) */
	0x00, 0x00, 0x00, 0x00, /* e_flags (processor-specific flags) */
	0x34, 0x00, /* e_ehsize (elf header is 52 bytes) */
	0x20, 0x00, /* e_phentsize (program header entries are 32 bytes)*/
	0x01, 0x00, /* e_phnum (number of program header entries) */
	0x28, 0x00, /* e_shentsize (section headers are 40 bytes) */
	0x00, 0x00, /* e_shnum (number of section headers) */
	0x00, 0x00, /* e_shstrndx section header table index for string names */
	/* section header table's size = (e_shentsize * e_shnum) */

	/* program header */
	0x01, 0x00, 0x00, 0x00, /* p_type PT_LOAD=1 */
	0x00, 0x00, 0x00, 0x00, /* p_offset */
	0x00, 0x80, 0x04, 0x08, /* p_vaddr virtual address of first byte */
	0x00, 0x80, 0x04, 0x08, /* p_paddr physical address */
	0x00, 0x00, 0x00, 0x00, /* p_filesz */
	0x00, 0x00, 0x00, 0x00, /* p_memsz */
	0x05, 0x00, 0x00, 0x00, /* p_flags 5=Read, execute; 6=RW; 7=RWX */
	0x00, 0x10, 0x00, 0x00, /* p_align 4096 */
	/* _start */
};

unsigned char elf_section_header[] = {
	0x00, 0x00, 0x00, 0x00,	/* sh_name index into the sh string table */
	0x00, 0x00, 0x00, 0x00,	/* sh_type SHT_STRTAB=3, SHT_PROGBITS=1,... */
	0x00, 0x00, 0x00, 0x00,	/* sh_flags SHF_WRITE=0x1, SHF_ALLOC=0x2...*/
	0x00, 0x00, 0x00, 0x00,	/* sh_addr addr where should reside in memory*/
	0x00, 0x00, 0x00, 0x00,	/* sh_offset from the beginning of the file */
	0x00, 0x00, 0x00, 0x00,	/* sh_size */
	0x00, 0x00, 0x00, 0x00,	/* sh_link */
	0x00, 0x00, 0x00, 0x00,	/* sh_info */
	0x00, 0x00, 0x00, 0x00,	/* sh_addralign */
	0x00, 0x00, 0x00, 0x00,	/* sh_entsize non-zero if fixed sized entries */
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

	length = *bytes_written;
	length += length % 4;
	/* fixup program size */
	buf[68] = 0xFF & length;
	buf[69] = 0xFF & (length >> 8);
	buf[70] = 0xFF & (length >> 16);
	buf[71] = 0xFF & (length >> 24);

	/* fixup memory size */
	buf[72] = 0xFF & length;
	buf[73] = 0xFF & (length >> 8);
	buf[74] = 0xFF & (length >> 16);
	buf[75] = 0xFF & (length >> 24);
}
