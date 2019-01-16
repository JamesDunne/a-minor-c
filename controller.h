#pragma once

#include "yaml.h"

struct controller {
	struct setlists *setlists;
	struct midi_programs *midi_programs;
};

struct controller *ctl_new();

int ctl_load(struct controller *c);

void ctl_free(struct controller *c);
