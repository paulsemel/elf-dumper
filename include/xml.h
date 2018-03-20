#pragma once

int xml_new_section(const char *name);
int xml_new_array(const char *name);
int xml_end_section(const char *name, bool last);
int xml_end_array(const char *name, bool last);
int xml_new_entry(const char *key, const char *value, bool last);
void xml_configure(FILE *f);
int xml_init_format();
int xml_end_format();
