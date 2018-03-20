#pragma once

#include <stdbool.h>
#include <stdio.h>
#include "xlat.h"
#include "e_type.h"
#include "e_machine.h"
#include "e_version.h"
#include "sh_type.h"
#include "p_type.h"
#include "st_bind.h"
#include "st_type.h"
#include "st_other.h"

#define print_entry_xval(ent, ptr, last) \
{ \
  char value[256]; \
  char sanitize[256]; \
  sprintxval(value, 255, ent, (ptr)->ent, "???"); \
  sprintf(sanitize, "\"%s\"", value); \
  new_entry(#ent, sanitize, last); \
}

#define print_entry(ent, ptr, format, last) \
{ \
  char value[255]; \
  sprintf(value, format, (ptr)->ent); \
  new_entry(#ent, value, last); \
}

extern int (*init_format)();
extern int (*end_format)();
extern int (*new_section)(const char *name);
extern int (*end_section)(const char *name, bool last);
extern int (*new_array)(const char *name);
extern int (*end_array)(const char *name, bool last);
extern int (*new_entry)(const char *key, const char *value, bool last);
extern void (*configure)(FILE *f);

struct console {
	const char *name;
	int (*init_format)();
	int (*end_format)();
	int (*new_section)(const char *name);
	int (*end_section)(const char *name, bool last);
	int (*new_array)(const char *name);
	int (*end_array)(const char *name, bool last);
	int (*new_entry)(const char *key, const char *value, bool last);
	void (*configure)(FILE *f);
} __attribute__((packed));

extern struct console __start_consoles[];
extern struct console __stop_consoles[];

#define register_console(__name, __init, __end_format, \
			 __new_section, __end_section, \
			 __new_array, __end_array, __new_entry, \
			 __configure) \
	static struct console \
	__attribute__ ((section("consoles"), used)) \
	__attribute__ ((aligned(8))) \
	cons_ ## __name = { \
		.name = #__name, \
		.init_format = __init, \
		.end_format = __end_format, \
		.new_section = __new_section, \
		.end_section = __end_section, \
		.new_array = __new_array, \
		.end_array = __end_array, \
		.new_entry = __new_entry, \
		.configure = __configure \
	}
