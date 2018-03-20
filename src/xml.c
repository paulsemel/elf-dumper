#include <stdio.h>
#include <stdbool.h>
#include "print.h"

#define __unused __attribute__((unused))

static int xml_level = 0;
static FILE *out;

static void xml_print_offset(void)
{
	for (int i = 0; i < xml_level; i++)
		fprintf(out, "\t");
}

int xml_new_section(const char *name)
{
	xml_level++;
	xml_print_offset();
	fprintf(out, "<%s type=\"entity\">\n", name ? name : "entity");
	return 1;
}

int xml_new_array(const char *name)
{
	xml_level++;
	xml_print_offset();
	fprintf(out, "<%s type=\"array\">\n", name ? name : "array");
	return 1;
}

int xml_end_section(const char *name, bool __unused last)
{
	xml_print_offset();
	fprintf(out, "</%s>\n", name ? name : "entity");
	xml_level--;
	return 1;
}

int xml_end_array(const char *name, bool __unused last)
{
	xml_print_offset();
	fprintf(out, "</%s>\n", name ? name : "array");
	xml_level--;
	return 1;
}

int xml_new_entry(const char *key, const char *value, bool __unused last)
{
	xml_level++;
	xml_print_offset();
	fprintf(out, "<%s>%s</%s>\n", key, value, key);
	xml_level--;
	return 1;
}

void xml_configure(FILE *f)
{
	out = f;
}

int xml_init_format()
{
	fprintf(out, "<myreadelf>\n");
	return 1;
}

int xml_end_format()
{
	fprintf(out, "</myreadelf>\n");
	return 1;
}

register_console(xml, &xml_init_format, &xml_end_format, &xml_new_section,
		 &xml_end_section, &xml_new_array, &xml_end_array, &xml_new_entry,
		 &xml_configure);
