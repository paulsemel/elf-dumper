#include <elf.h>
#include <string.h>
#include "print_shdr.h"
#include "print.h"

int (*init_format) ();
int (*end_format) ();
int (*new_section) (const char *name);
int (*end_section) (const char *name, bool last);
int (*new_array) (const char *name);
int (*end_array) (const char *name, bool last);
int (*new_entry) (const char *key, const char *value, bool last);

static void print_st_info(unsigned char st_info)
{
  char value[256];
  char sanityze[256];
  sprintxval(value, 256, st_bind, ELF32_ST_BIND(st_info), "???");
  sprintf(sanityze, "\"%s\"", value);
  new_entry("bind", sanityze, false);
  sprintxval(value, 256, st_type, ELF32_ST_TYPE(st_info), "???");
  sprintf(sanityze, "\"%s\"", value);
  new_entry("type", sanityze, true);
}

static void print_st_other(unsigned char st_o)
{
  char value[256];
  char sanityze[256];
  sprintxval(value, 256, st_other, ELF32_ST_VISIBILITY(st_o), "???");
  sprintf(sanityze, "\"%s\"", value);
  new_entry("bind", sanityze, true);
}

static char *strtab_resolve(ElfW(Shdr) *sh, ElfW(Half) shnum,
                            ElfW(Half) shentsize, char *strtab,
                            char *base, const char *section)
{
  char *str = NULL;
  for (int i = 0; i < 5; i++)
  {
    ElfW(Shdr) *tmp = get_shdr_entry(sh, shnum, shentsize, SHT_STRTAB, i);
    str = base + tmp->sh_offset;
    if (!strcmp(section, strtab + tmp->sh_name))
      break;
  }
  return str;
}

void print_symbol(ElfW(Sym) *sym, ElfW(Xword) symnum, ElfW(Xword) symsize, char *strtab)
{
  char value[256];
  for (ElfW(Xword) i = 0; i < symnum; i++, sym = (ElfW(Sym) *)((char *)sym + symsize))
  {
    new_section(NULL);
    print_entry(st_name, sym, "%u", false);
    new_section("st_info");
    print_st_info(sym->st_info);
    end_section("st_info", false);
    new_section("st_other");
    print_st_other(sym->st_other);
    end_section("st_other", false);
    print_entry(st_shndx, sym, "%u", false);
    print_entry(st_value, sym, "%lu", false);
    sprintf(value, "\"%s\"", strtab + sym->st_name);
    new_entry("name", value, false);
    print_entry(st_size, sym, "%lu", true);
    end_section(NULL, i == symnum - 1 ? true : false);
  }
}

void print_symbols(ElfW(Shdr) *sh, ElfW(Half) shnum, ElfW(Half) shsize,
                       char *strtab, char *base)
{
  ElfW(Shdr) *save = sh;
  new_section(NULL);
  new_entry("name", "\".dynsym\"", false);
  new_array("data");
  save = get_shdr_entry(sh, shnum, shsize, SHT_DYNSYM, 1);
  char *dynstr = strtab_resolve(sh, shnum, shsize, strtab, base, ".dynstr");
  print_symbol((ElfW(Sym) *)(save->sh_offset + base), save->sh_size / sizeof(ElfW(Sym)), sizeof(ElfW(Sym)), dynstr);
  end_array("data", true);
  end_section(NULL, false);

  new_section(NULL);
  new_entry("name", "\".dynsym\"", false);
  new_array("data");
  save = get_shdr_entry(sh, shnum, shsize, SHT_SYMTAB, 1);
  dynstr = strtab_resolve(sh, shnum, shsize, strtab, base, ".strtab");
  print_symbol((ElfW(Sym) *)(save->sh_offset + base), save->sh_size / sizeof(ElfW(Sym)), sizeof(ElfW(Sym)), dynstr);
  end_array("data", true);
  end_section(NULL, true);
}
