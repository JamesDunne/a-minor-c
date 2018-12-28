#pragma once

#include <stdbool.h>

#include <cyaml/cyaml.h>

struct setlist
{
	const char *date;
	const char *venue;
	bool active;
	bool print;

	const char **song_names;
	int song_names_count;
};

static const cyaml_schema_value_t string_ptr_schema = {
	CYAML_VALUE_STRING(CYAML_FLAG_POINTER, char, 0, CYAML_UNLIMITED),
};

static const cyaml_schema_field_t setlist_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
		"date", CYAML_FLAG_POINTER,
		struct setlist, date, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_STRING_PTR(
		"venue", CYAML_FLAG_POINTER,
		struct setlist, venue, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_BOOL(
		"active", CYAML_FLAG_OPTIONAL,
		struct setlist, active
	),
	CYAML_FIELD_BOOL(
		"print", CYAML_FLAG_OPTIONAL,
		struct setlist, print
	),
	CYAML_FIELD_SEQUENCE_COUNT(
		"songs", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
		struct setlist, song_names, song_names_count,
		&string_ptr_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t setlist_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_DEFAULT,
		struct setlist,
		setlist_fields_schema
	)
};

struct setlists
{
	struct setlist *sets;
	int sets_count;
};

static const cyaml_schema_field_t setlists_fields_schema[] = {
	CYAML_FIELD_SEQUENCE_COUNT(
		"sets", CYAML_FLAG_POINTER,
		struct setlists, sets, sets_count,
		&setlist_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t setlists_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_POINTER,
		struct setlists,
		setlists_fields_schema
	)
};

enum xy_switch {
	XY_X,
	XY_Y
};

struct fx_block {
	const char *name;
	bool *on;
	enum xy_switch *xy;
};

static const cyaml_strval_t xy_switch_strings[] = {
	{ "X", XY_X },
	{ "Y", XY_Y }
};

static const cyaml_schema_field_t fx_block_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
		"name", CYAML_FLAG_POINTER,
		struct fx_block, name, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_BOOL(
		"on", CYAML_FLAG_OPTIONAL | CYAML_FLAG_POINTER,
		struct fx_block, on
	),
	CYAML_FIELD_ENUM(
		"xy", CYAML_FLAG_OPTIONAL | CYAML_FLAG_POINTER,
		struct fx_block, xy,
		xy_switch_strings, CYAML_ARRAY_LEN(xy_switch_strings)
	),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t fx_block_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_DEFAULT,
		struct fx_block,
		fx_block_fields_schema
	)
};

struct amp_tone_definition {
	const char *name;
	int gain;
	double volume_dB; // volume

	struct fx_block *blocks;
	int blocks_count;
};

static const cyaml_schema_field_t amp_tone_definition_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
		"name", CYAML_FLAG_POINTER,
		struct amp_tone_definition, name, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_INT(
		"gain", CYAML_FLAG_OPTIONAL,
		struct amp_tone_definition, gain
	),
	CYAML_FIELD_FLOAT(
		"volume", CYAML_FLAG_OPTIONAL,
		struct amp_tone_definition, volume_dB
	),
	// CYAML_FIELD_IGNORE("blocks", CYAML_FLAG_OPTIONAL),
	CYAML_FIELD_SEQUENCE_COUNT(
		"blocks", CYAML_FLAG_POINTER,
		struct amp_tone_definition, blocks, blocks_count,
		&fx_block_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t amp_tone_definition_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_DEFAULT,
		struct amp_tone_definition,
		amp_tone_definition_fields_schema
	)
};

struct fx_block_definition {
	const char *name;
	int enabled_switch_cc;
	int *xy_switch_cc;     // optional
};

static const cyaml_schema_field_t fx_block_definition_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
		"name", CYAML_FLAG_POINTER,
		struct fx_block_definition, name, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_UINT(
		"enabled_switch_cc", CYAML_FLAG_OPTIONAL,
		struct fx_block_definition, enabled_switch_cc
	),
	CYAML_FIELD_UINT(
		"xy_switch_cc", CYAML_FLAG_OPTIONAL | CYAML_FLAG_POINTER,
		struct fx_block_definition, xy_switch_cc
	),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t fx_block_definition_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_DEFAULT,
		struct fx_block_definition,
		fx_block_definition_fields_schema
	)
};

struct amp_definition {
	const char *name;

	// TODO: generalize MIDI CCs with slider controls? or add gate input threshold as a CC

	// MIDI CC of external controller that is mapped to gain:
	int gain_controller_cc;
	// MIDI CC of external controller that is mapped to volume:
	int volume_controller_cc;

	// Available FX blocks for this amp in this MIDI program, including amp, cab, gate, etc.:
	struct fx_block_definition *blocks;
	int blocks_count;

	// Available general tones for this amp and their block settings, e.g. clean, dirty, acoustic:
	struct amp_tone_definition *tones;
	int tones_count;
};

static const cyaml_schema_field_t amp_definition_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
		"name", CYAML_FLAG_POINTER,
		struct amp_definition, name, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_UINT(
		"gain_controller_cc", CYAML_FLAG_OPTIONAL,
		struct amp_definition, gain_controller_cc
	),
	CYAML_FIELD_UINT(
		"volume_controller_cc", CYAML_FLAG_OPTIONAL,
		struct amp_definition, volume_controller_cc
	),
	CYAML_FIELD_SEQUENCE_COUNT(
		"blocks", CYAML_FLAG_POINTER,
		struct amp_definition, blocks, blocks_count,
		&fx_block_definition_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_SEQUENCE_COUNT(
		"tones", CYAML_FLAG_POINTER,
		struct amp_definition, tones, tones_count,
		&amp_tone_definition_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t amp_definition_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_DEFAULT,
		struct amp_definition,
		amp_definition_fields_schema
	)
};

#if 0
struct song_fx_block_override {
    string name;
    bool? on;
    xy_switch? xy;
}

struct song_amp_tone_override {
    string name;
    int? gain;
    double? volume_dB;

    struct song_fx_block_override *blocks;
    int blocks_count;
}

struct scene_amp_tone_selection /*: song_amp_tone_override*/ {
    string tone;
    string name;
    int? gain;
    double? volume_dB;

    struct song_fx_block_override *blocks;
    int blocks_count;
}

struct scene_descriptor {
    string name;

    struct scene_amp_tone_selection *amps;
    int amps_count;
}

struct song_amp_overrides {
    struct song_amp_tone_override *tones;
    int tones_count;
}
#endif

struct song {
    const char *name;
    const char *short_name;
    const char **alternate_names;
    int alternate_names_count;

    const char *who_starts;

    int tempo;

    struct song_amp_overrides *amps;
    int amps_count;

    struct scene_descriptor *scenes;
    int scenes_count;
};

static const cyaml_schema_field_t song_fields_schema[] = {
	CYAML_FIELD_STRING_PTR(
		"name", CYAML_FLAG_POINTER,
		struct song, name, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_STRING_PTR(
		"short_name", CYAML_FLAG_POINTER,
		struct song, short_name, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_SEQUENCE_COUNT(
		"alternate_names", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
		struct song, alternate_names, alternate_names_count,
		&string_ptr_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_STRING_PTR(
		"who_starts", CYAML_FLAG_POINTER,
		struct song, who_starts, 0, CYAML_UNLIMITED
	),

	CYAML_FIELD_INT(
		"tempo", CYAML_FLAG_OPTIONAL,
		struct song, tempo
	),

	CYAML_FIELD_IGNORE("amps", CYAML_FLAG_OPTIONAL),
	// CYAML_FIELD_SEQUENCE_COUNT(
	// 	"amps", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
	// 	struct song, amps, amps_count,
	// 	&song_schema, 0, CYAML_UNLIMITED
	// ),
	CYAML_FIELD_IGNORE("scenes", CYAML_FLAG_OPTIONAL),
	// CYAML_FIELD_SEQUENCE_COUNT(
	// 	"scenes", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
	// 	struct song, scenes, scenes_count,
	// 	&song_schema, 0, CYAML_UNLIMITED
	// ),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t song_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_DEFAULT,
		struct song,
		song_fields_schema
	)
};

struct midi_program {
	int program_number;

	struct amp_definition *amps;
	int amps_count;

	struct song *songs;
	int songs_count;
};

static const cyaml_schema_field_t midi_program_fields_schema[] = {
	CYAML_FIELD_INT(
		"midi", CYAML_FLAG_OPTIONAL,
		struct midi_program, program_number
	),
	CYAML_FIELD_SEQUENCE_COUNT(
		"amps", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
		struct midi_program, amps, amps_count,
		&amp_definition_schema, 0, CYAML_UNLIMITED
	),

	// CYAML_FIELD_IGNORE("songs", CYAML_FLAG_OPTIONAL),
	CYAML_FIELD_SEQUENCE_COUNT(
		"songs", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
		struct midi_program, songs, songs_count,
		&song_schema, 0, CYAML_UNLIMITED
	),

	CYAML_FIELD_END
};

static const cyaml_schema_value_t midi_program_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_DEFAULT,
		struct midi_program,
		midi_program_fields_schema
	)
};

struct midi_programs
{
	struct midi_program *midi_programs;
	int midi_programs_count;
};

static const cyaml_schema_field_t midi_programs_fields_schema[] = {
	CYAML_FIELD_SEQUENCE_COUNT(
		"midi_programs", CYAML_FLAG_POINTER,
		struct midi_programs, midi_programs, midi_programs_count,
		&midi_program_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t midi_programs_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_POINTER,
		struct midi_programs,
		midi_programs_fields_schema
	)
};

#if 0
public class LiveFX
{
    public string BlockName;

    public int EnabledCC;
    public int? XYSwitchCC;

    public bool? Enabled;
    public XYSwitch? XY;
}

public class LiveAmp
{
    public AmpDefinition AmpDefinition;
    public AmpToneDefinition ToneDefinition;

    public SongAmpToneOverride SongTone;

    public SceneDescriptor SceneDescriptor;
    public SceneAmpToneSelection SceneTone;

    public int Gain;
    public int VolumeMIDI;

    public List<LiveFX> FX;

    public float Volume => (float)Controller.MIDItoDB(VolumeMIDI);
}
#endif
