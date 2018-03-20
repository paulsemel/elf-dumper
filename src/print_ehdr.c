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

void print_ehdr(ElfW(Ehdr) * eh)
{
	print_entry_xval(e_type, eh, false);
	print_entry_xval(e_machine, eh, false);
	print_entry_xval(e_version, eh, false);
	print_entry(e_entry, eh, "%lu", false);
	print_entry(e_phoff, eh, "%lu", false);
	print_entry(e_shoff, eh, "%lu", false);
	print_entry(e_flags, eh, "%u", false);
	print_entry(e_ehsize, eh, "%u", false);
	print_entry(e_phentsize, eh, "%u", false);
	print_entry(e_phnum, eh, "%u", false);
	print_entry(e_shentsize, eh, "%u", false);
	print_entry(e_shnum, eh, "%u", false);
	print_entry(e_shstrndx, eh, "%u", true);
}
