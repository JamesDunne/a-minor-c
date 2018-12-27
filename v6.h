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
	const char **songNames;
	int songCount;
};

struct setlists
{
	struct setlist *sets;
	int setCount;
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
		struct setlist, songNames, songCount,
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

static const cyaml_schema_field_t setlists_fields_schema[] = {
	CYAML_FIELD_SEQUENCE_COUNT(
		"sets", CYAML_FLAG_POINTER,
		struct setlists, sets, setCount,
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
struct FXBlockDefinition
{
    [YamlMember(Alias = "EnabledSwitchCc")]
    public int EnabledSwitchCC { get; set; }
    [YamlMember(Alias = "XySwitchCc")]
    public int? XYSwitchCC { get; set; }
}

public class AmpDefinition
{
    [YamlIgnore]
    public MidiProgram MidiProgram { get; set; }
    [YamlIgnore]
    public int AmpNumber { get; set; }

    public string Name { get; set; }

    // Available FX blocks for this amp in this MIDI program, including amp, cab, gate, etc.:
    public Dictionary<string, FXBlockDefinition> Blocks { get; set; }

    // MIDI CC of external controller that is mapped to gain:
    [YamlMember(Alias = "GainControllerCc")]
    public int GainControllerCC { get; set; }
    // MIDI CC of external controller that is mapped to volume:
    [YamlMember(Alias = "VolumeControllerCc")]
    public int VolumeControllerCC { get; set; }

    // Available general tones for this amp and their block settings, e.g. clean, dirty, acoustic:
    public Dictionary<string, AmpToneDefinition> Tones { get; set; }
}

public enum XYSwitch
{
    X,
    Y
}

public class FXBlock
{
    public bool? On { get; set; }
    [YamlMember(Alias = "Xy")]
    public XYSwitch? XY { get; set; }
}

public class AmpToneDefinition
{
    [YamlIgnore]
    public AmpDefinition AmpDefinition { get; set; }

    public int Gain { get; set; }
    [YamlMember(Alias = "Volume")]
    public double VolumeDB { get; set; }
    [YamlIgnore]
    public int Volume { get; set; }

    public Dictionary<string, FXBlock> Blocks { get; set; }
}

public class SongFXBlockOverride
{
    public bool? On { get; set; }
    [YamlMember(Alias = "Xy")]
    public XYSwitch? XY { get; set; }
}

public class SongAmpToneOverride
{
    [YamlIgnore]
    public AmpToneDefinition AmpToneDefinition { get; set; }

    public int? Gain { get; set; }
    [YamlMember(Alias = "Volume")]
    public double? VolumeDB { get; set; }
    [YamlIgnore]
    public int? Volume { get; set; }

    public Dictionary<string, SongFXBlockOverride> Blocks { get; set; }
}

public class SceneAmpToneSelection : SongAmpToneOverride
{
    [YamlIgnore]
    public int AmpNumber { get; set; }

    public string Tone { get; set; }
}

public class SceneDescriptor
{
    [YamlIgnore]
    public Song Song { get; set; }
    [YamlIgnore]
    public int SceneNumber { get; set; }

    public string Name { get; set; }
    public List<SceneAmpToneSelection> Amps { get; set; }
}

public class SongAmpOverrides
{
    [YamlIgnore]
    public AmpDefinition AmpDefinition { get; set; }
    [YamlIgnore]
    public int AmpNumber { get; set; }

    public Dictionary<string, SongAmpToneOverride> Tones { get; set; }
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

public class MidiProgram
{
    [YamlMember(Alias = "midi", ApplyNamingConventions = false)]
    public int ProgramNumber { get; set; }

    public List<AmpDefinition> Amps { get; set; }

    public List<Song> Songs { get; set; }
}

public class AllPrograms
{
    public List<MidiProgram> MidiPrograms { get; set; }
}

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
