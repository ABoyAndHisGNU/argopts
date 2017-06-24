#include "cli/options.h"
#include "utils/fatal.h"
#include "utils/misc.h"

#include <stdio.h>
#include <stdlib.h>

#define APP_NAME	"optdemo"
#define APP_VERSION	"v.0.0"

struct global_options g_opts = { .verbose = 1 };

static options_db_t *db;

void
print_help(FILE *fstream)
{
	options_print_db(db);
}

int
main(int argc, char *argv[])
{
	char *str;
	_Bool b_help;
	_Bool b_verbose;

	--argc;
	++argv;

	fatal_hook(print_help);
	if (!argc)
		fatal("missing arguments");

	db = options_new_db();
	
	db->header = APP_NAME ": This program is an example implementation of the argopts library.";
	db->usage = APP_NAME " [options]";

	options_add(
		db,
		"--tbool",
		"prints this menu",
		NULL,
		OPT_BOOL,
		3);
	options_add(
		db,
		"--tint",
		"prints this menu",
		NULL,
		OPT_INT,
		1);
	options_add(
		db,
		"--tstr",
		"prints this menu",
		&str,
		OPT_STRING,
		1);

	options_process_args(db, argc, argv);
	options_delete_db(&db);

	return EXIT_SUCCESS;
}
