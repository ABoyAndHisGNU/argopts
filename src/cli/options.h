#ifndef OPTIONS_H_
#define OPTIONS_H_

#include <stddef.h>

struct global_options
{
	_Bool verbose;
};

extern struct global_options g_opts;

enum option_type
{
	OPT_BOOL,
	OPT_INT,
	OPT_STRING
};

struct option
{
	struct option *next;

	char *name;
	char *desc;

	size_t n;
	void *value;
	enum option_type type;

	_Bool free;
};

struct options_db
{
	struct option *options;

	const char *header;
	const char *usage;
};

typedef struct option option_t;
typedef struct options_db options_db_t;

/* DATABASE FUNCTIONS */
options_db_t *options_new_db();
void options_delete_db(options_db_t **ptr);
void options_print_db(options_db_t *db);
void options_process_args(options_db_t *db, int optc, char *optv[]);

/* DATABASE (ENTRY) OPTIONS */
option_t *
options_add(
	options_db_t *db,
	char *name,
	char *desc,
	void *value,
	enum option_type type,
	size_t n);

#endif /* !OPTIONS_H_ */
