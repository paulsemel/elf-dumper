#pragma once

#include <stdbool.h>

int json_new_section(const char *name);
int json_new_array(const char *name);
int json_end_section(const char *name, bool last);
int json_end_array(const char *name, bool last);
int json_new_entry(const char *key, const char *value, bool last);
void json_configure(FILE *f);
int json_init_format();
int json_end_format();
