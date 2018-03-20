CFLAGS=-Wall -Werror -std=c99 -Iinclude# -g -fprofile-arcs -ftest-coverage
#LDFLAGS= -lgcov --coverage
MAIN=main.c
SRCS=xlat.c json.c xml.c print_elf.c print_shdr.c print_phdr.c print_symbols.c print_ehdr.c
VPATH=src
OBJS := ${SRCS:.c=.o} ${MAIN:.c=.o}
BIN=main
LIB=libreadelf.so

all: main

target: $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $(BIN)

lib: bootstrap
	@$(MAKE) --no-print-directory mklib

mklib: CFLAGS+=-fPIC
mklib: ${SRCS:.c=.o}
	$(CC) $^ -shared -o $(LIB)

main: bootstrap
	@$(MAKE) --no-print-directory target

bootstrap:
	./bootstrap.sh

clean:
	$(RM) $(OBJS) $(BIN) *.gcov *.gcno *.gcda $(LIB)
	./clean.sh

proper:
	$(RM) $(OBJS) *.gcov *.gcno *.gcda $(LIB)

.PHONY: all clean proper lib
