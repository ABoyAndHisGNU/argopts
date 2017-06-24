#ifndef MISC_H_
#define MISC_H_

#include <stdlib.h>

#define TRUE	1
#define FALSE	0

#define FREE(ptr)			\
	do				\
	{				\
		free((ptr));		\
		(ptr) = NULL;		\
	} while (0);			\

#define CAST_BOOL_PTR(ptr)	((_Bool *) (ptr))
#define CAST_INT_PTR(ptr)	((int *) (ptr))
#define CAST_STR_PTR(ptr)	((char **) (ptr))

#endif /* !MISC_H_ */
