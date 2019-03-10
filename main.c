#include <stdlib.h>
#include <stdio.h>

#include "controller.h"

int main(int argc, char **argv) {
	int e;
	struct controller *c = ctl_new();

	if (0 != (e = midi_init(&c->midi, NULL, 0))) {
		if (0 != (e = midi_init(&c->midi, "/dev/null", 0))) {
			return e;
		}
	}

	if (0 != (e = ctl_load(c))) return e;

	if (0 != (e = ctl_save(c))) return e;

	ctl_free(c);

	return 0;
}
