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

#if 0
	for (int i = 0; i < setlists->sets_count; i++) {
		const struct setlist *setlist = &setlists->sets[i];
		printf("%s %s\n", setlist->date, setlist->venue);
		for (int j = 0; j < setlist->song_names_count; j++) {
			printf("  %2d) %s\n", j+1, setlist->song_names[j]);
		}
	}
#endif

	// Load programs:
	err = load_programs(&midi_programs);
	if (!err) {
		return 1;
	}

	for (int i = 0; i < midi_programs->midi_programs_count; i++) {
		const struct midi_program *midi_program = &midi_programs->midi_programs[i];

		printf("MIDI: %d\n", midi_program->program_number);
		for (int a = 0; a < midi_program->amps_count; a++) {
			const struct amp_definition *amp = &midi_program->amps[a];

			printf("  Amp[%d]\n", a);
			printf("    Name:      %s\n", amp->amp_name);
			printf("    Gain CC:   %d\n", amp->gain_controller_cc);
			printf("    Volume CC: %d\n", amp->volume_controller_cc);

			printf("    Blocks:\n");
			for (int b = 0; b < amp->blocks_count; b++) {
				const struct fx_block_definition *block = &amp->blocks[b];

				printf("      Name:         %s\n", block->block_name);
				printf("        Enabled CC:   %d\n", block->enabled_switch_cc);
				if (block->xy_switch_cc) {
					printf("        XY Switch CC: %d\n", *block->xy_switch_cc);
				}
			}

			printf("    Tones:\n");
			for (int b = 0; b < amp->tones_count; b++) {
				const struct amp_tone_definition *tone = &amp->tones[b];

				printf("      Name:         %s\n", tone->tone_name);
				printf("        Gain:       %02x\n", tone->gain);
				printf("        Volume(dB): %.2f\n", tone->volume_dB);
				printf("        Blocks:\n");
				for (int b = 0; b < tone->blocks_count; b++) {
					const struct fx_block *block = &tone->blocks[b];

					printf("          Name: %s\n", block->block_name);
					if (block->on) {
						printf("            On: %d\n", *block->on);
					}
					if (block->xy) {
						printf("            XY: %s\n", *block->xy == XY_X ? "X" : "Y");
					}
				}
			}
		}

		for (int s = 0; s < midi_program->songs_count; s++) {
			const struct song *song = &midi_program->songs[s];

			printf("  Song[%d]\n", s);
			printf("    Name:      %s\n", song->name);
			printf("    Tempo:     %d\n", song->tempo);
			for (int a = 0; a < song->amps_count; a++) {
				const struct song_amp_overrides *amp = &song->amps[a];

				printf("    Amp[%s]\n", midi_program->amps[a].amp_name);
				for (int t = 0; t < amp->tones_count; ++t) {
					const struct song_amp_tone_override *tone = &amp->tones[t];

					printf("      Tone: %s\n", tone->tone_name);
					if (tone->gain) {
						printf("        Gain:   %02x\n", *tone->gain);
					}
					if (tone->volume_dB) {
						printf("        Volume: %.2f\n", *tone->volume_dB);
					}
				}
			}

			// scenes
			for (int c = 0; c < song->scenes_count; c++) {
				const struct scene_descriptor *scene = &song->scenes[c];

				printf("    Scene[%d]\n", c+1);
				printf("      Name: %s\n", scene->scene_name);

				for (int a = 0; a < scene->amps_count; a++) {
					const struct scene_amp_tone_selection *tone = &scene->amps[a];

					printf("      Amp[%s]\n", midi_program->amps[a].amp_name);
					printf("        Tone: %s\n", tone->tone_name);
					if (tone->gain) {
						printf("          Gain:   %02x\n", *tone->gain);
					}
					if (tone->volume_dB) {
						printf("          Volume: %.2f\n", *tone->volume_dB);
					}
				}
			}
		}
	}

	free_setlists(&setlists);
	free_programs(&midi_programs);

	return 0;
}
