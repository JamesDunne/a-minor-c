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
		printf("%s %s\n", setlists->sets[i].date, setlists->sets[i].venue);
	}

	return 0;
}
