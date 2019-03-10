#pragma once

#include <stdint.h>

struct midi {
	// file descriptor used to write MIDI:
	int fd;

	// 256 byte buffer for batching up SysEx data to send in one write() call:
	uint8_t sysex[256];
	size_t sysex_p;

	// cache of per-channel per-CC state used to avoid resending redundant commands:
	struct {
		uint8_t cc[128];
	} channel[16];
};

int midi_init(struct midi *m, const char *dev, int is_uart);
