#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#ifdef __linux
#  define termios asmtermios
#  define termio asmtermio
#  define winsize asmwinsize
#  include <asm/termios.h>
#  undef  winsize
#  undef  termio
#  undef  termios
#endif
#include <sys/ioctl.h>

#include "midi.h"

// Open device for MIDI communications and set baud rate to 31250 per MIDI standard:
int midi_init(struct midi *m, const char *dev, int is_uart) {
    char err[100];
#ifdef __linux
    struct termios2 tio;
#endif

    if (dev == NULL) {
        // Use "Fore" USB-MIDI adapter device on Raspberry Pi 3:
        // P:  Vendor=552d ProdID=4348 Rev=02.11
        // S:  Product=USB Midi
        dev = "/dev/snd/midiC1D0";
        is_uart = 0;
    }

    m->fd = open(dev, (is_uart ? (O_RDWR | O_NOCTTY) : O_WRONLY) | O_NONBLOCK);
    if (m->fd == -1) {
        sprintf(err, "open('%s')", dev);
        perror(err);
        return 1;
    } else {
        perror(err);
    }

    if (is_uart) {
        // Requirements:
        //   * UART0 is enabled
        //   * UART0 is unused by the system for any ttys (remove mention of ttyAMA0 from `/etc/inittab`)
        //   * UART0 clock rate is set to default of 3MHz in /boot/config.txt
        //     * `init_uart_clock=3000000`
        //   * UART0 clock rate for Linux is set to default of 3MHz in /boot/cmdline.txt
        //     * `bcm2708.uart_clock=3000000`
        //
        // NOTE: Any hacks to change the default UART clock speed of the RPi B to adjust the
        // standard baud rates of 38400 to actually be ~31250 DO NOT WORK on latest Raspbian
        // kernels. I'm using `Linux 4.1.18 armv6l` as of 2017-02-20.
        //
        // The only thing that DOES work on latest Raspbian kernels is the code below which uses
        // termios2 to set a custom baud rate of 31250 which is the MIDI standard.
        
        // Set baud rate to 31250 for MIDI:
#ifdef __linux
        ioctl(m->fd, TCGETS2, &tio);
        tio.c_cflag &= ~CBAUD;
        tio.c_cflag |= BOTHER;
        tio.c_ispeed = 31250;
        tio.c_ospeed = 31250;
        ioctl(m->fd, TCSETS2, &tio);
#endif
    }

    m->sysex_p = 0;

    return 0;
}

void midi_send_cmd1_impl(struct midi *m, uint8_t cmd_byte, uint8_t data1) {
    ssize_t count;
    uint8_t buf[2];
    buf[0] = cmd_byte;
    buf[1] = data1;
    count = write(m->fd, buf, 2);
    if (count != 2) {
        perror("Error sending MIDI bytes");
        return;
    }
    fprintf(stderr, "MIDI: %02X %02X\n", cmd_byte, data1);
}

void midi_send_cmd2_impl(struct midi *m, uint8_t cmd_byte, uint8_t data1, uint8_t data2) {
    ssize_t count;
    uint8_t buf[3];
    buf[0] = cmd_byte;
    buf[1] = data1;
    buf[2] = data2;
    count = write(m->fd, buf, 3);
    if (count != 3) {
        perror("Error sending MIDI bytes");
        return;
    }
    fprintf(stderr, "MIDI: %02X %02X %02X\n", cmd_byte, data1, data2);
}

// Buffer up SysEx data until terminating F7 byte is encountered:
void midi_send_sysex(struct midi *m, uint8_t byte) {
    //fprintf(stderr, "MIDI: %02X\n", byte);

    if (m->sysex_p >= 256) {
        fprintf(stderr, "MIDI SysEx data too large (>= 256 bytes)\n");
        return;
    }

    // Buffer data:
    m->sysex[m->sysex_p++] = byte;

    if (byte == 0xF7) {
        size_t i;
        size_t write_count = m->sysex_p;
        m->sysex_p = 0;

        fprintf(stderr, "MIDI SysEx:");
        for (i = 0; i < write_count; i++) {
            fprintf(stderr, " %02X", m->sysex[i]);
        }
        fprintf(stderr, "\n");

        ssize_t count = write(m->fd, m->sysex, write_count);
        if (count < 0) {
            perror("write in midi_send_sysex");
            return;
        }
        if (count != write_count) {
            fprintf(stderr, "midi_send_sysex write didnt write enough bytes\n");
            return;
        }
    }
}
