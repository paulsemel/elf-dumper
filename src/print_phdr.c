#include <elf.h>
#include <link.h>
#include "print.h"

int (*init_format) ();
int (*end_format) ();
int (*new_section) (const char *name);
int (*end_section) (const char *name, bool last);
int (*new_array) (const char *name);
int (*end_array) (const char *name, bool last);
int (*new_entry) (const char *key, const char *value, bool last);

void print_phdr(ElfW(Phdr) *ph, ElfW(Half) phnum, ElfW(Half) phsize)
{
  for (ElfW(Half) i = 0; i < phnum; i++, ph = (ElfW(Phdr) *)((char *)ph + phsize))
  {
    new_section(NULL);
    print_entry_xval(p_type, ph, false);
    print_entry(p_flags, ph, "%u", false);
    print_entry(p_offset, ph, "%lu", false);
    print_entry(p_vaddr, ph, "%lu", false);
    print_entry(p_paddr, ph, "%lu", false);
    print_entry(p_filesz, ph, "%lu", false);
    print_entry(p_memsz, ph, "%lu", false);
    print_entry(p_align, ph, "%lu", true);
    end_section(NULL, i == phnum - 1 ? true : false);
  }
}
