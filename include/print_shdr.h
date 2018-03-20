#pragma once

#include <elf.h>
#include <link.h>

ElfW(Shdr) *get_shdr_entry(ElfW(Shdr) *sh, ElfW(Half) shnum, ElfW(Half) shsize, ElfW(Word) type, int k);
void print_shdr(ElfW(Shdr) *sh, ElfW(Half) shnum, ElfW(Half) shsize, char *strtab);
