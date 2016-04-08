//
// Data Compression Project -- CS 4310
//
// @author Alec Carpenter <alecgunnar@gmail.com>
// @date   April 3, 2016
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lzss.h"
#include "boyer_moore.h"
#include "error.h"

// External variables
FILE *input_file;

// Global variables
byte *window;
byte *buffer;

int WIN_SIZE;
int BUF_SIZE;

int window_length = 0;
int buffer_length = 0;

int last_offset = 0;

int finished_reading = 0;

// Prototypes
void write_prefix_match_data(byte length);
void fill_buffer();
int longest_prefix();
int prefix_offset(int length);
void shift_window(int length);

// Start-up and shutdown functions

void standard_start () {
    configured_start(WINDOW_SIZE, BUFFER_SIZE);
}

void configured_start (int win_size, int buf_size) {
    WIN_SIZE = win_size;
    BUF_SIZE = buf_size;

    window = malloc(sizeof(byte) * win_size);
    buffer = malloc(sizeof(byte) * buf_size);
}

void shutdown () {
    free(window);
    free(buffer);
}

// Compression functions

int compress () {
    standard_start();

    fill_buffer();

    int length;

    while (buffer_length > 0) {
        length = longest_prefix();
// printf("[%s]\n", window);
        if (length >= MINIMUM_PREFIX_LENGTH) {
            // printf("<%d, %d>", last_offset, length);

            write_prefix_match_data(length);

            shift_window(length);
        } else {
            putchar(buffer[0]);
            shift_window(1);
        }
    }

    shutdown();

    return 1;
}

void write_prefix_match_data (byte length) {
    byte offset = (byte) (last_offset * -1);

    fwrite(&offset, sizeof(byte), 1, stdout);
    fwrite(&length, sizeof(byte), 1, stdout);
}

int longest_prefix () {
    int length = 1;

    while (length <= buffer_length)
        if (prefix_offset(length++) < 0)
            break;

    length--;

    if (length > 1)
        return length;

    return -1;
}

int prefix_offset (int length) {
    int offset = get_offset(window, window_length, buffer, length);

    if (offset >= 0)
        last_offset = offset;

    return offset;
}

void shift_window (int length) {
    int i;

    if ((window_length + length) > WIN_SIZE) {
        int diff = (window_length + length) - WIN_SIZE;

        for (i = 0; i < window_length - diff; i++)
            window[i] = window[i + diff];

        window_length -= diff;
    }

    for (i = 0; i < length; i++)
        window[window_length++] = buffer[i];

    for (i = 0; i < buffer_length - length; i++)
        buffer[i] = buffer[i + length];

    window[window_length] = 0;
    buffer[buffer_length] = 0;

    buffer_length -= length;

    fill_buffer();
}

void fill_buffer () {
    if (input_file == NULL || finished_reading)
        return;

    buffer_length += fread(buffer + buffer_length, sizeof(byte), BUFFER_SIZE - buffer_length, input_file);

    if (buffer_length < BUFFER_SIZE)
        finished_reading = 1;
}

// Decompression functions

int decompress () {
    return 0;
}
