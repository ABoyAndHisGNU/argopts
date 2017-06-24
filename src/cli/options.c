#include "options.h"

#include "utils/fatal.h"
#include "utils/misc.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

static void
link_option(options_db_t *db, option_t *option)
{
	option_t *prev, *e;

	if (option->name[0] != '-' || option->name[1] != '-')
		fatal("options '%s' must start with '--'", option->name);

	for (prev = NULL, e = db->options; e != NULL; prev = e, e = e->next)
		if (e->name[0] == option->name[0] && !strcmp(e->name, option->name))
			fatal("option '%s' is defined twice", option->name);

	if (prev)
		prev->next = option;
	else	/* first option */
		db->options = option;

	option->next = NULL;
}

static size_t
process_arg(option_t *option, char *optv[], int count)
{
	int i;

	assert(option);
	assert(optv);

	++optv;
	--count;

	if (option->type == OPT_BOOL && option->n == 0)
		((_Bool *)option->value)[0] = 1;

	for (i = 0; i < option->n; i++)
	{
		if ((i >= count) || (optv[i][0] == '-' && optv[i][1] == '-'))
			fatal("'%s' expects '%zu' arguments but only found '%d'", option->name, option->n, i);

		switch (option->type)
		{
			case OPT_BOOL:
				CAST_BOOL_PTR(option->value)[i] = (optv[i][0] == '1');
				break;
			case OPT_INT:
				((int *) option->value)[i] = atoi(optv[i]);
				break;
			case OPT_STRING:
				((char **) option->value)[i] = optv[i];
				break;
			default:
				fatal("option added with type '%d' but no such type exists", option->type);
		}
	}

	return i;
}

options_db_t *
options_new_db()
{
	options_db_t *db;

	db = (options_db_t *) calloc(1, sizeof(options_db_t));
	if (!db)
		fatal("Cannot calloc(), system out of virtual memory");

	return db;
}

void
options_delete_db(options_db_t **ptr)
{
	options_db_t *db;
	option_t *prev, *e;

	assert(ptr);
	assert(*ptr);

	db = *ptr;

	/* Deallocate Options */
	{
		for (prev = NULL, e = db->options; e != NULL; prev = e, e = e->next)
		{
			if (prev && prev->free)
				free(prev->value);
			free(prev);
		}

		if (prev)
		{
			free(prev->value);
			FREE(prev);
		}
	}

	FREE(db);
}

void
options_print_db(options_db_t *db)
{
	option_t *prev, *e;

	assert(db && db->options);

	if (db->header)
		puts(db->header);
	
	printf("usage: %s\n", db->usage);

	for (prev = NULL, e = db->options; e != NULL; prev = e, e = e->next)
		printf("    %s\t%s\n", e->name, e->desc);
}

void
options_process_args(options_db_t *db, int optc, char *optv[])
{
	int i;
	char *arg;
	option_t *prev, *e;

	assert(db);

	for (i = 0; i < optc; i++)
	{
		arg = optv[i];

		if (arg[0] != '-' || arg[1] != '-')
			fatal("orphan '%s' encountered while processing args", arg);

		for (prev = NULL, e = db->options; e != NULL; prev = e, e = e->next)
		{
			if (!strcmp(e->name, arg))
			{
				i += process_arg(e, &optv[i], optc - i);
			}
		}
	}
}

option_t *
options_add(
	options_db_t *db,
	char *name,
	char *desc,
	void *value,
	enum option_type type,
	size_t n)
{
	option_t *option;

	assert(db);
	assert(type <= OPT_STRING);
	assert(!((n == 0) && (type != OPT_BOOL)));

	option = (option_t *) calloc(1, sizeof(option_t));
	if (!option)
		fatal("out of virtual memory");

	if (value)
		option->value = value;
	else
	{
		switch (type)
		{
			case OPT_BOOL:
				option->value = (void *) calloc(n, sizeof(_Bool));
				break;
			case OPT_INT:
				option->value = (void *) calloc(n, sizeof(int));
				break;
			case OPT_STRING:
				option->value = (void *) calloc(n, sizeof(char *));
				break;
			default:
				fatal("option added with type '%d' but no such type exists", type);
		}

		option->free = TRUE;
	}

	option->next = NULL;
	option->name = name;
	option->desc = desc;
	option->n = n;
	option->type = type;

	link_option(db, option);

	return option;
}
