#include <stdlib.h>
#include <stdio.h>

#include "controller.h"

int main(int argc, char **argv) {
	struct controller *c = ctl_new();

	ctl_load(c);

	ctl_save(c);

	ctl_free(c);

	return 0;
}
