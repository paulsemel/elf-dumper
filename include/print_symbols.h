#pragma once

#include <elf.h>
#include <link.h>

void print_symbols(ElfW(Shdr) *sh, ElfW(Half) shnum, ElfW(Half) shsize,
                       char *strtab, char *base);
void print_symbol(ElfW(Sym) *sym, ElfW(Xword) symnum, ElfW(Xword) symsize, char *strtab);
