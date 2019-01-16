#pragma once

#include "yaml.h"

struct controller {
	struct setlists *setlists;
	struct midi_programs *midi_programs;

	int setlist;
	int program;

};

// Create/destroy controller and load/save functions:

struct controller *ctl_new();

void ctl_free(struct controller *c);

int ctl_load(struct controller *c);

int ctl_save(struct controller *c);
