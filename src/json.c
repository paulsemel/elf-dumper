#include <stdio.h>
#include <stdbool.h>

static int json_level = 0;
static FILE *out;

static void json_print_offset(void)
{
	for (int i = 0; i < json_level; i++)
		fprintf(out, "\t");
	fflush(out);
}

int json_new_section(const char *name)
{
	json_level++;
	json_print_offset();
	if (name)
		fprintf(out, "\"%s\": ", name);
	fprintf(out, "{\n");
	return 1;
}

int json_new_array(const char *name)
{
	json_level++;
	json_print_offset();
	if (name)
		fprintf(out, "\"%s\": ", name);
	fprintf(out, "[\n");
	return 1;
}

int json_end_section(const char *name, bool last)
{
	json_print_offset();
	fprintf(out, "}%s\n", last ? "" : ",");
	json_level--;
	return 1;
}

int json_end_array(const char *name, bool last)
{
	json_print_offset();
	fprintf(out, "]%s\n", last ? "" : ",");
	json_level--;
	return 1;
}

int json_new_entry(const char *key, const char *value, bool last)
{
	json_level++;
	json_print_offset();
	fprintf(out, "\"%s\": %s%s\n", key, value, last ? "" : ",");
	json_level--;
	return 1;
}

void json_configure(FILE *f)
{
	out = f;
}

int json_init_format()
{
	fprintf(out, "{\n");
	return 1;
}

int json_end_format()
{
	fprintf(out, "}\n");
	return 1;
}
