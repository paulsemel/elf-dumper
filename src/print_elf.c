#include <strings.h>
#include <elf.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "print_shdr.h"
#include "print_ehdr.h"
#include "print_phdr.h"
#include "print_symbols.h"

int (*init_format) ();
int (*end_format) ();
int (*new_section) (const char *name);
int (*end_section) (const char *name, bool last);
int (*new_array) (const char *name);
int (*end_array) (const char *name, bool last);
int (*new_entry) (const char *key, const char *value, bool last);

static char *strtab_search(ElfW(Shdr) *sh, ElfW(Half) shnum, ElfW(Half) shentsize, char *base, const char *section)
{
  char *strtab = NULL;
  for (int i = 0; i < 5; i++)
  {
    ElfW(Shdr) *tmp = get_shdr_entry(sh, shnum, shentsize, SHT_STRTAB, i);
    strtab = base + tmp->sh_offset;
    if (!strcmp(section, strtab + tmp->sh_name))
      break;
  }
  return strtab;
}

void print_elf(char *base)
{
  ElfW(Ehdr) *ehdr = (ElfW(Ehdr) *)base;
  ElfW(Phdr) *phdr = (ElfW(Phdr) *)(base + ehdr->e_phoff);
  ElfW(Shdr) *shdr = (ElfW(Shdr) *)(base + ehdr->e_shoff);
  char *strtab = NULL;

  strtab = strtab_search(shdr, ehdr->e_shnum, ehdr->e_shentsize, base, ".shstrtab");

  init_format();
  new_section("header");
  print_ehdr(ehdr);
  end_section("header", false);
  new_array("sections");
  print_shdr(shdr, ehdr->e_shnum, ehdr->e_shentsize, strtab);
  end_array("sections", false);
  new_array("segments");
  print_phdr(phdr, ehdr->e_phnum, ehdr->e_phentsize);
  end_array("segments", false);

  new_array("symbols");
  print_symbols(shdr, ehdr->e_shnum, ehdr->e_shentsize, strtab, base);
  end_array("symbols", true);

  end_format();
}
