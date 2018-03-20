#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>
#include <link.h>
#include <stdbool.h>
#include <string.h>
#include "xlat.h"
#include "json.h"
#include "xml.h"
#include "print.h"
#include "print_elf.h"

int (*init_format)();
int (*end_format)();
int (*new_section)(const char *name);
int (*end_section)(const char *name, bool last);
int (*new_array)(const char *name);
int (*end_array)(const char *name, bool last);
int (*new_entry)(const char *key, const char *value, bool last);

int console_init(const char *name, FILE *f)
{
	size_t count = (size_t)(__stop_consoles - __start_consoles);
	int res = 0;
	for (size_t i = 0; i < count; i++) {
		if (!strcmp(__start_consoles[i].name, name)) {
			init_format = __start_consoles[i].init_format;
			end_format = __start_consoles[i].end_format;
			new_section = __start_consoles[i].new_section;
			end_section = __start_consoles[i].end_section;
			new_array = __start_consoles[i].new_array;
			end_array = __start_consoles[i].end_array;
			new_entry = __start_consoles[i].new_entry;
			__start_consoles[i].configure(f);
			res = 1;
			break;
		}
	}
	return res;
}

static int print_usage(char *pname)
{
	printf("Usage: %s bin [--format|-f] format\n", pname);
	return 1;
}

static int issue_error(char *msg)
{
	fprintf(stderr, "%s\n", msg);
	return 1;
}

static int fileExists(char *filename)
{
	return access(filename, F_OK) < 0 ? issue_error("File doesn't exist")
					  : 0;
}

static char *getFormat(int argc, char *argv[])
{
	char *res = NULL;
	for (int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "--format")
		    || !strcmp(argv[i], "-f"))
			res = argv[i + 1];
	}
	return res;
}

static void *map_file(int fd)
{
	struct stat buf;
	void *res = !fstat(fd, &buf) ? mmap(NULL, buf.st_size, PROT_READ,
					    MAP_SHARED, fd, 0)
				     : (void *)MAP_FAILED;
	return res;
}

static int unmap_file(void *addr, int fd)
{
	struct stat buf;
	int res = 1;
	if (!fstat(fd, &buf)) {
		munmap(addr, buf.st_size);
		close(fd);
		res = 0;
	}
	return res;
}

int main(int argc, char *argv[])
{
	int res = 0;

	res = argc < 4 ? print_usage(argv[0]) : fileExists(argv[1]);
	char *name = getFormat(argc, argv);

	if (!res && name) {
		int fd = open(argv[1], O_RDONLY);
		char *base = map_file(fd);
		if (!console_init(name, stdout))
			return 1;
		print_elf(base);
		unmap_file(base, fd);
	}

	return res;
}
