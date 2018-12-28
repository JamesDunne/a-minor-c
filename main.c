#include <stdlib.h>
#include <stdio.h>

#include "yaml.h"

int main(int argc, char **argv) {
	struct setlists *setlists = NULL;
	struct midiPrograms *midiPrograms = NULL;

	// Load setlists:
	bool err = load_setlists(&setlists);
	if (!err) {
		return 1;
	}

	for (int i = 0; i < setlists->setCount; i++) {
		const struct setlist *setlist = &setlists->sets[i];
		printf("%s %s\n", setlist->date, setlist->venue);
		for (int j = 0; j < setlist->songCount; j++) {
			printf("  %2d) %s\n", j+1, setlist->songNames[j]);
		}
	}

	// Load programs:
	bool err = load_midiPrograms(&midiPrograms);
	if (!err) {
		return 1;
	}

	// cyaml_free(&config, &setlists_schema, setlists, 0);
	// cyaml_free(&config, &midiPrograms_schema, midiPrograms, 0);

	return 0;
}
