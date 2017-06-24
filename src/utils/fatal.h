#ifndef FATAL_H
#define FATAL_H

#include <stdio.h>

void fatal_hook(void (*fn)(FILE *fstream));

void _fatal(char *file, const char *func, int line, char *fmt, ...) __attribute__((noreturn));

#ifdef __GNUC__
	#define fatal(fmt, args...) \
		_fatal(__FILE__, __FUNCTION__, __LINE__, fmt, ##args)
#else	/* !__GNUC__ */
	#error "Currently only GCC is supported"
#endif

#endif /* !FATAL_H */
