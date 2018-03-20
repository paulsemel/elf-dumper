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


void json_init(FILE *f)
{
	init_format = json_init_format;
	end_format = json_end_format;
	new_section = json_new_section;
	end_section = json_end_section;
	new_array = json_new_array;
	end_array = json_end_array;
	new_entry = json_new_entry;
	json_configure(f);
}

void xml_init(FILE *f)
{
	init_format = xml_init_format;
	end_format = xml_end_format;
	new_section = xml_new_section;
	end_section = xml_end_section;
	new_array = xml_new_array;
	end_array = xml_end_array;
	new_entry = xml_new_entry;
	xml_configure(f);
}

static int print_usage(char *pname)
{
	printf("Usage: %s bin\n", pname);
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

	res = argc < 2 ? print_usage(argv[0]) : fileExists(argv[1]);

	if (!res) {
		int fd = open(argv[1], O_RDONLY);
		char *base = map_file(fd);
		// xml_init(stdout);
		json_init(stdout);
		print_elf(base);
		unmap_file(base, fd);
	}

	return res;
}
