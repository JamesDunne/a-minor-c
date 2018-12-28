#pragma once

#include "v6.h"

bool load_setlists(struct setlists **setlists);
bool load_programs(struct midi_programs **midi_programs);

bool free_setlists(struct setlists **setlists);
bool free_programs(struct midi_programs **midi_programs);
