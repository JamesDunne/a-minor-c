#include <stdlib.h>
#include <stdio.h>

#include "yaml.h"

int main(int argc, char **argv) {
	struct setlists *setlists = NULL;
	struct midi_programs *midi_programs = NULL;
	bool err = false;

	// Load setlists:
	err = load_setlists(&setlists);
	if (!err) {
		return 1;
	}

	for (int i = 0; i < setlists->sets_count; i++) {
		const struct setlist *setlist = &setlists->sets[i];
		printf("%s %s\n", setlist->date, setlist->venue);
		for (int j = 0; j < setlist->song_names_count; j++) {
			printf("  %2d) %s\n", j+1, setlist->song_names[j]);
		}
	}

	// Load programs:
	err = load_programs(&midi_programs);
	if (!err) {
		return 1;
	}

	for (int i = 0; i < midi_programs->midi_programs_count; i++) {
		const struct midi_program *midi_program = &midi_programs->midi_programs[i];

		printf("MIDI: %3d\n", midi_program->program_number);
		for (int a = 0; a < midi_program->amps_count; a++) {
			const struct amp_definition *amp = &midi_program->amps[a];

			printf("  Amp[%2d]\n", a);
			printf("    Name:      %s\n", amp->name);
			printf("    Gain CC:   %d\n", amp->gain_controller_cc);
			printf("    Volume CC: %d\n", amp->volume_controller_cc);

			printf("    Blocks:\n");
			for (int b = 0; b < amp->blocks_count; b++) {
				const struct fx_block_definition *block = &amp->blocks[b];

				printf("      Name:         %s\n", block->name);
				printf("      Enabled CC:   %d\n", block->enabled_switch_cc);
				if (block->xy_switch_cc) {
					printf("      XY Switch CC: %d\n", *block->xy_switch_cc);
				}
			}

			printf("    Tones:\n");
			for (int b = 0; b < amp->tones_count; b++) {
				const struct amp_tone_definition *tone = &amp->tones[b];

				printf("      Name:       %s\n", tone->name);
				printf("      Gain:       %d\n", tone->gain);
				printf("      Volume(dB): %.2f\n", tone->volume_dB);
			}
		}
	}

	// cyaml_free(&config, &setlists_schema, setlists, 0);
	// cyaml_free(&config, &midiPrograms_schema, midi_programs, 0);

	return 0;
}
