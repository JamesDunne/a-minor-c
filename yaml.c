#include <stdlib.h>
#include <stdio.h>

#include "v6.h"

static const cyaml_config_t config = {
	.log_level = CYAML_LOG_WARNING, /* Logging errors and warnings only. */
	.log_fn = cyaml_log,            /* Use the default logging function. */
	.mem_fn = cyaml_mem,            /* Use the default memory allocator. */
	.flags = CYAML_CFG_IGNORE_UNKNOWN_KEYS,
};

bool load_setlists(struct setlists **setlists) {
	cyaml_err_t err;

	err = cyaml_load_file(
		"setlists.yml",
		&config,
		&setlists_schema,
		(void **) setlists,
		NULL
	);
	if (err != CYAML_OK) {
		fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
		return false;
	}

	return true;
}

bool load_programs(struct midi_programs **midi_programs) {
	cyaml_err_t err;

	err = cyaml_load_file(
		"all-programs-v6.yml",
		&config,
		&midi_programs_schema,
		(void **) midi_programs,
		NULL
	);
	if (err != CYAML_OK) {
		fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
		return false;
	}

	return true;
}

bool free_setlists(struct setlists **setlists) {
	cyaml_err_t err;

	err = cyaml_free(&config, &setlists_schema, *setlists, 0);
	if (err != CYAML_OK) {
		fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
		return false;
	}

	*setlists = NULL;
	return true;
}

bool free_programs(struct midi_programs **midi_programs) {
	cyaml_err_t err;

	err = cyaml_free(&config, &midi_programs_schema, *midi_programs, 0);
	if (err != CYAML_OK) {
		fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
		return false;
	}

	*midi_programs = NULL;
	return true;
}

bool save_programs(struct midi_programs *midi_programs) {
	cyaml_err_t err;

	err = cyaml_save_file(
		"all-programs-v6.yml",
		&config,
		&midi_programs_schema,
		(void *) midi_programs,
		0
	);
	if (err != CYAML_OK) {
		fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err));
		return false;
	}

	return true;
}
