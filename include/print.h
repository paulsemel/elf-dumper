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

extern int (*init_format) ();
extern int (*end_format) ();
extern int (*new_section) (const char *name);
extern int (*end_section) (const char *name, bool last);
extern int (*new_array) (const char *name);
extern int (*end_array) (const char *name, bool last);
extern int (*new_entry) (const char *key, const char *value, bool last);
