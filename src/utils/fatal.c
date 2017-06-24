#include "fatal.h"

#include "../cli/options.h"

#include <stdarg.h>
#include <stdlib.h>

static void (*p_hook_fn)(FILE *fstream) = NULL;

void
fatal_hook(void (*fn)(FILE *fstream))
{
	p_hook_fn = fn;
}

void
_fatal(char *file, const char *func, int line, char *fmt, ...)
{
	va_list v;

	if (g_opts.verbose)
		fprintf(stderr, "[%s %s:%d] ", file, func, line);

	fprintf(stderr, "fatal: ");

	va_start(v, fmt);
	vfprintf(stderr, fmt, v);
	va_end(v);

	fprintf(stderr, "\n");

	if (p_hook_fn)
		(*p_hook_fn)(stderr);
	
	exit(EXIT_FAILURE);
}
