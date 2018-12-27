#include <stdlib.h>
#include <stdio.h>

#include "yaml.h"

int main(int argc, char **argv) {
	struct setlists *setlists = NULL;

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

	return 0;
}
