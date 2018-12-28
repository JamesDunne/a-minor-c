#pragma once

#include <stdbool.h>

#include <cyaml/cyaml.h>

struct setlist
{
	const char *date;
	const char *venue;
	bool active;
	bool print;

	//[YamlMember(Alias = "Songs")]
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

#if 0
public enum XYSwitch
{
    X,
    Y
}

public class FXBlock
{
    public string Name { get; set; }
    public bool? On { get; set; }
    [YamlMember(Alias = "Xy")]
    public XYSwitch? XY { get; set; }
}

public class AmpToneDefinition
{
    [YamlIgnore]
    public AmpDefinition AmpDefinition { get; set; }

    public string Name { get; set; }
    public int Gain { get; set; }
    [YamlMember(Alias = "Volume")]
    public double VolumeDB { get; set; }
    [YamlIgnore]
    public int Volume { get; set; }

    public List<FXBlock> Blocks { get; set; }
}

public class FXBlockDefinition
{
    public string Name { get; set; }
    [YamlMember(Alias = "EnabledSwitchCc")]
    public int EnabledSwitchCC { get; set; }
    [YamlMember(Alias = "XySwitchCc")]
    public int? XYSwitchCC { get; set; }
}
#endif

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
		CYAML_FLAG_POINTER,
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

	// // Available general tones for this amp and their block settings, e.g. clean, dirty, acoustic:
	// struct amp_tone_definition *tones;
	// int tones_count;
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
	// CYAML_FIELD_IGNORE("blocks", CYAML_FLAG_OPTIONAL),
	CYAML_FIELD_SEQUENCE_COUNT(
		"blocks", CYAML_FLAG_POINTER,
		struct amp_definition, blocks, blocks_count,
		&fx_block_definition_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_IGNORE("tones", CYAML_FLAG_OPTIONAL),
	// CYAML_FIELD_SEQUENCE_COUNT(
	// 	"tones", CYAML_FLAG_POINTER,
	// 	struct amp_definition, tones, tones_count,
	// 	&amp_tone_definition_schema, 0, CYAML_UNLIMITED
	// ),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t amp_definition_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_POINTER,
		struct amp_definition,
		amp_definition_fields_schema
	)
};

#if 0
public class SceneDescriptor
{
    [YamlIgnore]
    public Song Song { get; set; }
    [YamlIgnore]
    public int SceneNumber { get; set; }

    public string Name { get; set; }
    public List<SceneAmpToneSelection> Amps { get; set; }
}

public class SongFXBlockOverride
{
    public string Name { get; set; }
    public bool? On { get; set; }
    [YamlMember(Alias = "Xy")]
    public XYSwitch? XY { get; set; }
}

public class SongAmpToneOverride
{
    [YamlIgnore]
    public AmpToneDefinition AmpToneDefinition { get; set; }

    public string Name { get; set; }
    public int? Gain { get; set; }
    [YamlMember(Alias = "Volume")]
    public double? VolumeDB { get; set; }
    [YamlIgnore]
    public int? Volume { get; set; }

    public List<SongFXBlockOverride> Blocks { get; set; }
}

public class SongAmpOverrides
{
    [YamlIgnore]
    public AmpDefinition AmpDefinition { get; set; }
    [YamlIgnore]
    public int AmpNumber { get; set; }

    public List<SongAmpToneOverride> Tones { get; set; }
}

public class SceneAmpToneSelection : SongAmpToneOverride
{
    [YamlIgnore]
    public int AmpNumber { get; set; }

    public string Tone { get; set; }
}

public class Song
{
    [YamlIgnore]
    public MidiProgram MidiProgram { get; set; }

    public string Name { get; set; }
    public string ShortName { get; internal set; }
    public List<string> AlternateNames { get; internal set; }
    public string WhoStarts { get; internal set; }

    public int Tempo { get; set; }

    public List<SongAmpOverrides> Amps { get; set; }

    [YamlMember(Alias = "scenes", ApplyNamingConventions = false)]
    public List<SceneDescriptor> SceneDescriptors { get; set; }

    public bool MatchesName(string match)
    {
        return String.Compare(match, Name, true) == 0
            || String.Compare(match, ShortName, true) == 0
            || AlternateNames.Any(name => String.Compare(match, name, true) == 0);
    }
}
#endif

struct midi_program
{
	int program_number;

	struct amp_definition *amps;
	int amps_count;

	// TODO
	// struct *song songs;
	// int song_count;
};

static const cyaml_schema_field_t midi_program_fields_schema[] = {
	CYAML_FIELD_UINT(
		"midi", CYAML_FLAG_OPTIONAL,
		struct midi_program, program_number
	),
	CYAML_FIELD_SEQUENCE_COUNT(
		"amps", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
		struct midi_program, amps, amps_count,
		&amp_definition_schema, 0, CYAML_UNLIMITED
	),
	CYAML_FIELD_IGNORE("songs", CYAML_FLAG_OPTIONAL),
	// CYAML_FIELD_SEQUENCE_COUNT(
	// 	"songs", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
	// 	struct midi_program, songs, song_count,
	// 	&song_schema, 0, CYAML_UNLIMITED
	// ),
	CYAML_FIELD_END
};

static const cyaml_schema_value_t midi_program_schema = {
	CYAML_VALUE_MAPPING(
		CYAML_FLAG_POINTER,
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
