#pragma once

#include <elf.h>
#include <link.h>

void print_ehdr(ElfW(Ehdr) *eh);
