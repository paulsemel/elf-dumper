#include <elf.h>
#include <link.h>
#include "print.h"

int (*init_format)();
int (*end_format)();
int (*new_section)(const char *name);
int (*end_section)(const char *name, bool last);
int (*new_array)(const char *name);
int (*end_array)(const char *name, bool last);
int (*new_entry)(const char *key, const char *value, bool last);

ElfW(Shdr)
	* get_shdr_entry(ElfW(Shdr) * sh, ElfW(Half) shnum, ElfW(Half) shsize,
			 ElfW(Word) type, int k)
{
	ElfW(Shdr) *res = NULL;
	for (ElfW(Half) i = 0; i < shnum;
	     i++, sh = (ElfW(Shdr) *)((char *)sh + shsize)) {
		if (sh->sh_type == type && k-- <= 1) {
			res = sh;
			break;
		}
	}
	return res;
}

void print_shdr(ElfW(Shdr) * sh, ElfW(Half) shnum, ElfW(Half) shsize,
		char *strtab)
{
	char buf[256];
	for (ElfW(Half) i = 0; i < shnum;
	     i++, sh = (ElfW(Shdr) *)((char *)sh + shsize)) {
		new_section(NULL);
		print_entry(sh_name, sh, "%u", false);
		print_entry_xval(sh_type, sh, false);
		print_entry(sh_flags, sh, "%lu", false);
		print_entry(sh_addr, sh, "%lu", false);
		print_entry(sh_offset, sh, "%lu", false);
		print_entry(sh_size, sh, "%lu", false);
		print_entry(sh_link, sh, "%u", false);
		print_entry(sh_info, sh, "%u", false);
		print_entry(sh_addralign, sh, "%lu", false);
		print_entry(sh_entsize, sh, "%lu", false);
		sprintf(buf, "\"%s\"", strtab + sh->sh_name);
		new_entry("name", buf, true);
		end_section(NULL, i == shnum - 1 ? true : false);
	}
}
