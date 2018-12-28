#include <stdlib.h>
#include <stdio.h>

#include "yaml.h"

int main(int argc, char **argv) {
	struct setlists *setlists = NULL;
	struct midi_programs *midi_programs = NULL;

	// Load setlists:
	bool err = load_setlists(&setlists);
	if (!err) {
		return 1;
	}

	for (int i = 0; i < setlists->setCount; i++) {
		const struct setlist *setlist = &setlists->sets[i];
		printf("%s %s\n", setlist->date, setlist->venue);
		for (int j = 0; j < setlist->song_names_count; j++) {
			printf("  %2d) %s\n", j+1, setlist->song_names[j]);
		}
	}

	// Load programs:
	bool err = load_midiPrograms(&midi_programs);
	if (!err) {
		return 1;
	}

	// cyaml_free(&config, &setlists_schema, setlists, 0);
	// cyaml_free(&config, &midiPrograms_schema, midi_programs, 0);

	return 0;
}
