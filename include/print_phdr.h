#pragma once

#include <elf.h>
#include <link.h>

void print_phdr(ElfW(Phdr) *ph, ElfW(Half) phnum, ElfW(Half) phsize);
